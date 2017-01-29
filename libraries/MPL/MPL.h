#ifndef MPL_H
#define MPL_H

#include <inttypes.h>
#include <Arduino.h>

typedef short int16_t;

#define byte uint8_t
#define ADDRESS 0x60		// Sensor I2C address
#define STD_TEMP 20			// Standard temperature at sea level
#define STD_PRESS 101326	// Standard pressure at sea level

#define NUM_SAMPLES_AVG 100 // Number of samples we want to average
#define ERROR 0.75
#define PREDICT_CONSTANT 1.2
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

  float readTemp();
  uint8_t readByte(uint8_t reg);

  int initAtl(uint16_t fullScale);
  int initTemp(uint8_t fullScale);

private:
  void debug(String msg);
  // Writes a given byte to a given register
  byte writeByte(byte _regAddr, byte _value);

  int readBytes(uint8_t reg, uint8_t length, uint8_t* data);
};

#endif
