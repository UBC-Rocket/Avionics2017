#ifndef DataCollection_H
#define DataCollection_H



#include "MPLSim.h"
#include "MPUSim.h"
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

  float curr_alt;
  float curr_accel[3];

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

/*
#ifndef DataCollection_H
#define DataCollection_H



#include "MPL.h"
#include "MPU.h"
#include <i2c_t3.h>
#include <inttypes.h>
#include <stdio.h>


#define BUFFER_SIZE 50

*/

/*
* MPU Definitions
*/
/*
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



typedef short int16_t;

class DataCollection {







	//---------------------------------------------------------------------PRIVATE?



	//Buffer size




	//the posistion in the buffers

	int bufPosition = 0; */
	/*
	* The Buffer for each seonsor type
	*/

/*
	//-----------------------
	//MPU
	//find these by trial and error
	float MPUSensorError = 0.75;
	float MPUSensorPredConst = 1.2;



	//MPU1
	MPU mpu1;


	float previousFilterGyroReadings1[3];
	float previousFilterAccelReadings1[3];
	float previousFilterMagReadings1[3];

	float previousErrorGyroReadings1[3] = { 1, 1, 1 };
	float previousErrorAccelReadings1[3] = { 1, 1, 1 };
	float previousErrorMagReadings1[3] = { 1, 1, 1 };


	int16_t gyroReadingsRAW1[BUFFER_SIZE][3];
	int16_t accelReadingsRAW1[BUFFER_SIZE][3];
	int16_t magReadingsRAW1[BUFFER_SIZE][3];

	int16_t gyroReadingsFILTER1[BUFFER_SIZE][3];
	int16_t accelReadingsFILTER1[BUFFER_SIZE][3];
	int16_t magReadingsFILTER1[BUFFER_SIZE][3];

	//MPU2
	MPU mpu2;


	float previousFilterGyroReadings2[3];
	float previousFilterAccelReadings2[3];
	float previousFilterMagReadings2[3];

	float previousErrorGyroReadings2[3] = { 1, 1, 1 };
	float previousErrorAccelReadings2[3] = { 1, 1, 1 };
	float previousErrorMagReadings2[3] = { 1, 1, 1 };

	int16_t gyroReadingsRAW2[BUFFER_SIZE][3];
	int16_t accelReadingsRAW2[BUFFER_SIZE][3];
	int16_t magReadingsRAW2[BUFFER_SIZE][3];

	int16_t gyroReadingsFILTER2[BUFFER_SIZE][3];
	int16_t accelReadingsFILTER2[BUFFER_SIZE][3];
	int16_t magReadingsFILTER2[BUFFER_SIZE][3];


	//MPU3
	MPU mpu3;
	float previousFilterGyroReadings3[3];
	float previousFilterAccelReadings3[3];
	float previousFilterMagReadings3[3];


	float previousErrorGyroReadings3[3] = { 1, 1, 1 };
	float previousErrorAccelReadings3[3] = { 1, 1, 1 };
	float previousErrorMagReadings3[3] = { 1, 1, 1 };

	int16_t gyroReadingsFILTER3[BUFFER_SIZE][3];
	int16_t accelReadingsFILTER3[BUFFER_SIZE][3];
	int16_t magReadingsFILTER3[BUFFER_SIZE][3];

	int16_t gyroReadingsRAW3[BUFFER_SIZE][3];
	int16_t accelReadingsRAW3[BUFFER_SIZE][3];
	int16_t magReadingsRAW3[BUFFER_SIZE][3];


	//ALT
	//find these by trial and error  the same for all sensors
	float MPLSensorError = 4;
	float MPLSensorPredConst = 1.000;

	//Buffers
	//ATL1

	MPL* PSensor1;

	float previousErrorAltReadings1 = 1;
	float previousFilterAltReadings1 = 0;

	float alt1RAW[BUFFER_SIZE];
	float alt1FILTER[BUFFER_SIZE];

	

	//ATL2

	MPL* PSensor2;

	float previousErrorAltReadings2 = 1;
	float previousFilterAltReadings2 = 0;

	float alt2RAW[BUFFER_SIZE];
	float alt2FILTER[BUFFER_SIZE];










public:








	float timeDelta;
	//these are the filtered values i.e. best guess
	float previousALTITUDE;
	float currentALTITUDE;

	//these are calulated but this object based on the altitude
	float previousAltVelocity;
	float currentAltVelocity;

	//filtered acceleration data i.e. best guess THESE WILL HAVE TO BE FLOATS AS CORRECT TO Gs
	int16_t  currentAcceleration[3];

	//filtered data from the syste	
	int16_t  previousAcceleration[3];






 DataCollection();
	int update();
	int writeToSD();
	int getBufferSize();
	void filterALT1();



private:



};
#endif
*/
