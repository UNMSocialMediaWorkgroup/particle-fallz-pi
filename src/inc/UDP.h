#ifndef PARTICLE_FALLZ_PI_SRC_INC_UDP_H_
#define PARTICLE_FALLZ_PI_SRC_INC_UDP_H_

#include "./itype.h"
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct {
  bool initialized;
  int descriptor;
  u32 port;
  struct sockaddr_in address;
} UDPSocket;

/** Fills the given socket with information that will let
 *  it write UDP Packets. Make sure, if given a
 *  UDPSocket that has already been started, that
 */
extern void startUDPSocket(UDPSocket* socket, const u32 port);

/** Stops the given UDPSocket. This socket must have been started
 * already with startUDPSocket.
 */
extern void stopUDPSocket(UDPSocket* socket);

/** Sends an array of chars over a given UDPSocket. Note that this
 * socket needs to be open and the array of chars needs to be NULL
 * terminated.
 */
extern void sendUDPPacket(const UDPSocket* socket,
                          const char* packet,
                          const struct sockaddr* addresses,
                          const u32 addressCount);

#endif  /* PARTICLE_FALLZ_PI_SRC_INC_UDP_H_ */
