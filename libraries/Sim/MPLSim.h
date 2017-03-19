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
  int begin(bool whichWire);

  // Initialization functions
  void setGround();

  // Check for data functions
  bool getDataReady();

  // Read data functions
  float readAGL();
  float readAlt();

  float readTemp();
private:
  void debug(String msg);
  int getTimeIndex();
};

#endif
