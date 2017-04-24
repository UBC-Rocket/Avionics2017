#include <DataCollection.h>



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

	//Initialize pins for ignition circuits as outputs

	update();
}


int DataCollection::update() {

	//stage 1, get raw data put it in the Buffers

	//stage 2, do math to get the right values into the BEST GUESS feilds



	//TEMP TO TEST NOT REALLY THE OPERATIONS
	previousALTITUDE = currentALTITUDE;
	currentALTITUDE = PSensor1->readAlt();


	//these will have to be a convertion to float later
	previousAcceleration[0] = currentAcceleration[0];
	previousAcceleration[1] = currentAcceleration[1];
	previousAcceleration[2] = currentAcceleration[2];

	mpu1.readAccel(currentAcceleration);




	return 0;
}
int DataCollection::writeToSD() {
	return 0;

}
int DataCollection::getBufferSize() {
	return 0;
}

void DataCollection::filterALT1() {




}
