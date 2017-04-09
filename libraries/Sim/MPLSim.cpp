#include "MPL.h"
#include <i2c_t3.h>

#define REG_INIT 0xA1


int MPL::begin(bool whichWire, uint8_t Addr) {
  start = micros();
  lastTimeIndex = 0;
  return 0;
}

/*
Set the ground level to be subtracted off the real altitude measurement
*/
void MPL::setGround() {
  groundLevel = readAlt();
}

/*
Reads the current altitude in meters and return that value minus the gound level offset if it has been set.
*/
int MPL::readAGL(float *data) {
  data = readAlt() - groundLevel;
  return 0;
}

int MPL::readAlt(float *data) {
  data = alts[getTimeIndex()];
  return 0;
}

/*
returns the temperature in degrees Celsius
*/
int MPL::readTemp(float *data) {
  data = temps(getTimeIndex());
  return 0;
}

int MPU::getTimeIndex() {
  if(lastTimeIndes >= SIM_LENGTH) return SIM_LENGTH;

  unsigned long time = micros() - start;

  while(time >= times[lastTimeIndex + 1]) {
    lastTimeIndex++;
  }

  return lastTimeIndex;
}
