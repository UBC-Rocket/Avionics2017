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
MPU test;
long quat[4];
double angles[3];


void setup() {
  delay(500);
  Serial.begin(9600);
  delay(2000);
  Serial.println("hi");
  test.begin(1, ADDR);
  Serial.println("Loading DMP ");
  Serial.println(test.loadDMP());

  Serial.println("Enabling DMP ");
  Serial.println(test.enableDMP(true));
}



void loop() {
  
  elapsed = micros() - elapsed;
  Serial.println("Running: ");
  Serial.println("Reading DMP ");
  Serial.println(test.readDMP(quat));
  Serial.println("Quat data: ");
  
  Serial.println(quat[0]);
  Serial.println(quat[1]);
  Serial.println(quat[2]);
  Serial.println(quat[3]);

  Serial.println("Angle Data: ");
  parseQuat(quat, angles);

  Serial.println(angles[0]);
  Serial.println(angles[1]);
  Serial.println(angles[2]);
  delay(10);


//  x_pos = (x_pos > 360) ? (x_pos - 360) : x_pos;
//  y_pos = (y_pos > 360) ? (y_pos - 360) : y_pos;
//  z_pos = (z_pos > 360) ? (z_pos - 360) : z_pos;
//
//  x_pos = (x_pos < 360) ? (x_pos + 360) : x_pos;
//  y_pos = (y_pos < 360) ? (y_pos + 360) : y_pos;
//  z_pos = (z_pos < 360) ? (z_pos + 360) : z_pos;

}


//quaternion to euler angles from the internet
void parseQuat(long quat[], double angles[]) {
  double sqw = quat[0] * quat[0];
  double sqx = quat[1] * quat[1];
  double sqy = quat[2] * quat[2];
  double sqz = quat[3] * quat[3];
  double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
  double test = quat[1] * quat[2] + quat[3] * quat[0];
  
  if (test > 0.499*unit) { // singularity at north pole
    angles[0] = 2 * atan2(quat[1],quat[0]);
    angles[1] = PI/2;
    angles[2] = 0;
    return;
  }
  if (test < -0.499*unit) { // singularity at south pole
    angles[0] = -2 * atan2(quat[1],quat[0]);
    angles[1] = -PI/2;
    angles[2] = 0;
    return;
  }
    angles[0] = atan2(2*quat[2]*quat[0]-2*quat[1]*quat[3] , sqx - sqy - sqz + sqw);
    angles[1] = asin(2*test/unit);
    angles[2] = atan2(2*quat[1]*quat[0]-2*quat[2]*quat[3] , -sqx + sqy - sqz + sqw);
}
