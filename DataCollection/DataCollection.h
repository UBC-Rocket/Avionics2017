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
  MPU mpu();
  MPL mpl();

  int bufPosition;
  int16_t gyroReadings[BUFFER_SIZE][3];
	int16_t accelReadings[BUFFER_SIZE][3];
	int16_t magReadings[BUFFER_SIZE][3];
	float altReadings[BUFFER_SIZE];
  unsigned long time[BUFFER_SIZE];
  bool bufferLock;

public:
	float kalmanError;
	float predictionError = 1;

  int collectAndFilter();
  int popGyro(int16_t* gyro);
  int popAccel(int16_t* accel);
  int popMag(int16_t* mag);
  float popAlt();

	int filterData();
	int collect();
  int writeData();

private:
  int loadBuffer3(int16_t data[], int16_t buf[][3]);
};
#endif
