#include "MPUSim.h"
#include "MPLSim.h"

MPU *mpu;
MPL *mpl;

float tmp[10000];

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    delay(100);
  }
  tmp[0] = 1;
  tmp[10000-1] = 1;

  Serial.println("Beginning...");


  mpl = new MPL();
  Serial.println("MPL created");
  mpu = new MPU();
  Serial.println("Objects created");

  mpu->begin(0,0);
  mpl->begin(0,0);

  Serial.println("objects initialized");

  mpu->initAccel(16);

  delay(2000);
}

void loop() {
  float tmp[3];
  Serial.println(mpu->readAccel(tmp));
  Serial.println(tmp[2]);
  delay(1000);
}
