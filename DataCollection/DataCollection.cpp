#include "DataCollection.h"

int DataCollection::begin(MPU mpu[], MPL mpl[]) {
  mpus = mpu;
  mpls = mpl;
}

int DataCollection::filterData();

int DataCollection::popGyro(float gyro[]) {
  if(bufferPosition < 1) return -1;
  readBuffer3(gyroReadings, gyro);
  return 0;
}

/**
 * Get the most recent accelerometer reading
 * \param accel A 3 element array to be filled with the x y and z components of accelerometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popAccel(float accel[]) {
  if(bufferPosition < 1) return -1;
  readBuffer3(accelReadings, accel);
  return 0;
}

/**
 * Get most recent magnetometer reading
 * \param mag A 3 element array to be filled with the x y and z components of magnetometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popMag(float mag[]) {
  if(bufferPosition < 1) return -1;
  readBuffer3(magReadings, mag);
  return 0;
}

/**
 * Get most recent altitude reading.
 * \param alt a float to be filled with last altitude
 * \return Returns most recent altitude reading or -1 if the buffer is empty
 */

int DataCollection::popAlt(float *alt) {
  if(bufferPosition < 1) return -1;
  return altReadings[bufferPosition - 1];
}

/**
 * Collect a timestep of data and write to buffer.
 * Collect expects all sensors to be properly initialized.
 * \return Returns amount of space left in buffer or -1 if buffer in use.
 */

int DataCollection::collect() {
  if(bufferPosition > BUFFER_SIZE) return 0;
  if(bufferLock) return -1;
  bufferLock = 1;

  time[bufferPosition] = micros();
  int droppedReadings; //number of sensor readings ignored due to communication issues

  //Buffers for one timestep of readings from all sensors.
  float gyros[MPULength][3];
  float accels[MPULength][3];
  float mags[MPULength][3];
  float alts[MPLLength];

  dropperReadings = 0;
  for(int x = 0; x < MPULength; x++) {
    if(mpuError[x]) {
      droppedReadings++;
      continue;
    }

    MPU mpu = mpus[x];

    float tmp[3];
    mpuError[x] = mpuError[x] ? mpuError[x] : mpu.readGyro(tmp);
    loadBuffer3(tmp, gyro, x-droppedReadings);

    mpuError[x] = mpuError[x] ? mpuError[x] : mpu.readAccel(tmp);
    loadBuffer3(tmp, accel, x-droppedReadings);

    mpuError[x] = mpuError[x] ? mpuError[x] : mpu.readMag(tmp);
    loadBuffer3(tmp, mag, x-droppedReadings);
  }

  float tmp[3];

  //average each set of data from MPU and load in to buffer
  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, gyroReadings, bufferPosition);

  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, accelReadings, bufferPosition);

  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, magReadings, bufferPosition);

  droppedReadings = 0;
  for(int x = 0; x < MPLLength; x++) {
    if(mplError[x]) {
      droppedReadings++;
      continue;
    }

    MPL mpl = mpls[x];

    float alt;
    mplError[x] = mplError[x] ? mplError[x] : mpl.readAGL(&alt);
    alts[x - droppedReadings] = alt;
  }

  alt[bufferPosition] = average(alts, MPLLength - droppedReadings);

  bufferPosition++;
  bufferLock = 0;
  return BUFFER_SIZE - bufferPosition;
}

/* writeData: writes contents of buffer to SD card.
 * returns: 0 on success, -1 if the buffer is locked, or SD card error code.
 */

int DataCollection::writeData() {
  if (bufferLock) return -1;
  bufferLock = 1;
  int16_t lastGyro[3];
  int16_t lastAccel[3];
  int16_t lastMag[3];
  float lastAlt;
  unsigned int lastTime;

  readBuffer3(gyroReadings, lastGyro);
  readBuffer3(accelReadings, lastAccel);
  readBuffer3(magReadings, lastMag);
  lastAlt = altReadings[bufferPosition - 1];
  lastTime = time[bufferPosition - 1];

  //write data to SD card here

  bufferPosition = 0;
  loadBuffer3(lastGyro, gyroReadings, bufferPosition);
  loadBuffer3(lastAccel, accelReadings, bufferPosition);
  loadBuffer3(lastMag, magReadings, bufferPosition);
  altReadings[bufferPosition] = lastAlt;
  time[bufferPosition] = lastTime;

  bufferPosition++;
  bufferLock = 0;
  return 0;
}

void DataCollection::readBuffer3(int16_t *buf[][3], int16_t *data[]) {
  data[0] = buf[bufferPosition - 1][0];
  data[1] = buf[bufferPosition - 1][1];
  data[2] = buf[bufferPosition - 1][2];
}

void DataCollection::loadBuffer3(int16_t *data[], int16_t *buf[][3], int pos) {
  buf[pos][0] = data[0];
  buf[pos][1] = data[1];
  buf[pos][2] = data[2];
}

void DataCollection::average3(float data[][3], int length, float avg[]) {
  for(int x = 0; x < length; x++) {
    avg[0] += data[x][0];
    avg[1] += data[x][1];
    avg[2] += data[x][2];
  }

  avg[0] /= length;
  avg[1] /= length;
  avg[2] /= length;
}

float DataCollection::average(float data[], int length) {
  float avg;
  for(int x = 0; x < length; x++) {
    avg += data[x];
  }

  return avg/length;
}

int DataCollection::verifySensor(MPL mpl) {
  mpl.
}

int DataCollection::verifySensor(MPU mpu) {

}
