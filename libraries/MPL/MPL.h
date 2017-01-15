#ifndef MPL_H
#define MPL_H

#include <inttypes.h>

typedef short int16_t;

class MPL {

  int num_samples_avg;
  bool wire;
  bool ground_set;
  float ground_level;

public:

  MPL(bool pickWire1);

  // Sensor configuration functions
  void resetChip();

  // Initialization functions
  bool init();
  void setGround();
  float getOffset();

  // Check for data functions
  bool getDataReady();

  // Read data functions
  float readAltitude();
  float readAvgAltitude();
  float readTemp();
  uint8_t readByte(uint8_t reg);

  int initAtl(uint16_t fullScale);
  int initTemp(uint8_t fullScale);

private:
  // Writes a given byte to a given register
  byte writeByte(byte _regAddr, byte _value);

  int readBytes(uint8_t reg, uint8_t length, uint8_t* data);
};

#endif
