#include "./inc/Util.h"
#include <stdio.h>  /* For perror */
#include <stdlib.h> /* For exit */

void die(const char* message, u32 errorCode) {
  perror(message);
  exit(errorCode);
}

void die1(const char* message) {
  die(message, 1);
}
