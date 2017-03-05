#include "MPU.h"
#include <i2c_t3.h>

#define PWR_MNG 0x6B //register address of pwr management
#define USR_CNTRL 0x6A //register address of user control
#define WHOAMI 0x75 //test adress should always read 0x73

#define GYRO_SETUP 0x1B //register address of gryoscope setup
#define ACCEL_SETUP 0x1C

#define MAG_ADDR 0x0c //i2c address of the magnetometer behind the main chip
#define SLV0_ADDR 0x25 //register address of i2c address to access with slave 0
#define SLV0_REG 0x26 //register address to put internal slave 0 register address to modify
#define SLV0_CNTRL 0x27 //register address of slave 0 control byte
#define SLV0_WRITE 0x63 //register address at which to prepare byte to write to slave 0

#define MAG_BASE_REG 0x3 //internal base register for magnetometer readings

#define GYRO_BASE 0x43
#define ACCEL_BASE 0x3B
#define MAG_BASE 0x49

#define MEM_REG_ADDR 0x6D //register address of DMP memory register for writing
#define MEM_DATA_ADDR 0x6F //register address of DMP memory register for data

#define magFS 4800.0
#define shortFS 32768.0

#define FIFO_COUNT 0x72 //high byte of two byte set of registers containing length of data in fifo
#define FIFO_RW 0x74 //register to read fifo data from. Read sequentially from the same address.

//hard values from invensense for DMP registers
#define DMP_START_ADDR 0x70
#define DMP_CODE_SIZE           (3062)
#define CFG_9_QUAT              (2712)

int MPU::begin(bool whichWire, uint8_t Addr) {
  start = micros();
  lastTimeIndex = 0;
}

int MPU::selfTest() {
  uint8_t test;
  test = 0x1a;

  return test;
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

  gyroFS = fullScale;
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

  accelFS = fullScale;
  return 0;
}

int MPU::initMag() {

  return 0;
}

int MPU::readGyro(int16_t *data) {
  unsigned long time = micros();
  int gyroVal = gyro.lower_bound(time-start);
  return gyroVal;
}

int MPU::readAccel(int16_t *data) {
  uint8_t rawData[6];
  int err;
  if(err = read(ACCEL_BASE, 6, rawData)) return err;

  data[0] = rawData[1] | (rawData[0]<<8);
  data[1] = rawData[3] | (rawData[2]<<8);
  data[2] = rawData[5] | (rawData[4]<<8);
  return 0;
}

int MPU::readMag(int16_t* data) {
  uint8_t rawData[6];
  int err;
  if(err = read(MAG_BASE, 6, rawData)) return err;

  data[0] = rawData[1] | (rawData[0]<<8);
  data[1] = rawData[3] | (rawData[2]<<8);
  data[2] = rawData[5] | (rawData[4]<<8);
  return 0;
}

void MPU::cleanGyro(float* cleanData, int16_t* data) {
  for(int i = 0; i < 3; i++) {
    cleanData[i] = data[i] * gyroFS / shortFS;
  }
}

int MPU::readDMP(long quat[]) {
  int err;
  uint8_t data[16];
  unsigned short fifoCount;
  uint8_t tmp[2];

  if(err = read(FIFO_COUNT, 2, tmp)) return err;
  fifoCount = (tmp[0] << 8) | tmp[1];
  debug("Data available in FIFO: " + (String)fifoCount);
  if(fifoCount == 0) return -1;

  if(err = read(FIFO_RW, 16, data)) return err;

  quat[0] = ((long)data[0] << 24) | ((long)data[1] << 16) | ((long)data[2] << 8) | data[3];
  quat[1] = ((long)data[4] << 24) | ((long)data[5] << 16) | ((long)data[6] << 8) | data[7];
  quat[2] = ((long)data[8] << 24) | ((long)data[9] << 16) | ((long)data[10] << 8) | data[11];
  quat[3] = ((long)data[12] << 24) | ((long)data[13] << 16) | ((long)data[14] << 8) | data[15];

  return 0;
}

int MPU::loadDMP() {
  int err;
  uint16_t length;
  uint8_t chunkTest[16];
  uint8_t addrBytes[2] = {0x04, 0x00};

  for(int pos = 0; pos < DMP_CODE_SIZE; pos += length) {
    length = (DMP_CODE_SIZE - pos < 16) ? (DMP_CODE_SIZE - pos) : 16;

    debug("About to write DMP memory");
    if(err = writeMem(pos, length, &dmp_memory[pos])) return err;
    debug("DMP written to memory\r\n");
    if(err = readMem(pos, length, chunkTest)) return err;
    for(int i = 0; i < length; i++) {
      if(chunkTest[i] != dmp_memory[pos+i]) return -1; //verify memory was written correctly
    }
  }

  if(err = write(DMP_START_ADDR, 2, addrBytes)) return err; //set DMP start address to 0x0400

  return 0;
}

int MPU::getTimeIndex() {
  unsigned long time = micros() - start;

  while(time >= time(lastTimeIndex + 1)) {
    lastTimeIndex++;
  }

  return lastTimeIndex;
}
