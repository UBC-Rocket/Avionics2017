#ifndef MPUSIM_H
#define MPUSIM_H

#include <Arduino.h>
#include <inttypes.h>

typedef short int16_t;

class MPU {
  #include "testData.h"

  unsigned long start;
  int lastTimeIndex;
  float gyroFS;
  float accelFS;
public:
  int begin(bool whichWire, uint8_t Addr);
  int selfTest();

  int initGyro(uint16_t fullScale);
  int initAccel(uint8_t fullScale);
  int initMag();

  int readGyro(int16_t *data);
  int readAccel(int16_t *data);
  int readMag(int16_t *data);

  int readGyro(float data[]);
  int readAccel(float data[]);
  int readMag(float data[]);
private:
  int getTimeIndex();

};
#endif