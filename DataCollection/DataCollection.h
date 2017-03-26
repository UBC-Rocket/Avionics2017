#ifndef DataCollection_H
#define DataCollection_H



#include "MPL.h"
//#include "MPU.h"
//#include <i2c_t3.h>
#include <inttypes.h>
#include <stdio.h>

#define BUFFER_SIZE 500

//MPL alt = new MPL(0);
//MPU pos = new MPU(0);

typedef short int16_t;

class DataCollection {
  MPU mpus[];
  int mpuError[];
  MPL mpls[];
  int mplError[];
  int MPULength, MPULength;

  int bufPosition;
  float gyroReadings[BUFFER_SIZE][3];
	float accelReadings[BUFFER_SIZE][3];
	float magReadings[BUFFER_SIZE][3];
	float altReadings[BUFFER_SIZE];
  unsigned long time[BUFFER_SIZE];
  bool bufferLock;

public:

  int begin(MPU [], MPL mpl[]);
  int filterData();
  int popGyro(float gyro[]);
  int popAccel(float accel[]);
  int popMag(float mag[]);
  int popAlt(float *alt);

	int filterData();
	int collect();
  int writeData();

private:
  void readBuffer3(int16_t *buf[][3], int16_t *data[]);
  void loadBuffer3(int16_t data[], int16_t buf[][3], int pos);

  void average3(float data[][3], int length, float avg[])
  float average(float data[], int length);
};
#endif
