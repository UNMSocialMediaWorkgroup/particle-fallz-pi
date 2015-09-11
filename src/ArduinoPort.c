#define _BSD_SOURCE

#include "./inc/ArduinoPort.h"
#include "./inc/Util.h"
#include <string.h>  /* For memset, strcmp */
#include <unistd.h>  /* For read, close */
#include <stdio.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <fcntl.h>

#define BAUD_RATE 9600
#define FLOAT_BUFFER_SIZE 64

static int serialport_init(const char* serialport, int baud);
static void skipToAnalogTotal(ArduinoPort* port);
static float readNextFloat(ArduinoPort* port);
static void shiftBuffer(char* buffer, int buffLen);

void initArduinoPort(ArduinoPort* port, const char* filename) {
  if (port->isOpen) {
    die("Arduino is already open", 4);
  }
  port->descriptor = serialport_init(filename, BAUD_RATE);
  port->isOpen = true;
}

void closeArduinoPort(ArduinoPort* port) {
  if (!port->isOpen) {
    die("Arduino is not open", 4);
  }
  close(port->descriptor);
  port->isOpen = false;
}

u8 readNextAirbeamByte(ArduinoPort* port) {
  float f = readNextAirbeamValue(port);

  return (u32)(10.0f * ((f - 0.1f)));
}

float readNextAirbeamValue(ArduinoPort* port) {
  float f;
  skipToAnalogTotal(port);
  f = readNextFloat(port);
  return f;
}

static void skipToAnalogTotal(ArduinoPort* port) {
  char buffer[sizeof("Analog Total: ") / sizeof(char) + 1];
  u32 bufferSize = sizeof(buffer);
  int charsRead;

  if (!port->isOpen) {
    die("Arduino port is not open", 5);
  }

  memset((void*)buffer, 0, bufferSize);

  while (strcmp(buffer, "Analog Total: ")) {
    charsRead = read(port->descriptor,
                     buffer + (bufferSize - 2),
                     1);
    if (charsRead == -1) {
      die("Could not read from ArduinoPort", 6);
    } else if (charsRead == 0) {
      sleep(1);
      continue;
    }
    shiftBuffer(buffer, bufferSize);
  }
}

static float readNextFloat(ArduinoPort* port) {
  char buffer[FLOAT_BUFFER_SIZE];
  char* bp = buffer;
  int charsRead = 0;
  char lastChar;

  do {
    charsRead = read(port->descriptor,
                     &lastChar, 1);
    if (charsRead == -1) {
      die("Could not read from ArduinoPort", 6);
    } else if (charsRead == 0) {
      sleep(1);
      continue;
    } else {
      *(bp++) = lastChar;
    }
  } while (lastChar != ' ');

  *bp = 0;
  return atof(buffer);
}

static void shiftBuffer(char* buffer, int buffLen) {
  u32 i;
  for (i = 0; i < buffLen - 1; i++) {
    buffer[i] = buffer[i + 1];
  }
}

static int serialport_init(const char* serialport, int baud)
{
  struct termios toptions;
  int fd;
  speed_t brate = baud;


  fd = open(serialport, O_RDWR | O_NOCTTY);
  if (fd == -1)  {
    perror("init_serialport: Unable to open port ");
    return -1;
  }

  if (tcgetattr(fd, &toptions) < 0) {
    perror("init_serialport: Couldn't get term attributes");
    return -1;
  }

  switch(baud) {
  case 4800:   brate=B4800;   break;
  case 9600:   brate=B9600;   break;
#ifdef B14400
  case 14400:  brate=B14400;  break;
#endif
  case 19200:  brate=B19200;  break;
#ifdef B28800
  case 28800:  brate=B28800;  break;
#endif
  case 38400:  brate=B38400;  break;
  case 57600:  brate=B57600;  break;
  case 115200: brate=B115200; break;
  }
  cfsetispeed(&toptions, brate);
  cfsetospeed(&toptions, brate);

  toptions.c_cflag &= ~PARENB;
  toptions.c_cflag &= ~CSTOPB;
  toptions.c_cflag &= ~CSIZE;
  toptions.c_cflag |= CS8;

  toptions.c_cflag &= ~CRTSCTS;

  toptions.c_cflag |= CREAD | CLOCAL;
  toptions.c_iflag &= ~(IXON | IXOFF | IXANY);

  toptions.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
  toptions.c_oflag &= ~OPOST;

  toptions.c_cc[VMIN]  = 0;
  toptions.c_cc[VTIME] = 20;

  if( tcsetattr(fd, TCSANOW, &toptions) < 0) {
    perror("init_serialport: Couldn't set term attributes");
    return -1;
  }

  return fd;
}
