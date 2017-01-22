
#include <DataCollection.h>


void DataCollection::writeBuffer() {

  if (bufPosition >= BUFFER_SIZE) {
    for (int i = 0; i < BUFFER_SIZE/2; i++) {
      //write buffer element of each data type to sd card at write position.
      writePosition++;
	  Serial.println("single compare");
      Serial.println(altReadings[i]);
	  Serial.println(filteredAltReadings[i]);
	  
    }
    bufPosition = 0;
  }else{
    for (int i = BUFFER_SIZE/2 ; i < BUFFER_SIZE; i++) {
      //write buffer element of each data type to sd card at write position.
      writePosition++;
	  
	  Serial.println("single compare");
      Serial.println(altReadings[i]);
	  Serial.println(filteredAltReadings[i]);
    }
  }
}

void DataCollection::readData(MPL* alt) {
	bufPosition++;

	//gyroReadings[bufPosition] = MPU.readGyro();
	//accelReadings[bufPosition] = MPU.readAccel();
	//magReadings[bufPosition] = MPU.readMag();
	altReadings[bufPosition] = alt->readAltitude();

	if (bufPosition >= BUFFER_SIZE || bufPosition == BUFFER_SIZE/2) 
		writeBuffer();
}

void DataCollection::filterData(MPL* alt) {
	readData(alt);
	kalmanError = predictionError / (predictionError + 0.75);
	filteredAltReadings[bufPosition] = filteredAltReadings[bufPosition - 1] +
		kalmanError * (altReadings[bufPosition] - filteredAltReadings[bufPosition - 1]);
	predictionError = predictionError * (1 - kalmanError);
}


