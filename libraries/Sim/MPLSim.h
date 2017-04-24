#ifndef MPL_H
#define MPL_H

#include <inttypes.h>
#include <Arduino.h>

#define BUF_LENGTH 100


typedef short int16_t;

class MPL {
  unsigned int times[BUF_LENGTH];
  float alts[BUF_LENGTH];
  float temps[BUF_LENGTH];

  int filePos;
  int lastPos;

  unsigned long start;
  float groundLevel;

  int addr;
public:
  int begin(bool whichWire, uint8_t Addr);

  // Initialization functions
  void setGround();

  // Read data functions
  int readAGL(float &data);
  int readAlt(float &data);

  int readTemp(float &data);

private:
  int getPos(unsigned int time);
  int updateBuffer();
};

#endif
