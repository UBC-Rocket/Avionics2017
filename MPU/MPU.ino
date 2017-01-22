#include <i2c_t3.h>
#include <TimerOne.h>
#include "MPU.h"


#define ADDR 0x68       //i2c address of sensor
#define PWR_MNG 0x6B    //register address of pwr management
#define USR_CNTRL 0x6A  //register address of user control
#define GYRO_SETUP 0x1B //register address of gryoscope setup
#define ACCEL_SETUP 0x1C

#define GYRO_BASE 0x43
#define ACCEL_BASE 0x3B

#define OFFSET_X_HIGH 0x0
#define OFFSET_X_LOW 0x1

#define OFFSET_Y_HIGH 0x2
#define OFFSET_Y_LOW 0x3

#define OFFSET_Z_HIGH 0x4
#define OFFSET_Z_LOW 0x5

int16_t gyro[3];
int16_t linx, liny, linz;
float cleanGyro[3];
unsigned long elapsed = 0;
MPU* test;
long quat[4];


void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("hi");
  test = new MPU(0, ADDR);
  Serial.print("Loading DMP ");
  Serial.println(test->loadDMP());

  Serial.print("Enabling DMP ");
  Serial.println(test->enableDMP(1));
}



void loop() {
  
  elapsed = micros() - elapsed;
  Serial.println("Running: ");
  Serial.print("Reading DMP ");
//  Serial.println(test->readDMP(&quat));
  Serial.println("Quat data: ");
  
  Serial.println(quat[0]);
  Serial.println(quat[1]);
  Serial.println(quat[2]);
  Serial.println(quat[3]);
  
  delay(5000);


//  x_pos = (x_pos > 360) ? (x_pos - 360) : x_pos;
//  y_pos = (y_pos > 360) ? (y_pos - 360) : y_pos;
//  z_pos = (z_pos > 360) ? (z_pos - 360) : z_pos;
//
//  x_pos = (x_pos < 360) ? (x_pos + 360) : x_pos;
//  y_pos = (y_pos < 360) ? (y_pos + 360) : y_pos;
//  z_pos = (z_pos < 360) ? (z_pos + 360) : z_pos;

}
