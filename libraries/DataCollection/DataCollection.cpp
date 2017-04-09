#include "DataCollection.h"

#define ACCEL_FS 16
#define GYRO_FS 2000

int DataCollection::begin(MPU *mpu[], int MPUlen, MPL *mpl[], int MPLlen) {
  bufPosition = 0;

  MPULength = MPUlen;
  MPLLength = MPLlen;

  for(int i = 0; i < MPULength; i++) {
    mpus[i] = mpu[i];
    mpuError[i] = 0;

    mpus[i]->initAccel(ACCEL_FS);
    mpus[i]->initGyro(GYRO_FS);
    mpus[i]->initMag();
  }

  for(int i = 0; i < MPLLength; i++) {
    mpls[i] = mpl[i];
    mplError[i] = 0;
  }

  return 0;
}

int DataCollection::filterData() {
  return 0;
}

int DataCollection::popGyro(float gyro[]) {
  if(bufPosition < 1) return -1;
  readBuffer3(gyroReadings, gyro);
  return 0;
}

/**
 * Get the most recent accelerometer reading
 * \param accel A 3 element array to be filled with the x y and z components of accelerometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popAccel(float accel[]) {
  if(bufPosition < 1) return -1;
  readBuffer3(accelReadings, accel);
  return 0;
}

/**
 * Get most recent magnetometer reading
 * \param mag A 3 element array to be filled with the x y and z components of magnetometer reading
 * \return Returns 0 on success or -1 if buffer is empty
 */

int DataCollection::popMag(float mag[]) {
  if(bufPosition < 1) return -1;
  readBuffer3(magReadings, mag);
  return 0;
}

/**
 * Get most recent altitude reading.
 * \param alt a float to be filled with last altitude
 * \return Returns most recent altitude reading or -1 if the buffer is empty
 */

int DataCollection::popAlt(float *alt) {
  if(bufPosition < 1) return -1;
  return altReadings[bufPosition - 1];
}

/**
 * Collect a timestep of data and write to buffer.
 * Collect expects all sensors to be properly initialized.
 * \return Returns amount of space left in buffer or -1 if buffer in use.
 */

int DataCollection::collect() {
  if(bufPosition > BUFFER_SIZE) return 0;
  if(bufferLock) return -1;
  bufferLock = 1;

  time[bufPosition] = micros();
  int droppedReadings; //number of sensor readings ignored due to communication issues

  //Buffers for one timestep of readings from all sensors.
  float gyros[MPULength][3];
  float accels[MPULength][3];
  float mags[MPULength][3];
  float alts[MPLLength];

  droppedReadings = 0;
  for(int x = 0; x < MPULength; x++) {
    if(mpuError[x]) {
      droppedReadings++;
      continue;
    }

    MPU *mpu = mpus[x];
    debug("MPU" + (String)x + ":");
    debug(mpu->selfTest());

    float tmp[3];
    mpuError[x] = mpuError[x] ? mpuError[x] : mpu->readGyro(tmp);
    loadBuffer3(tmp, gyros, x-droppedReadings);

    mpuError[x] = mpuError[x] ? mpuError[x] : mpu->readAccel(tmp);
    loadBuffer3(tmp, accels, x-droppedReadings);

    mpuError[x] = mpuError[x] ? mpuError[x] : mpu->readMag(tmp);
    loadBuffer3(tmp, mags, x-droppedReadings);
  }

  float tmp[3];

  //average each set of data from MPU and load in to buffer
  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, gyroReadings, bufPosition);

  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, accelReadings, bufPosition);

  average3(gyros, MPULength - droppedReadings, tmp);
  loadBuffer3(tmp, magReadings, bufPosition);

  droppedReadings = 0;
  for(int x = 0; x < MPLLength; x++) {
    if(mplError[x]) {
      droppedReadings++;
      continue;
    }

    MPL *mpl = mpls[x];

    float alt;
    mplError[x] = mplError[x] ? mplError[x] : mpl->readAGL(alt);
    alts[x - droppedReadings] = alt;
  }

  alts[bufPosition] = average(alts, MPLLength - droppedReadings);

  curr_alt = alts[bufPosition];  
  readBuffer3(accelReadings, curr_accel);
  
  bufPosition++;
  bufferLock = 0;
  return BUFFER_SIZE - bufPosition;
}

/* writeData: writes contents of buffer to SD card.
 * returns: 0 on success, -1 if the buffer is locked, or SD card error code.
 */

int DataCollection::writeData() {
  if (bufferLock) return -1;
  bufferLock = 1;
  float lastGyro[3];
  float lastAccel[3];
  float lastMag[3];
  float lastAlt;
  unsigned int lastTime;

  readBuffer3(gyroReadings, lastGyro);
  readBuffer3(accelReadings, lastAccel);
  readBuffer3(magReadings, lastMag);
  lastAlt = altReadings[bufPosition - 1];
  lastTime = time[bufPosition - 1];

  //write data to SD card here

  bufPosition = 0;
  loadBuffer3(lastGyro, gyroReadings, bufPosition);
  loadBuffer3(lastAccel, accelReadings, bufPosition);
  loadBuffer3(lastMag, magReadings, bufPosition);
  altReadings[bufPosition] = lastAlt;
  time[bufPosition] = lastTime;

  bufPosition++;
  bufferLock = 0;
  return 0;
}

void DataCollection::readBuffer3(float buf[][3], float data[]) {
  data[0] = buf[bufPosition - 1][0];
  data[1] = buf[bufPosition - 1][1];
  data[2] = buf[bufPosition - 1][2];
}

void DataCollection::loadBuffer3(float data[], float buf[][3], int pos) {
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
  float avg = 0;;
  for(int x = 0; x < length; x++) {
    avg += data[x];
  }

  return avg/length;
}

void DataCollection::debug(String msg) {
  Serial.println(msg);
}



/*#include <DataCollection.h>



 DataCollection::DataCollection() {
	Serial.begin(9600);
	delay(500);

	// Initialize MPU
	Serial.println(mpu1.begin(1, 0x68));
	mpu1.initGyro(250);
	mpu1.initAccel(2); // lol idk which number should go here
	mpu1.initMag();

	// Initialize MPL
	PSensor1 = new MPL;
	PSensor1->begin(0);
	PSensor1->setGround(); // think we need this (???? double check)
	
	//Initialize pins for ignition circuits as outputs
	
	update();
}


int DataCollection::update() {

	//stage 1, get raw data put it in the Buffers
	
	//stage 2, do math to get the right values into the BEST GUESS feilds 



	//TEMP TO TEST NOT REALLY THE OPERATIONS
	previousALTITUDE = currentALTITUDE;
	currentALTITUDE = PSensor1->readAlt();


	//these will have to be a convertion to float later
	previousAcceleration[0] = currentAcceleration[0];
	previousAcceleration[1] = currentAcceleration[1];
	previousAcceleration[2] = currentAcceleration[2];
	
	mpu1.readAccel(currentAcceleration);




	return 0;
}
int DataCollection::writeToSD() {
	return 0;

}
int DataCollection::getBufferSize() {
	return 0;
}

void DataCollection::filterALT1() {




}*/
