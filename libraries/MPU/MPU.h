#ifndef MPU_H
#define MPU_H

#include <Arduino.h>
#include <inttypes.h>

typedef short int16_t;

class MPU {
  bool wire;
  uint8_t addr;
  float gyroFS, accelFS;
public:
  int begin(bool whichWire, uint8_t Addr);
  int selfTest();

  int initGyro(uint16_t fullScale);
  int initAccel(uint8_t fullScale);
  int initMag();

  int readGyro(int16_t *data);
  int readAccel(int16_t *data);
  int readMag(int16_t *data);

  int readGyro(float *data);
  int readAccel(float *data);

  int readDMP(long quat[]);
  int loadDMP();

  int enableDMP(bool enable);
private:
  void debug(String msg);
  int writeMem(uint16_t addr, uint8_t length, uint8_t *data);
  int readMem(uint16_t addr, uint8_t length, uint8_t *data);

  int write(uint8_t reg, uint8_t data);
  int write(uint8_t reg, uint8_t length, uint8_t *data);
  uint8_t read(uint8_t reg);
  int read(uint8_t reg, uint8_t length, uint8_t *data);
};
#endif
