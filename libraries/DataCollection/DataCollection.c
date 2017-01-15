#include "MPL.h"
#include "MPU.h"
#include <i2c_t3.h>

#define BUFFER_SIZE

MPL alt = new MPL(0);
MPU pos = new MPU(0);
int16_t gyroReadings[][] = int16_t[BUFFER_SIZE][3];
int16_t accelReadings[][] = int16_t[BUFFER_SIZE][3];
int16_t magReadings[][] = int16_t[BUFFER_SIZE][3];
float altReadings[] = float[BUFFER_SIZE];

float filteredAltReadings = float[BUFFER_SIZE];
int kalmanError;
int predictionError = 1;

int bufPosition = 0;
int writePosition = 0;

void readData() {
  bufPosition++;

  gyroReadings[bufPosition] = MPU.readGyro();
  accelReadings[bufPosition] = MPU.readAccel();
  magReadings[bufPosition] = MPU.readMag();
  altReadings[bufPosition] = MPL.readAltitude();

  if(bufPosition >= BUFFER_SIZE) writeBuffer();
}

void filterData() {
  readData();
  kalmanError = predictionError/(predictionError + 0.5);
  filteredAltReadings[bufPosition] = filteredAltReadings[bufPosition - 1] +
    kalmanError * (altReadings[bufPosition] - filteredAltReadings[bufPosition - 1]);
  predictionError = predictionError * (1 - kalmanError);

  filteredAltReadings[bufPosition] =
}

void writeBuffer() {
  for(int i = 0; i < BUFFER_SIZE; i++) {
    //write buffer element of each data type to sd card at write position.
    writePosition++;
  }
  bufPosition = 0;
}
