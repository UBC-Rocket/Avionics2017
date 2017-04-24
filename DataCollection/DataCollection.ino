#include <i2c_t3.h>
#include <String.h>
#include "MPLSim.h"
#include "MPUSim.h"
#include "DataCollection.h"

MPU *mpu1;
MPL *mpl1;

DataCollection collector;

void setup() {
  Serial.begin(9600);

  while(!Serial) delay(1);
  Serial.println("Beginning...");
  Serial.println("==============");

  pinMode(10, OUTPUT); //neccesary for some reason?

  if(!SD.begin(4)) Serial.println("SD failed to load");
  else Serial.println("SD initialized");

  mpl1 = new MPL();
  Serial.print("mpl1 created: ");
  Serial.println((int)(&mpl1), HEX);
  mpu1 = new MPU();

  Serial.println("MPU1 init: ");
  Serial.println(mpu1->begin(1, 0x68));

  Serial.println("MPL1 init: ");
  Serial.println(mpl1->begin(0, 0x60));

  MPU *mpus[2] = {mpu1};
  MPL *mpls[1] = {mpl1};

  Serial.println("Data Collection init");
  collector.begin(mpus, 1, mpls, 1);
}

void loop() {
  delay(100);
  Serial.println("Collecting data: " + (String)collector.collect());
  float accel[3];
  float alt;
  Serial.println("Accels: " + (String)collector.popAccel(accel));
  Serial.println(micros());
  Serial.println(accel[0]);
  Serial.println(accel[1]);
  Serial.println(accel[2]);
  Serial.println("Alt: " + (String)collector.popAlt(alt));
  Serial.println(alt);
  Serial.println("--------");
}
