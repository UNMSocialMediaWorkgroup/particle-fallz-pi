#ifndef PARTICLE_FALLZ_PI_SRC_INC_AIRBEAMREADER_H_
#define PARTICLE_FALLZ_PI_SRC_INC_AIRBEAMREADER_H_

#include "./itype.h"

typedef struct {
  bool isOpen;
  int descriptor;
} ArduinoPort;

extern void initArduinoPort(ArduinoPort* port, const char* filename);

extern void closeArduinoPort(ArduinoPort* port);

extern float readNextAirbeamValue(ArduinoPort* port);

#endif  /* PARTICLE_FALLZ_PI_SRC_INC_AIRBEAMREADER_H_ */
