#include <stdio.h>
#include <arpa/inet.h>
#include "./inc/ArduinoPort.h"
#include "./inc/UDP.h"

#define PORT 6982

int main(int argc, char** argv) {
  struct sockaddr_in ip4addr;
  UDPSocket socket;
  ArduinoPort arduino;
  char packet[2] = {0};
  bool running = true;
  float nextVal;
  socket.initialized = false;
  arduino.isOpen = false;

  startUDPSocket(&socket, PORT);
  initArduinoPort(&arduino, "/dev/ttyACM0");

  ip4addr.sin_family = AF_INET;
  ip4addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "10.0.0.10", &ip4addr.sin_addr);

  while (running) {
    nextVal = readNextAirbeamValue(&arduino);
    packet[0] = (u8)(255.0f * nextVal);
    printf("Sending byte: %u (%f)\n", packet[0] & 0xff, nextVal);
    sendUDPPacket(&socket, packet, (struct sockaddr*)&ip4addr, 1);
  }


  stopUDPSocket(&socket);
  closeArduinoPort(&arduino);

  return 0;
}
