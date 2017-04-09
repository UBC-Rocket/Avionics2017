#ifndef MPL_H
#define MPL_H

#include <inttypes.h>
#include <Arduino.h>

typedef short int16_t;

class MPL {
  #include "testData.h"

  int lastTimeIndex;
  float groundLevel;

public:
  int begin(bool whichWire, uint8_t Addr);

  // Initialization functions
  void setGround();

  // Read data functions
  int readAGL(float *data);
  int readAlt(float *data);

  int readTemp(float *data);
private:
  int getTimeIndex();
};

#endif
