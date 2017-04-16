#include "MPUSim.h"
#include <i2c_t3.h>

#define WHOAMI 0x1A

int MPU::begin(bool whichWire, uint8_t Addr) {
  start = micros();
  lastTimeIndex = 0;
  return 0;
}

int MPU::selfTest() {
  uint8_t test;
  test = WHOAMI;

  return test;
}

int MPU::initGyro(uint16_t fullScale) {
  uint8_t regValue;

  switch (fullScale) {
    case 250: regValue = 0b00; break;
    case 500: regValue = 0b01; break;
    case 1000: regValue = 0b10; break;
    case 2000: regValue = 0b11; break;
    default: return -1; break;
  }

  gyroFS = (float)fullScale;
  return 0;
}

int MPU::initAccel(uint8_t fullScale) {
  uint8_t regValue;

  switch (fullScale) {
    case 2: regValue = 0b00; break;
    case 4: regValue = 0b01; break;
    case 8: regValue = 0b10; break;
    case 16: regValue = 0b11; break;
    default: return -1; break;
  }

  accelFS = (float)fullScale;
  return 0;
}

int MPU::initMag() {
  return 0;
}

int MPU::readGyro(int16_t *data) {
  int gyro = gyros[getTimeIndex()];
  gyro = gyro * 2000.0 / gyroFS;

  data[0] = 0;
  data[1] = 0;
  data[2] = gyro;
  return 0;
}

int MPU::readAccel(int16_t *data) {
  int accel = accels[getTimeIndex()];
  //accel = accel * 16.0 / accelFS;

  data[0] = 0;
  data[1] = 0;
  data[2] = accel;
  return 0;
}

int MPU::readMag(int16_t* data) {
  int mag = mags[getTimeIndex()];

  data[0] = 0;
  data[1] = 0;
  data[2] = mag;
  return 0;
}

int MPU::readGyro(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readGyro(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = rawData[x] * gyroFS / INT16_MAX;
  }
  return 0;
}

int MPU::readAccel(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readAccel(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = rawData[x] * accelFS / INT16_MAX;
  }
  return 0;
}

int MPU::readMag(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readMag(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = (float)rawData[x];
  }
  return 0;
}

int MPU::getTimeIndex() {
  unsigned long time = micros() - start;

  while(time >= times[lastTimeIndex + 1]) {
    lastTimeIndex++;
  }

  return lastTimeIndex;
}
