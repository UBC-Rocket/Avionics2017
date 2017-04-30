#include "MPLSim.h"
#include "SD.h"

#define REG_INIT 0xA1

int MPL::begin(bool whichWire, uint8_t Addr) {
  start = micros();
  addr = Addr;
  return 0;
}

/*
Set the ground level to be subtracted off the real altitude measurement
*/
void MPL::setGround() {
   readAlt(groundLevel);
}

/*
Reads the current altitude in meters and return that value minus the gound level offset if it has been set.
*/
int MPL::readAGL(float &data) {
  readAlt(data);
  data -= groundLevel;
  return 0;
}

int MPL::readAlt(float &data) {
  data = alts[getPos(micros() - start)];
  return 0;
}

/*
returns the temperature in degrees Celsius
*/
int MPL::readTemp(float &data) {
  data = temps[getPos(micros() - start)];
  return 0;
}

//gets the position in the buffer corresponding to the passed time. Updates buffer if neccesary.
int MPL::getPos(unsigned int time) {
  while(time >= times[lastPos]) {
    if(lastPos == BUF_LENGTH - 1) {
      if(updateBuffer()) return 0;
    }
    lastPos++;
  }

  return lastPos;
}

//updates the buffer with next region of sim data
int MPL::updateBuffer() {
  File simData;

  //define test file names here
  switch(addr) {
	//add more files here for more sensors
    case 0x60: simData = SD.open("/testMPL.bin", FILE_READ); break;
    default: simData = SD.open("/testMPL.bin", FILE_READ); break;
  }

  if(!simData) {
    Serial.println("Failed to open SD card file.");
    return 2;
  }
  Serial.print("SD File opened: ");
  Serial.println(simData.name());

  unsigned int *lastTime;
  float *lastAlt, *lastTemp;

  if(simData.seek(filePos)) {
    char data[4];

    for(int bufPos = 0; (bufPos < BUF_LENGTH) && simData.available() >= 12; bufPos++) {
      simData.read(data, 4);
      lastTime = (unsigned int *)data;
      times[bufPos] = *lastTime;

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
