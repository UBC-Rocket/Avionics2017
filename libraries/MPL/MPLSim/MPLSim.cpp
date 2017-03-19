#include "MPL.h"
#include <i2c_t3.h>

#define REG_INIT 0xA1


int MPL::begin(bool whichWire) {
  start = micros();
  lastTimeIndex = 0;
  return 0;
}

/*
Set the ground level to be subtracted off the real altitude measurement
*/
void MPL::setGround() {
  groundLevel = alts[0];
}

/*
Reads the current altitude in meters and return that value minus the gound level offset if it has been set.
*/
float MPL::readAGL() {
  return readAlt() - groundLevel;
}

float MPL::readAlt() {
  return alts[getTimeIndex()];
}

/*
returns the temperature in degrees Celsius
*/
float MPL::readTemp() {
  return temps(getTimeIndex());
}

int MPU::getTimeIndex() {
  if(lastTimeIndes >= SIM_LENGTH) return SIM_LENGTH;

  unsigned long time = micros() - start;

  while(time >= times[lastTimeIndex + 1]) {
    lastTimeIndex++;
  }

  return lastTimeIndex;
}
