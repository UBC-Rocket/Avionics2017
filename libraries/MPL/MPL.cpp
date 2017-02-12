#include "MPL.h"
#include <i2c_t3.h>

//#define PWR_MNG 0x6B //register address of pwr management
//#define USR_CNTRL 0x6A //register address of user control


/*
 * 81 - oversample rate of 1
 * 99 - oversample rate of 8
 * A1 - oversample rate of 16
 * B9 - oversample rate of 128
 */
#define REG_INIT 0xA1


MPL::MPL(bool pickWire1) {
  wire = pickWire1;
  error = 0.75;
  predictConstant= 1.2;
  ground_set = false;
  if(wire){
     Wire1.begin();
  }
  else {
    Wire.begin();
  }
  debug("before reset chip");
  init();
}

// Resets the altimeter via software
void MPL::resetChip() {
  writeByte(0x26, 0x04);
  init();
  delay(100);
}

//  s to see that the altimeter is connected and functional
//  and sets up the control registers
boolean MPL::init() {
  debug("in init");
  readByte(0x0C) == 196;

  delay(100);

  writeByte(0x26, REG_INIT);

  delay(1000);

  setGround();
  
  debug("in init after setGround()");

  num_samples_avg = NUM_SAMPLES_AVG;

  delay(1000);
  return true;

}

void MPL::setGround() {
  debug("in setGround");
  ground_level = 0.0;
  int readings = 100; //might want to think about the optimal number for this
  float finding_gnd = 0;

  for(int xxx = 0; xxx < readings; xxx++){
	delay (50);
	ground_level += readAltitude();
    //delay(100);
	debug(xxx);
	debug(ground_level);
  }
  ground_level /= readings;
  ground_set = true;
}

float MPL::getOffset() {
  return ground_level;
}

//  Reads the current altitude in meters
float MPL::readAltitude() {
  //The following will force a read:
  writeByte(0x26, REG_INIT+2); //This forces the chip to make a reading
  writeByte(0x26, REG_INIT); //OST bit set to 1

  int u_altitude, m_altitude;
  float l_altitude;

  uint8_t _buffer[3] = {0};

  readBytes(0x01, 3, _buffer);

  u_altitude = _buffer[0] << 8;//  The upper 8 bits of the altitude
  m_altitude = _buffer[1];//  The middle 8 bits of the altitude
  l_altitude = float(_buffer[2] >> 4) / 16.0;//  The lower 4 bits of the altitude


  int16_t _temp = u_altitude | m_altitude;
  if(ground_set){
    if (_temp < 0) {
      return float(_temp) - l_altitude - ground_level;
    }
    else {
      return float(_temp) + l_altitude - ground_level;
    }
  }

  else {
    if (_temp < 0) {
      return float(_temp) - l_altitude;
    }
    else {
      return float(_temp) + l_altitude;
    }
  }

}

float MPL::readTemp()
{
  int8_t u_temp;
  float l_temp;
  uint8_t _buffer[2];
  readBytes(0x04, 2, _buffer);

  u_temp = _buffer[0];//  Upper 8 bits of the temperature, representing the numbers before the decimal
  l_temp = float(_buffer[1] >> 4) / 16.0;//  Lower 4 bits of the temperature, representing the numbers

  return float(u_temp) + l_temp;
}

//  Writes a byte of data to the sensor at the given address WORKS
byte MPL::writeByte(byte _regAddr, byte _value)
{
  if(!wire) {
    Wire.beginTransmission(ADDRESS);
    Wire.write(_regAddr);
    Wire.write(_value);
    return Wire.endTransmission(true);
  }
  else {
    Wire1.beginTransmission(ADDRESS);
    Wire1.write(_regAddr);
    Wire1.write(_value);
    return Wire1.endTransmission(true);
  }
}

int MPL::readBytes(byte _regAddr, uint8_t _length, uint8_t *_data){
  if(!_data) return -1;

  if(wire) {

    Wire1.beginTransmission(ADDRESS);
    Wire1.write(_regAddr);
    Wire1.endTransmission(false);// or I2C_NOSTOP?
    Wire1.requestFrom(ADDRESS, _length); // Request the data...

    for (int _i = 0; _i < _length; _i++) {
      _data[_i] = Wire1.read();
    }
    return Wire1.endTransmission(true);
  }

  else {
    Wire.beginTransmission(ADDRESS);
    Wire.write(_regAddr);
    Wire.endTransmission(false);// or I2C_NOSTOP?
    Wire.requestFrom(ADDRESS, _length); // Request the data...

    for (int _i = 0; _i < _length; _i++) {
      _data[_i] = Wire.read();
    }

    return Wire.endTransmission(true);
    }
  }

  uint8_t MPL::readByte(uint8_t reg) {
    uint8_t data;
    readBytes(reg, 1, &data);
    return data;
  }

void MPL::debug(String msg){
  Serial.println(msg);
}
