#include "SimData.h"
#include <SD.h>



short SimData::getGyro(unsigned int time) {
  return gyros[getPos(time)];
}

short SimData::getAccel(unsigned int time) {
  return accels[getPos(time)];
}

short SimData::getMag(unsigned int time) {
  return mags[getPos(time)];
}

float SimData::getAlt(unsigned int time) {
  return alts[getPos(time)];
}

float SimData::getTemp(unsigned int time) {
  return temps[getPos(time)];
}

//gets the position in the buffer corresponding to the passed time. Updates buffer if neccesary.
int SimData::getPos(unsigned int time) {
  while(time >= times[lastPos]) {
    if(lastPos == BUF_LENGTH - 1) {
      if(updateBuffer()) return 0;
    }
    lastPos++;
  }

  return lastPos;
}

//updates the buffer with next region of sim data
int SimData::updateBuffer() {
  File simData = SD.open("/test.bin", FILE_READ);
  if(!simData) {
    Serial.println("Failed to open SD card file.");
    return 2;
  }
  Serial.print("SD File opened: ");
  Serial.println(simData.name());

  unsigned int *lastTime;
  short *lastGyro, *lastAccel, *lastMag;
  float *lastAlt, *lastTemp;

  if(simData.seek(filePos)) {
    char data[4];

    for(int bufPos = 0; (bufPos < BUF_LENGTH) && simData.available() >= 18; bufPos++) {
      simData.read(data, 4);
      lastTime = (unsigned int *)data;
      times[bufPos] = *lastTime;

      simData.read(data, 2);
      lastGyro = (short *)data;
      gyros[bufPos] = *lastGyro;

      simData.read(data, 2);
      lastAccel = (short *)data;
      accels[bufPos] = *lastAccel;

      simData.read(data, 2);
      lastMag = (short *)data;
      mags[bufPos] = *lastMag;

      simData.read(data, 4);
      lastAlt = (float *)data;
      alts[bufPos] = *lastAlt;

      simData.read(data, 4);
      lastTemp = (float *)data;
      temps[bufPos] = *lastTemp;
    }
  } else return 1;

  filePos = simData.position();

  simData.close();

  lastPos = 0;
  return 0;
}
