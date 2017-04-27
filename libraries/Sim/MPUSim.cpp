#include "MPUSim.h"
#include "SD.h"

int MPU::begin(bool whichWire, uint8_t Addr) {
  start = micros();
  addr = Addr;
  return 0;
}

int MPU::selfTest() {
  return addr;
}

int MPU::initGyro(uint16_t fullScale) {
  uint8_t regValue;

  switch (fullScale) {
    case 250: regValue = 0b00; break;
    case 500: regValue = 0b01; break;
    case 1000: regValue = 0b10; break;
    case 2000: regValue = 0b11; break;
    default: return -1; break;
  }

  gyroFS = (float)fullScale;
  return 0;
}

int MPU::initAccel(uint8_t fullScale) {
  uint8_t regValue;

  switch (fullScale) {
    case 2: regValue = 0b00; break;
    case 4: regValue = 0b01; break;
    case 8: regValue = 0b10; break;
    case 16: regValue = 0b11; break;
    default: return -1; break;
  }

  accelFS = (float)fullScale;
  return 0;
}

int MPU::initMag() {
  return 0;
}

int MPU::readGyro(int16_t *data) {
  int gyro = gyros[getPos(micros() - start)];
  gyro = gyro * 2000.0 / gyroFS;

  data[0] = 0;
  data[1] = 0;
  data[2] = gyro;
  return 0;
}

int MPU::readAccel(int16_t *data) {
  int accel = accels[getPos(micros() - start)];
  //accel = accel * 16.0 / accelFS;

  data[0] = 0;
  data[1] = 0;
  data[2] = accel;
  return 0;
}

int MPU::readMag(int16_t* data) {
  int mag = mags[getPos(micros() - start)];

  data[0] = 0;
  data[1] = 0;
  data[2] = mag;
  return 0;
}

int MPU::readGyro(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readGyro(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = rawData[x] * gyroFS / INT16_MAX;
  }
  return 0;
}

int MPU::readAccel(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readAccel(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = rawData[x] * accelFS / INT16_MAX;
  }
  return 0;
}

int MPU::readMag(float data[]) {
  int16_t rawData[3];
  int err;
  if(err = readMag(rawData)) return err;

  for (int x = 0; x < 3; x++) {
    data[x] = (float)rawData[x];
  }
  return 0;
}

//gets the position in the buffer corresponding to the passed time. Updates buffer if neccesary.
int MPU::getPos(unsigned int time) {
  while(time >= times[lastPos]) {
    if(lastPos == BUF_LENGTH - 1) {
      if(updateBuffer()) return 0;
    }
    lastPos++;
  }

  return lastPos;
}

//updates the buffer with next region of sim data
int MPU::updateBuffer() {
  File simData;

  //define test file names here
  switch(addr) {
    case 0x68: simData = SD.open("/testMPU.bin", FILE_READ); break;
    default: simData = SD.open("/testMPU.bin", FILE_READ); break;
  }

  if(!simData) {
    Serial.println("Failed to open SD card file.");
    return 2;
  }
  Serial.print("SD File opened: ");
  Serial.println(simData.name());

  unsigned int *lastTime;
  short *lastGyro, *lastAccel, *lastMag;

  if(simData.seek(filePos)) {
    char data[4];

    for(int bufPos = 0; (bufPos < BUF_LENGTH) && simData.available() >= 10; bufPos++) {
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
    }
  } else return 1;

  filePos = simData.position();

  simData.close();

  lastPos = 0;
  return 0;
}
