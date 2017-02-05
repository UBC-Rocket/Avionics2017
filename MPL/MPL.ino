#include <i2c_t3.h>

#include "MPL.h"


int16_t x, y, z;
int16_t x_off, y_off, z_off;
int16_t linx, liny, linz;
float x_pos, y_pos, z_pos;
unsigned long elapsed = 0;
bool w = false;

MPL* PSensor;

void setup() {
  delay(1000);

  PSensor = new MPL(w);

  Serial.begin(9600);
  
  delay(500);
  x = 0;
   
}

void loop() {
  delay(4);
  x++; 
  float altitude = 0;

  altitude = PSensor->readAltitude();
  Serial.print ("Offset: ");
  Serial.println(PSensor->getOffset());
  Serial.print("Current Altitude: "); 
  Serial.println(altitude); 

  //Serial.println(PSensor->readByte(0x01));
  Serial.println(x);  

}

