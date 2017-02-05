
#include <DataCollection.h>


void DataCollection::writeBuffer() {

  if (bufPosition >= BUFFER_SIZE) {
    for (int i = 0; i < BUFFER_SIZE/2; i++) {
      //write buffer element of each data type to sd card at write position.
      writePosition++;
	  Serial.println("single compare");
      Serial.println(Readings[i]);
	  Serial.println(filteredReadings[i]);

    }
    bufPosition = 0;
  }else{
    for (int i = BUFFER_SIZE/2 ; i < BUFFER_SIZE; i++) {
      //write buffer element of each data type to sd card at write position.
      writePosition++;

	  Serial.println("single compare");
      Serial.println(Readings[i]);
	  Serial.println(filteredReadings[i]);
    }
  }
}

void DataCollection::readData(float* predError, float sensor, float predict, float previous, float read) {
	sensorError = sensor;
  predictConstant = predict;
  previousPredict = previous;
  reading = read;
  if (previousPredict == NULL);
    previousPredict = reading;
  predictionError = predError;
	//gyroReadings[bufPosition] = MPU.readGyro();
	//accelReadings[bufPosition] = MPU.readAccel();
	//magReadings[bufPosition] = MPU.readMag();
	/*Readings[bufPosition] = alt->readAltitude();

	if (bufPosition >= BUFFER_SIZE || bufPosition == BUFFER_SIZE/2)
		writeBuffer();
		*/
}

float DataCollection::filterData() {
	//Predict:
	filteredReading = predictConstant*previousPredict;
  predictionError = predictConstant*predictionError*predictConstant

  //Update:
  Gain = predictionError/(predictionError + sensorError);
  filteredReading = filteredReading + Gain *(reading- filteredReading)
  predictionError =(1-Gain) *predictionError;
  return filteredReading;
}
