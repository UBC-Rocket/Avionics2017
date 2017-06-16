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
#define AVG_SIZE 5

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
  float prev_alts[AVG_SIZE];
  float curr_Acc[3];
  float prev_z_accel[AVG_SIZE]; //ONLY CONTAINS Z AXIS ACCEL
  float prev_total_accel[AVG_SIZE]; //CONTAINS SQ/SQRT OF X Y Z

  int bufPosition;
  float gyroReadings[BUFFER_SIZE][3];
  float accelReadings[BUFFER_SIZE][3];
  float magReadings[BUFFER_SIZE][3];
  int states[BUFFER_SIZE*2];
  float altReadings[BUFFER_SIZE];
  unsigned long time[BUFFER_SIZE];

public:

  int begin(MPU *mpu[], int MPUlen, MPL *mpl[], int MPLlen);
  int filterData();
  int saveRocketState(int state);
  int popGyro(float gyro[]);
  int popAccel(float accel[]);
  int updatePrevZAccel();
  int avgPrevZAccel(float &avgZAccel, float &prevAvgZAccel);
  int getTotalAccel(float total_accel);
  int updatePrevTotalAccel();
  int avgPrevTotalAccel(float &avgTotalAccel, float &prevAvgTotalAccel);
  int popMag(float mag[]);
  int popAlt(float &alt);
  int updatePrevAlts();
  int avgPrevAlts(float &avgAlt, float &prevAvgAlt);

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