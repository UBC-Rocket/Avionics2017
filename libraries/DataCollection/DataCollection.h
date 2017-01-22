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
  
  
  
  
public:
 	int16_t gyroReadings[BUFFER_SIZE][3];
	int16_t accelReadings[BUFFER_SIZE][3];
	int16_t magReadings[BUFFER_SIZE][3];
	float altReadings[BUFFER_SIZE];


	float filteredAltReadings[BUFFER_SIZE];
	float kalmanError;
	float predictionError = 1;

	int bufPosition = 1;
	int writePosition = 0;

	void readData(MPL* alt);
	void filterData(MPL* alt);
	void writeBuffer();
 


};
#endif 






