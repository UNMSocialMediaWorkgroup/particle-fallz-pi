#include "UDP.h"
#include "Util.h"
#include <string.h>

#include <arpa/inet.h>
#include <unistd.h>  /* For close */

void startUDPSocket(UDPSocket* skt, const u32 port) {
  if (skt->initialized) {
    die("Socket initialized already.", 1);
  }

  memset((char*)skt, 0, sizeof(UDPSocket));

  skt->descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (skt->descriptor == -1) {
    die("Socket not initialized properly", 1);
  }

  skt->address.sin_family = AF_INET;
  skt->address.sin_port = htons(port);
  skt->address.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(skt->descriptor,
           (struct sockaddr*)&(skt->address),
           sizeof(skt->address)) == -1) {
    die("Socket could not bind",1 );
  }

  skt->initialized = true;
}

void stopUDPSocket(UDPSocket* skt) {
  if (!skt->initialized) {
    die("Socket has not been initialized.", 2);
  }
  skt->initialized = false;
  close(skt->descriptor);
}

void sendUDPPacket(const UDPSocket* socket,
                   const char* packet,
                   const struct sockaddr* addresses,
                   const u32 addressCount) {
  int sendResp;
  u32 i;
  for (i = 0; i < addressCount; i++) {
    sendResp = sendto(socket->descriptor,
                      packet,
                      strlen(packet),
                      0, addresses + i, sizeof(struct sockaddr));
    if (sendResp == -1) {
      die("Could not send UDP packet", 3);
    }
  }
}
