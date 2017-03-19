#include "MPL.h"
#include <i2c_t3.h>

#define CTRL1_REG 0x26
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

  writeByte(CTRL1_REG, REG_INIT); //initialize
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
}

/*
Set the ground level to be subtracted off the real altitude measurement
*/
void MPL::setGround() {
  debug("in setGround");
  groundLevel = 0.0;
  int readings = 100; //might want to think about the optimal number for this
  float finding_gnd = 0;

  for(int x = 0; x < readings; x++) {
    delay (50);
    groundLevel += readAlt();
    debug(x);
    debug(groundLevel);
  }
  groundLevel /= readings;
}

/*
Reads the current altitude in meters and return that value minus the gound level offset if it has been set.
*/
float MPL::readAGL() {
  return readAlt() - groundLevel;
}

float MPL::readAlt() {
  //The following will force a read:
  writeByte(CTRL1_REG, REG_INIT+2); //This forces the chip to make a reading
  writeByte(CTRL1_REG, REG_INIT); //OST bit set to 1

  uint8_t altH, altL;
  float altF;

  uint8_t buffer[3] = {0};

  readBytes(0x01, 3, buffer);

  altH = buffer[0]; //The high byte of the altitude
  altL = buffer[1];//  The low byte of the altitude
  altF = float(buffer[2] >> 4) / 16.0; //The fractional component of the altitude

  return ((altH << 8) | altL) + altF;
}

/*
returns the temperature in degrees Celsius
*/
float MPL::readTemp() {
  int8_t tempH;
  float tempF;
  uint8_t buffer[2];
  readBytes(0x04, 2, buffer); //read temperature register

  tempH = buffer[0];//  Upper 8 bits of the temperature, representing the numbers before the decimal
  tempF = float(buffer[1] >> 4) / 16.0;//  Lower 4 bits of the temperature, representing the numbers

  return tempH + tempF;
}

//  Writes a byte of data to the sensor at the given address WORKS
uint8_t MPL::writeByte(byte regAddr, byte value) {
  if(!wire) {
    Wire.beginTransmission(ADDRESS);
    Wire.write(regAddr);//load target reg
    Wire.write(value); //load value into target reg

    return Wire.endTransmission(true);
  } else {
    Wire1.beginTransmission(ADDRESS);
    Wire1.write(regAddr);
    Wire1.write(value);

    return Wire1.endTransmission(true);
  }
}

//  read "length" bytes starting at a given address and loads the values into the given array pointer
int MPL::readBytes(byte regAddr, uint8_t length, uint8_t *data){
  if(!data) return -1;

  if(wire) {
    Wire1.beginTransmission(ADDRESS);
    Wire1.write(regAddr);
    Wire1.endTransmission(false);// or I2C_NOSTOP?
    Wire1.requestFrom(ADDRESS, length); // Request the data...

    for (int i = 0; i < length; i++) {
      data[i] = Wire1.read();
    }

    return Wire1.endTransmission(true);
  } else {
    Wire.beginTransmission(ADDRESS);
    Wire.write(regAddr);
    Wire.endTransmission(false);// or I2C_NOSTOP?
    Wire.requestFrom(ADDRESS, length); // Request the data...

    for (int i = 0; i < length; i++) {
      data[i] = Wire.read();
    }

    return Wire.endTransmission(true);
  }
}

//read ONE byte from the given reg and returns the unsigned value
  uint8_t MPL::readByte(uint8_t reg) {
    uint8_t data;
    readBytes(reg, 1, &data);
    return data;
  }
  /*
  serial prints a given string message
  */
  void MPL::debug(String msg){
    Serial.println(msg);
  }
