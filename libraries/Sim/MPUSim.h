#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <inttypes.h>

#define BUF_LENGTH 100

typedef short int16_t;

class MPU {
  unsigned int times[BUF_LENGTH];
  short gyros[BUF_LENGTH];
  short accels[BUF_LENGTH];
  short mags[BUF_LENGTH];

  int filePos;
  int lastPos;

  unsigned long start;
  float gyroFS;
  float accelFS;
  int addr;
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
  int getPos(unsigned int time);
  int updateBuffer();
};
#endif
