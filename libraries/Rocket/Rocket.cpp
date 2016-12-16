/*
 * Rocket.cpp 
 * Here are all the functions that the rocket can use 
 */

//#include "Arduino.h"
#include "Rocket.h"
#include "i2c_t3.h"
#include "MPU.h"

// Rocket State Contructor
Rocket::Rocket(int initialCurrentState, int initialNextState)
{
	currentState = initialCurrentState;
	nextState = initialNextState;
	//setRocketStates(currentState, nextState);
}

/*
 * Reset function will be called once at the beginning of each use
 * Calibrate all the sensors
 */
void Rocket::reset() {
	//do stuff here 
}

/*
 * Standby
 * check if the rocket has launched 
 * if yes, return TRUE 
 * if no, return FALSE 
 */
bool Rocket::standby(){
  bool launch = false;
  
  //checking for a change in altitude
  //if we are seeing enough change in altitude, set launch = true

  return launch;
}

/*
 * Powered_Ascent 
 * see if our flight is powered
 * if no, return TRUE 
 * if yes, return FALSE 
 */
bool Rocket::powered_ascent(){
	bool motorOff = false;
	
	//check if ascent is powered
	//if not, motorOff = true
	
	return motorOff;
}

/*
 * Coasting
 * check if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::coasting(){
	bool apogee = false;
	
	//if we think we're at apogee, apogee = true
	
	return apogee;
}

/*
 * Test_Apogee 
 * confirm if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::test_apogee(){
	bool apogee = false;
	
	//if we confirm we're at apogee, apogee = true
	
	return apogee; 
}

/*
 * Deploy Drogue Chute
 * send current to ignite the gunpowder
 * return true once the drogue chute has deployed 
 */
bool Rocket::deploy_drogue(){
	bool drogueDeployComplete = false;
	
	//send the signal to deploy the drogue
	//do we maybe want to double check that it actually happened?
		//if it did, drogueDeployComplete = true
	//if we dont want to double check, make this function void 
	
	return drogueDeployComplete;
}

/*
 * Deploy Payload
 * send current to ignite the gunpowder
 * return true once the payload has deployed 
 */
bool Rocket::deploy_payload(){
	bool payloadDeployComplete = false;
	
	//send the signal to deploy the payload
	//do we maybe want to double check that it actually happened?
		//if it did, payloadDeployComplete = true
	//if we dont want to double check, make this function void 
		
	return payloadDeployComplete;
}

/*
 * Initial Descent
 * see if out current altitude is where we want to deploy the main chute 
 * if yes, return TRUE 
 * if no, return FALSE
 */
bool Rocket::initial_descent(){
	bool deployMainSpot = false; //cant think of a more reasonable name 
	
	//if we think now would be a good time to deploy the main, deployMainSpot = true
	
	return deployMainSpot;
  
}

/*
 * Deploy Main
 * send current to ignite the gunpowder
 * return true once the main chute has deployed 
 */
bool Rocket::deploy_main(){
	bool mainDeployComplete = false;
	
	//send the signal to deploy the main
	//do we maybe want to double check that it actually happened?
		//if it did, mainDeployComplete = true
	//if we dont want to double check, make this function void 
		
	return mainDeployComplete;
  
}

/*
 * Final_Descent 
 */
void Rocket::final_descent(){
	//lol what do we actually do here 
}

