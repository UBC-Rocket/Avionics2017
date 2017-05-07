#ifndef DataCollection_H
#define DataCollection_H


#include "MPL.h"
#include "MPU.h"


#include <inttypes.h>
#include <stdio.h>
#include <Arduino.h>
#include <SD.h>

#define BUFFER_SIZE 20
#define NSENSORS 3

//MPL alt = new MPL(0);
//MPU pos = new MPU(0);

typedef short int16_t;

class DataCollection {
  MPU *mpus[NSENSORS];
  int mpuError[NSENSORS];
  MPL *mpls[NSENSORS];
  int mplError[NSENSORS];
  int MPULength, MPLLength;
  
  float curr_altitude;
  int16_t curr_Acc[3];

  int bufPosition;
  float gyroReadings[BUFFER_SIZE][3];
  float accelReadings[BUFFER_SIZE][3];
  float magReadings[BUFFER_SIZE][3];
  float altReadings[BUFFER_SIZE];
  unsigned long time[BUFFER_SIZE];

public:

  int begin(MPU *mpu[], int MPUlen, MPL *mpl[], int MPLlen);
  int filterData();
  int popGyro(float gyro[]);
  int popAccel(float accel[]);
  float getTotalAccel(float accel[]);
  int popMag(float mag[]);
  int popAlt(float &alt);

  int collect();
  int writeData();

private:
  void readBuffer3(float buf[][3], float data[], int pos);
  void loadBuffer3(float data[], float buf[][3], int pos);

  void average3(float data[][3], int length, float avg[]);
  float average(float data[], int length);

  void debug(String msg);
};
#endif