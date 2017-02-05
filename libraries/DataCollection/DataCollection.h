#ifndef DataCollection_H
#define DataCollection_H



//#include "MPL.h"
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

  float reading;
	float filteredReading;
	float Gain;
	float* predictionError;
  float previousPredict;
  float sensorError;
  float predictConstant;

	void readData(float* predError, float sensor, float predict, float previous, float read);
	void filterData();

};
#endif
