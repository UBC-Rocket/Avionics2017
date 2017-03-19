#include DataCollection.h

int DataCollection::filterData();

int DataCollection::popGyro(int16_t* gyro) {
  if(bufferPosition < 1) return -1;
  readBuffer3(gyroReadings, gyro);
  return 0;
}

/**
 * Get the most recent accelerometer reading
 * \param accel A 3 element array to be filled with the x y and z components of accelerometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popAccel(int16_t* accel) {
  if(bufferPosition < 1) return -1;
  readBuffer3(accelReadings, accel);
  return 0;
}

/**
 * Get most recent magnetometer reading
 * \param mag A 3 element array to be filled with the x y and z components of magnetometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popMag(int16_t* mag) {
  if(bufferPosition < 1) return -1;
  readBuffer3(magReadings, mag);
  return 0;
}

/**
 * Get most recent altitude reading.
 * \return Returns most recent altitude reading or -1 if the buffer is empty
 */

float DataCollection::popAlt() {
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

  int16_t tmp[3];
  mpu.readGyro(tmp);
  loadBuffer3(tmp, gyroReadings);

  mpu.readAccel(tmp);
  loadBuffer3(tmp, accelReadings);

  mpu.readMag(tmp);
  loadBuffer3(tmp, magReadings);

  alt[bufferPosition] = mpl.getAGL();

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
  loadBuffer3(lastGyro, gyroReadings);
  loadBuffer3(lastAccel, accelReadings);
  loadBuffer3(lastMag, magReadings);
  altReadings[bufferPosition] = lastAlt;
  time[bufferPosition] = lastTime;

  bufferPosition++;
  bufferLock = 0;
  return 0;
}

int DataCollection::readBuffer3(int16_t* buf[][3], int16_t* data[]) {
  data[0] = buf[bufferPosition - 1][0];
  data[1] = buf[bufferPosition - 1][1];
  data[2] = buf[bufferPosition - 1][2];
}

int DataCollection::loadBuffer3(int16_t* data[], int16_t* buf[][3]) {
  buf[bufferPosition][0] = data[0];
  buf[bufferPosition][1] = data[1];
  buf[bufferPosition][2] = data[2];
}
