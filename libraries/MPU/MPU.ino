#include "MPU.h"

MPU mpu;

int16_t accel[3];

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    delay(1);
  }
  
  mpu.begin(0, 0x68);
  mpu.initAccel(16);
  mpu.readAccel(accel);
  Serial.println(accel[0]);
  delay(500);
}

void loop() {
  
}
