#ifndef DataCollection_H
#define DataCollection_H



#include "MPL.h"
#include "MPU.h"
#include <inttypes.h>
#include <stdio.h>
#include <Arduino.h>

#define BUFFER_SIZE 500

//MPL alt = new MPL(0);
//MPU pos = new MPU(0);

typedef short int16_t;

class DataCollection {
  MPU *mpus[5];
  int mpuError[5];
  MPL *mpls[5];
  int mplError[5];
  int MPULength, MPLLength;

  int bufPosition;
  float gyroReadings[BUFFER_SIZE][3];
	float accelReadings[BUFFER_SIZE][3];
	float magReadings[BUFFER_SIZE][3];
	float altReadings[BUFFER_SIZE];
  unsigned long time[BUFFER_SIZE];
  bool bufferLock;

public:

  int begin(MPU *mpu[], int MPUlen, MPL *mpl[], int MPLlen);
  int filterData();
  int popGyro(float gyro[]);
  int popAccel(float accel[]);
  int popMag(float mag[]);
  int popAlt(float *alt);

	int collect();
  int writeData();

private:
  void readBuffer3(float buf[][3], float data[]);
  void loadBuffer3(float data[], float buf[][3], int pos);

  void average3(float data[][3], int length, float avg[]);
  float average(float data[], int length);

  void debug(String msg);
};
#endif
