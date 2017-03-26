#include "MPL.h"
#include <i2c_t3.h>

#define CTRL1_REG 0x26
#define TEMP_REG 0x04
#define ALT_REG 0x01
#define ADDRESS 0x60		// Sensor I2C address

#define NUM_SAMPLES_AVG 100 // Number of samples we want to average
#define WIRE_TIMEOUT 1000

//Do we still need these?
//#define PWR_MNG 0x6B //register address of pwr management
//#define USR_CNTRL 0x6A //register address of user control


/*
 * 81 - oversample rate of 1
 * 99 - oversample rate of 8
 * A1 - oversample rate of 16
 * B9 - oversample rate of 128
 */
#define REG_INIT 0xA1


int MPL::begin(bool whichWire) {
  wire = whichWire;
  groundSet = false;

  if(wire) {
     Wire1.begin();
     Wire1.setDefaultTimeout(WIRE_TIMEOUT);
  }
  else {
    Wire.begin();
    Wire.setDefaultTimeout(WIRE_TIMEOUT);
  }

  write(CTRL1_REG, REG_INIT); //initialize
  num_samples_avg = NUM_SAMPLES_AVG;

  groundLevel = 0;

  return 0;
}

int MPL::selfTest() {
  if(wire) {
    Wire1.setDefaultTimeout(100);

    Wire1.setDefaultTimeout(WIRE_TIMEOUT);

  } else {
    Wire.setDefaultTimeout(100);

    Wire.setDefaultTimeout(WIRE_TIMEOUT);
  }
  return 0; //NOT FLIGHT CODE THIS SHOULD DO SOMETHING!-------------------------------------------------------------------------
}

/*
Set the ground level to be subtracted off the real altitude measurement
*/
void MPL::setGround() {
  debug("in setGround");
  groundLevel = 0.0;
  int readings = 100; //might want to think about the optimal number for this-----------------------------------
 
  delay(500);
  for(int x = 0; x < readings; x++) {
    delay (100);
	float groundGeuss = 0;
    readAlt(&groundGeuss);
	groundLevel += groundGeuss;
    debug(x);
    debug(groundLevel);
  }
  groundLevel /= readings;
}

/*
Reads the current altitude in meters and return that value minus the gound level offset if it has been set.
*/
int MPL::readAGL(float *data) {
  int err = readAlt(data);
  *data -= groundLevel;
  return err;
}

int MPL::readAlt(float *data) {
  int err;
  uint8_t buffer[3];

  if(err = write(CTRL1_REG, REG_INIT+2)) return err; //This forces the chip to make a reading
  if(err = write(CTRL1_REG, REG_INIT)) return err; //OST bit set to 1

  if(err = read(ALT_REG, 3, buffer)) return err;

  *data = (float)((buffer[0]<<8) | buffer[1]); //The high byte of the altitude
  *data += (float)(buffer[2] >> 4) / 16.0; //The fractional component of the altitude

  return 0;
}

/*
returns the temperature in degrees Celsius
*/
int MPL::readTemp(float *data) {
  int err;
  uint8_t buffer[2];

  if(err = read(TEMP_REG, 2, buffer)) return err; //read temperature register

  *data = buffer[0];//  Upper 8 bits of the temperature, representing the numbers before the decimal
  *data += float(buffer[1] >> 4) / 16.0;//  Lower 4 bits of the temperature, representing the numbers

  return 0;
}

int MPL::write(uint8_t reg, uint8_t data) {
  return write(reg, 1, &data);
}

int MPL::write(uint8_t reg, uint8_t length, uint8_t *data) {
  if(!data) return -1;

  if(wire) {
    Wire1.beginTransmission(ADDRESS);
    if(Wire1.write(reg) != 1) return -1;
    if(Wire1.write(data, length) != length) return -2;
    return Wire1.endTransmission(true);
  } else {
    Wire.beginTransmission(ADDRESS);
    if(Wire.write(reg) != 1) return -1;
    if(Wire.write(data, length) != length) return -2;
    return Wire.endTransmission(true);
  }
  return 50; //NOT FLIGHT CODE THIS ERROR CODE SHOULD BE DEFINED!
}

int MPL::read(uint8_t reg, uint8_t length, uint8_t *data) {
  if(!data) return -1;

  if(wire) {
    Wire1.beginTransmission(ADDRESS);//NOT THE RIGHT ADDRESS
    Wire1.write(reg);
    Wire1.endTransmission(false);

    Wire1.requestFrom(ADDRESS, length, false);
    for(int i = 0; i < length; i++) {
      data[i] = Wire1.read();
    }
    return Wire1.endTransmission(true);
  } else {
    Wire.beginTransmission(ADDRESS);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(ADDRESS, length, false);
    for(int i = 0; i < length; i++) {
      data[i] = Wire.read();
    }
    return Wire.endTransmission(true);
  }
}
  /*
  serial prints a given string message
  */
void MPL::debug(String msg){
  Serial.println(msg);
}
