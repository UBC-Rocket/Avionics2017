#ifndef MPUSIM_H
#define MPUSIM_H

#include <Arduino.h>
#include <inttypes.h>
#include "testData.h"

typedef short int16_t;

class MPU {
  unsigned long start;
  int lastTimeIndex;
  uint8_t gyroFS;
  uint16_t accelFS;
public:
  begin(bool whichWire, uint8_t Addr);
  int initGyro(uint16_t fullScale);
  int initAccel(uint8_t fullScale);
  int initMag();

  int readGyro(int16_t *data);
  int readAccel(int16_t *data);
  int readMag(int16_t *data);

  int loadDMP();
private:
  int getTimeIndex();

};
#endif
