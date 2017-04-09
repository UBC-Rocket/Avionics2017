#include "MPUSim.h"
#include "MPLSim.h"

MPU *mpu;
MPL *mpl;

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    delay(1);
  }

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

  float tmp[3];
  Serial.println(mpu->readAccel(tmp));
  Serial.println(tmp[2]);
}

void loop() {
  // put your main code here, to run repeatedly:

}
