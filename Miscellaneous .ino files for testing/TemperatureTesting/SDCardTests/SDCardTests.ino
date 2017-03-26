#include <i2c_t3.h>

#include <MPL.h>



#include <i2c_t3.h>
#include <TimerOne.h>
#include <stdio.h>
#include <SD.h>
#include <SD_t3.h>
#include <SPI.h>

#define BUFFER_SIZE 250
#define CS 4 //chip select pin
#define DIN 12 // MISO data into teensy, out of sd pin
#define DOUT 11 // MOSI data out of teensy, into sd pin
#define CLK 13


unsigned long time;



int16_t x, y, z;
int16_t x_off, y_off, z_off;
int16_t linx, liny, linz;
float x_pos, y_pos, z_pos;
unsigned long elapsed = 0;
bool w = false;
float altReadings[BUFFER_SIZE];
int bufferPosition;
File myfile;
MPL PSensor;

void setup() {


  
  delay(50);

  //must initialize teensy pin 10 as output or SD lib does not work
  pinMode(10, OUTPUT);

  
  bufferPosition = 0;
  PSensor.begin(1);
  PSensor.setGround();
  Serial.begin(9600);
  
  delay(5000);
  x = 0;


  //----------------------------------------------SD CARD SHIT
  if (!SD.begin(CS)) {
    Serial.println("Card failed, or not present");
    return;
  }
   Serial.println("Card successfully initialized");
  Serial.println("---------------");
    
    
    Serial.println("Accessing root directory...");
  File root = SD.open("/");
  Serial.println("---------------");


   if(!SD.exists("sdtest.TXT")){
    Serial.println("File does not exist");
    return;
  }

    Serial.println("---------------");
  Serial.println("Reading test file...");
  Serial.println("---------------");
  
  myfile = SD.open("sdtest.TXT");


// if the file is available, read the file
  if (myfile) 
  {
    while (myfile.available())
    {
      Serial.write(myfile.read());
    }
    myfile.close();
  }  
  // if the file cannot be opened give error report
  else {
    Serial.println("error opening the text file");
  } 

  //tried to reset/overwrite file -- did not work
  Serial.println(myfile.size());
 //// Serial.println("Resetting position...");
 Serial.println(myfile.seek(12));
  //Serial.println("New position:");
  Serial.println(myfile.position());

  
}





void loop() {
  delay(100);
  
  float temperature = 0;
  PSensor.readAGL(&temperature);

   altReadings[bufferPosition++] = temperature;
  Serial.println(bufferPosition); 
  
  Serial.println(x);  
  Serial.println(temperature); 

  dumpToDisk();
x++; 
}
void dumpToDisk() {

    
Serial.println("dumptest");
  if(bufferPosition == BUFFER_SIZE/2 ) {
    dump( 0, BUFFER_SIZE/2);
    
   }
  if(bufferPosition == BUFFER_SIZE -1){
    bufferPosition = 0;
    dump(BUFFER_SIZE/2, BUFFER_SIZE-1); 
  }

}
void dump(int low, int high) {
 
  Serial.print("Starting Write: ");
  time = micros();
  unsigned long starttime = time;

  Serial.println(myfile.position());
 myfile = SD.open("sdtest.TXT", FILE_WRITE);
    Serial.println(  myfile.write((char)8) );
    myfile.close();
    Serial.println(myfile.position());
    Serial.print("OneByteDown: ");
  time = micros();
  
  //prints time since program started
   Serial.println(time - starttime);
   

   starttime = time;
   
 myfile = SD.open("sdtest.TXT", FILE_WRITE);
    myfile.write(   (  ((char*)altReadings) + (low*4)    ) , (4 *(BUFFER_SIZE/2)) );
    myfile.close();
    Serial.println(myfile.position());
    
    Serial.print("500 byte down: ");
  time = micros();
  //prints time since program started
  Serial.println(time - starttime);
Serial.println("dump");
Serial.println(low);
Serial.println(high);

}


