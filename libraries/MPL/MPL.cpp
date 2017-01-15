#include "MPL.h"
#include <i2c_t3.h>

//#define PWR_MNG 0x6B //register address of pwr management
//#define USR_CNTRL 0x6A //register address of user control
#define WHO_AM_I 0x0C
#define CTRL_REG1 0x26
#define OUT_P_MSB 0x01
#define OUT_T_MSB 0x04

#define ADDRESS 0x60		// Sensor I2C address
#define STD_TEMP 20			// Standard temperature at sea level
#define STD_PRESS 101326	// Standard pressure at sea level

#define NUM_SAMPLES_AVG 100 // Number of samples we want to average 


MPL::MPL(bool pickWire1) {
  wire = pickWire1;
  ground_set = false;

  if(wire) {
     Wire1.begin();
  }
  else {
    Wire.begin();
  }
  resetChip();
}

//  Resets the altimeter via software
void MPL::resetChip() {
  writeByte(CTRL_REG1, 0x04);
  init();
}

//  s to see that the altimeter is connected and functional
//  and sets up the control registers
boolean MPL::init() {
  readByte(WHO_AM_I) == 196;

  // CTRL_REG1 (0x26): enable sensor, oversampling x128, altimeter mode
  // writeByte(0x26, 0xBB);
  writeByte(CTRL_REG1, 0x81);
  // CTRL_REG4 (0x29): Data ready interrupt enbabled
  //writeByte(0x29, 0x00);

  // Use these to set custom sea level pressures (Pressure in pascals at sea level / 2)
  //write(0x14, 0xC6);// BAR_IN_MSB (0x14):
  //write(0x15, 0x5B);// BAR_IN_LSB (0x15):

  //setGround(); //why cant you call it from here :(

  num_samples_avg = NUM_SAMPLES_AVG;

  delay(1000);

  return true;

}

void MPL::setGround() {
  ground_level = 0.0;
  int readings = 100; //might want to think about the optimal number for this
  float finding_gnd = 0;

  for(int x=0; x < readings; x++){
	delay (50);
	ground_level += readAltitude();
    //delay(100);
  }
  ground_level /= readings;
  ground_set = true;
}

float MPL::getOffset() {
  return ground_level;
}

//  Reads the current altitude in meters
float MPL::readAltitude() {
  writeByte(CTRL_REG1, 0x83); //This forces the chip to make a reading
  writeByte(CTRL_REG1, 0x81); //OST bit set to 1
  int u_altitude, m_altitude;
  float l_altitude;

  uint8_t buffer[3] = {0};

  readBytes(OUT_P_MSB, 3, buffer);

  u_altitude = buffer[0] << 8;//  The upper 8 bits of the altitude
  m_altitude = buffer[1];//  The middle 8 bits of the altitude
  l_altitude = float(buffer[2] >> 4) / 16.0;//  The lower 4 bits of the altitude


  int16_t temp = u_altitude | m_altitude;
  if(ground_set){
    if (temp < 0) {
      return float(temp) - l_altitude - ground_level;
    }
    else {
      return float(temp) + l_altitude - ground_level;
    }
  }

  else {
    if (temp < 0) {
      return float(temp) - l_altitude;
    }
    else {
      return float(temp) + l_altitude;
    }
  }

}

/*
 * Average your choice of altitude readings
 */
float MPL::readAvgAltitude() {

  int n;
  float avgAltitude = 0;

  for (n = 0; n < num_samples_avg; n++){
	  avgAltitude += readAltitude();
  }

  avgAltitude /= num_samples_avg;

  return avgAltitude;

}

float MPL::readTemp() {
  int8_t utemp;
  float ltemp;
  uint8_t buffer[2];
  readBytes(OUT_T_MSB, 2, buffer);

  utemp = buffer[0];//  Upper 8 bits of the temperature, representing the numbers before the decimal
  ltemp = float(buffer[1] >> 4) / 16.0;//  Lower 4 bits of the temperature, representing the numbers

  return float(utemp) + ltemp;
}

//  Writes a byte of data to the sensor at the given address WORKS
uint8_t MPL::writeByte(uint8_t regAddr, uint8_t value) {
  if(wire) {
    Wire1.beginTransmission(ADDRESS);
    Wire1.write(regAddr);
    Wire1.write(value);
    return Wire1.endTransmission(true);
  }
  else {
    Wire.beginTransmission(ADDRESS);
    Wire.write(regAddr);
    Wire.write(value);
    return Wire.endTransmission(true);
  }
}

int MPL::readBytes(uint8_t regAddr, uint8_t length, uint8_t *data) {
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
  }

  else {
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

uint8_t MPL::readByte(uint8_t reg) {
    uint8_t data;
    readBytes(reg, 1, &data);
    return data;
}
