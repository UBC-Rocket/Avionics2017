#include <i2c_t3.h>
#include <TimerOne.h>
#include "MPUSim.h"

#define ADDR 0x68

MPU test;

int16_t accel[3];

void setup() {
  delay(500);
  Serial.begin(9600);
  delay(2000);
  Serial.println("Initializing...");
  test.begin(0, ADDR);
  test.initAccel(16);

}

void loop() {
  Serial.println("Accel Data: ");

  test.readAccel(accel);

  Serial.println(accel[0]);
  Serial.println(accel[1]);
  Serial.println(accel[2]);

  delay(50);

}
