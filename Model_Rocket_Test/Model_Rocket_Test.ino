
/*
 * UBC ROCKET - Model Rocket Test
 * 19/03/2017
 */

#include "MPL.h"
#include "MPU.h"
#include <i2c_t3.h>
#include <TimerOne.h>
#include <stdio.h>
#include <SD.h>
#include <SD_t3.h>
#include <SPI.h>


int i = 0;  //data write position accel
int j = 0;  //data write position gyro
int k = 0;  //data write position mag
bool w = 1; //wire for sensor initialization

//initialize sensors
MPL PSensor;
MPU IMU;

//initialize flight data log file
File flightData;
//String fileName = "

//initialize sensor readings
float curr_alt; 
int16_t curr_accel[3];
int16_t curr_gyro[3];
int16_t curr_mag[3];
unsigned long curr_time;

#define CS 4 //chip select pin
#define DIN 12 // MISO data into teensy, out of sd pin
#define DOUT 11 // MOSI data out of teensy, into sd pin
#define CLK 13 // SD clock
#define LED_PIN 7 // LED pin for initialization

// initialize string to buffer data before logging to SD
String dataString = "";

void setup() {
  Serial.begin(9600);

  //must initialize teensy pin 10 as output or SD lib does not work
  pinMode(10, OUTPUT);

  //LED ON as start signal
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  delay(2000);
  
  // see if the card is present and can be initialized
  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card successfully initialized");
  Serial.println("---------------");

  // Open existing file on SD card
  //Serial.println("Opening test file..."); 
  flightData = SD.open("FLIGHT~2.TXT", FILE_WRITE);
  
  if(flightData){
    flightData.println("-----------------");
    flightData.println("Time, Altitude, X Accel, Y Accel, Z Accel, X Gyro, Y Gyro, Z Gyro, X Mag, Y Mag, Z Mag");
    Serial.println("File write successful");
    //closing the file saves the data to the SD card
    flightData.close();
  }
  else {
    Serial.println("File does not exist");
  }
  
  //Serial.println("---------------");
  
  
  Serial.println("Start Time: ");
  Serial.println(millis()/1000);
  Serial.println("Sensor Initialization: ");
  
  // Initialize sensors
  PSensor.begin(w);
  IMU.begin(w, 0x68);
  IMU.initGyro(2000);
  IMU.initAccel(16);
  IMU.initMag();

  Serial.println("Sensor self test:");
  Serial.println(IMU.selfTest());
}

void loop(){

  
  //update current values from sensors
  Serial.println("Reading sensor data");
  curr_time = millis();
  curr_alt = PSensor.readAlt();
  IMU.readGyro(curr_gyro); 
  IMU.readAccel(curr_accel); 
  IMU.readMag(curr_mag); 

  Serial.println("---------------");
  Serial.println("readGyro returns:");
  Serial.println(IMU.readGyro(curr_gyro));
  Serial.println("readAccel returns:");
  Serial.println(IMU.readAccel(curr_accel));
  Serial.println("readMag returns:");
  Serial.println(IMU.readMag(curr_mag));
  
  dataString += (curr_time/1000.0);
  dataString += (", ");
  dataString += (curr_alt);  
  dataString += (", ");  
  if(i<3){
    dataString += (curr_accel[i]);  
    dataString += (", ");  
    i++;
  }
  if(j<3){
    dataString += (curr_gyro[i]);  
    dataString += (", ");  
    j++;
  }
  if(k<3){
    dataString += (curr_mag[i]);  
    dataString += (", ");  
    k++;
  }

  /*
  flightData = SD.open("FLIGHT~2.TXT", FILE_WRITE);
  flightData.println(dataString);
  Serial.println("Printed to data log");
  flightData.flush();
  flightData.close();
*/
  Serial.println("Output to SD:");
  Serial.println(dataString);
  // Re-initialize data string and indices
  dataString = "";
  i=0;
  j=0;
  k=0;
}

