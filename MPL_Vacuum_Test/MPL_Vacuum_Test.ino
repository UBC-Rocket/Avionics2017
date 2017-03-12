
/*
 * UBC ROCKET - Pressure Sensor Vacuum Chamber Test Code
 */

#include "MPL.h"
#include <i2c_t3.h>
#include <TimerOne.h>
#include <stdio.h>
#include <SD.h>
#include <SD_t3.h>
#include <SPI.h>


/*
 * Ignition Circuit Definitions
 */
 
const int DROGUE_IGNITION_CIRCUIT = 7;  //pin to drogue chute ignition circuit
const int MAIN_IGNITION_CIRCUIT = 8; //pin to nose cone ignition circuit

/*
 * MPL Initializations
 */

int status = 0; //apogee or main detect status
int i = 0;  //buffer position
int j = 0; //status of launch detect
bool w = false;
MPL* PSensor;

File flightData;

float curr_altitude; 
float prev_altitude = 0;
int apogee_detect=0;

unsigned long drogue_launch_time;
unsigned long main_launch_time;
unsigned long curr_time;

#define BUFFER_SIZE 1
#define CS 4 //chip select pin
#define DIN 12 // MISO data into teensy, out of sd pin
#define DOUT 11 // MOSI data out of teensy, into sd pin
#define CLK 13

// make a string for assembling the data to log:
String dataString = "";
    
void setup() {
  Serial.begin(9600);
  
  //Serial.print("Initializing SD card...");
  
  //must initialize teensy pin 10 as output or SD lib does not work
  pinMode(10, OUTPUT);

  pinMode(DROGUE_IGNITION_CIRCUIT, OUTPUT);
  digitalWrite(DROGUE_IGNITION_CIRCUIT, HIGH);
  
  delay(2000);
  
  //Serial.println("Before SD begin");  
  // see if the card is present and can be initialized:
  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("Card successfully initialized");
  Serial.println("---------------");

  // Open existing file on SD card
  //Serial.println("Opening test file..."); 
  flightData = SD.open("/TEST/FLIGHT~3.TXT", FILE_WRITE);
  
  if(flightData){
    flightData.println("-----------------");
    flightData.println("Time, Altitude");
    Serial.println("File write successful");
    //closing the file saves the data to the SD card
    flightData.close();
  }
  else {
    Serial.println("File was not created");
  }
  
  //Serial.println("---------------");
  
  
  Serial.println("Start Time: ");
  Serial.println(millis()/1000);
  Serial.println("MPL Initialization: ");
  
  // Initialize MPL
  PSensor = new MPL(w);
  
  Serial.println("MPL Test Data: ");
  Serial.println("---------------");

  delay(60000); //delay after setup to put teensy in chamber
}

void loop(){

while(i<BUFFER_SIZE){
    //make sure ignition pins stay low!
    digitalWrite(DROGUE_IGNITION_CIRCUIT, LOW);
    digitalWrite(MAIN_IGNITION_CIRCUIT, LOW);
  
    curr_time = millis();
    curr_altitude = PSensor->readAltitude();
    
    Serial.print("Time: ");
    Serial.println(curr_time/1000);
    Serial.print("Current Altitude: ");
    Serial.println(curr_altitude);

    if(curr_altitude-prev_altitude<0){
      apogee_detect++;
    }
    
    if(curr_altitude > 3000 && apogee_detect == 5){ //10000 ft is 3048 m
      Serial.println("Altitude is 10000. Drogue is deployed.");
      digitalWrite(DROGUE_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
      status = 1;
      }
    else if (curr_altitude > 390 && curr_altitude < 400 && status > 0){ //1300 ft is 396 m
      Serial.println("Altitude is 1300. Main is deployed.");
      digitalWrite(MAIN_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
      status = 2;
    }
  
    Serial.println("---------------");
  
    dataString += (curr_time);
    dataString += (", ");
    dataString += (curr_altitude);  
    dataString += (", ");  
    if(status==1){
      dataString += ("APOGEE DETECED, ");
    }
    else if (status ==2){
      dataString += ("MAIN DEPLOYED, ");
      }
    Serial.println("Buffer updated # ");
    Serial.println(i);

    i++;
    status = 0;

    delay(50);
   }

   if(i==BUFFER_SIZE){
      flightData = SD.open("/TEST/FLIGHT~3.TXT", FILE_WRITE);
      flightData.println(dataString);
      Serial.println("Printed to data log");
      flightData.flush();
      flightData.close();
      i++;
  }
 
  // Re-initialize data string and buffer
  dataString = "";
  i=0;
  prev_altitude = curr_altitude;
  
}

