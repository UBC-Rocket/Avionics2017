#include <i2c_t3.h>
#include <String.h>
#include <MPL.h>
#include <MPU.h>
#include "DataCollection.h"

MPU *mpu1;
MPL *mpl1;

DataCollection collector;

void setup() {
  Serial.begin(9600);

  while(!Serial) delay(100);
  Serial.println("Beginning...");
  Serial.println("==============");

  mpu1 = new MPU();
  mpl1 = new MPL();

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
  delay(500);
  collector.collect();
  float accel[3];
  Serial.println("Accels: " + (String)mpu1->readAccel(accel));
  Serial.println(accel[0]);
  Serial.println(accel[1]);
  Serial.println(accel[2]);
  Serial.println("--------");
  
  int16_t accelRaw[3];
  //Serial.println("Raw Accels: " + (String)mpu1->readAccel(accelRaw));
  Serial.println(accelRaw[0]);
  Serial.println(accelRaw[1]);
  Serial.println(accelRaw[2]);
}
