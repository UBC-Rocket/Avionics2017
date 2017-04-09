#ifndef MPL_H
#define MPL_H

#include <inttypes.h>
#include <Arduino.h>

typedef short int16_t;

#define STD_TEMP 20			// Standard temperature at sea level
#define STD_PRESS 101326	// Standard pressure at sea level

class MPL {
  int num_samples_avg;
  uint8_t addr;
  bool wire;
  bool groundSet;
  float groundLevel;

public:
  int begin(bool whichWire, uint8_t Addr);

  // Initialization functions
  void setGround();

  // Read data functions
  int readAGL(float *data);
  int readAlt(float *data);

  int readTemp(float *data);
private:
  void debug(String msg);

  // Writes a given byte to a given register
  int write(uint8_t reg, uint8_t data);
  int write(uint8_t reg, uint8_t length, uint8_t *data);
  int read(uint8_t reg, uint8_t length, uint8_t *data);
};

#endif
