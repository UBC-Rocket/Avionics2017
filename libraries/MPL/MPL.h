#ifndef MPL_H
#define MPL_H

#include <inttypes.h>
#include <Arduino.h>

typedef short int16_t;

#define STD_TEMP 20			// Standard temperature at sea level
#define STD_PRESS 101326	// Standard pressure at sea level

class MPL {
  int num_samples_avg;
  bool wire;
  float groundLevel;

public:
  int begin(bool whichWire);

  // Initialization functions
  void setGround();

  // Read data functions
  float readAGL();
  float readAlt();

  float readTemp();
private:
  void debug(String msg);

  // Writes a given byte to a given register
  uint8_t writeByte(byte regAddr, byte value);
  int readBytes(uint8_t reg, uint8_t length, uint8_t* data);
  uint8_t readByte(uint8_t reg);
};

#endif
