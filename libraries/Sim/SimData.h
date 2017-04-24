#ifndef SIMDATA_H
#define SIMDATA_H

#define BUF_LENGTH 100

class SimData {
  unsigned int times[BUF_LENGTH];
  short gyros[BUF_LENGTH];
  short accels[BUF_LENGTH];
  short mags[BUF_LENGTH];
  float alts[BUF_LENGTH];
  float temps[BUF_LENGTH];

  int filePos;
  int lastPos;

public:
  //SimData();
  short getGyro(unsigned int time);
  short getAccel(unsigned int time);
  short getMag(unsigned int time);
  float getAlt(unsigned int time);
  float getTemp(unsigned int time);

private:
  int getPos(unsigned int time);
  int updateBuffer();
};

#endif
