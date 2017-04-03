
#include <DataCollection.h>
#include <i2c_t3.h>
#include <SD.h>
#include <SD_t3.h>
#include <SPI.h>
#include <stdio.h>

#define CS 4 //chip select pin
#define DIN 12 // MISO data into teensy, out of sd pin
#define DOUT 11 // MOSI data out of teensy, into sd pin
#define CLK 13 // SD clock
#define LED_PIN 7 // LED pin for initialization


 DataCollection::DataCollection() {
	Serial.begin(9600);
	delay(500);

	// Initialize MPU
	Serial.println(mpu1.begin(1, 0x68));
	mpu1.initGyro(250);
	mpu1.initAccel(2); // lol idk which number should go here
	mpu1.initMag();

	// Initialize MPL
	PSensor1 = new MPL;
	PSensor1->begin(0);
	PSensor1->setGround(); // think we need this (???? double check)
	
	//initialize SD file and data string
	file_ptr = (const char*)&fileName;
	flightData = SD.open(fileName2, FILE_WRITE);
	if(flightData){
    		flightData.println("-----------------");
    		flightData.println("Time, Altitude, X Accel, Y Accel, Z Accel");
    		Serial.println("File write successful");
    		//closing the file saves the data to the SD card
    		flightData.close();
  	}
  	else {
    		Serial.println("File does not exist");
  	}
  
	//Initialize pins for ignition circuits as outputs
	
	update();
}


int DataCollection::update() {

	//stage 1, get raw data put it in the Buffers
	
	//stage 2, do math to get the right values into the BEST GUESS feilds 



	//TEMP TO TEST NOT REALLY THE OPERATIONS
	previousALTITUDE = currentALTITUDE;
	currentALTITUDE = PSensor1->readAGL();


	//these will have to be a convertion to float later
	previousAcceleration[0] = currentAcceleration[0];
	previousAcceleration[1] = currentAcceleration[1];
	previousAcceleration[2] = currentAcceleration[2];
	
	mpu1.readAccel(currentAcceleration);

	//read current time
	curr_time = millis();
 


	return 0;
}

int DataCollection::writeToSD() {
	
	curr_time = millis();

	dataString += (curr_time/1000.0);
  	dataString += (", ");
  	dataString += (currentALTITUDE);  
  	dataString += (", ");  
    	dataString += (currentAcceleration[0]);  
    	dataString += (", ");  
    	dataString += (currentAcceleration[1]);  
    	dataString += (", ");  
    	dataString += (currentAcceleration[2]);  
    	dataString += (", ");  
	
  	flightData = SD.open(fileName2, FILE_WRITE);
  	flightData.println(dataString);
  	flightData.flush();
  	flightData.close();
	
	dataString = "";
	return 0;

}
int DataCollection::getBufferSize() {
	return 0;
}

void DataCollection::filterALT1() {




}
