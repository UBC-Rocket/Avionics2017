/*
 * Rocket.cpp 
 * Here are all the functions that the rocket can use 
 */

#include "Rocket.h"
#include "i2c_t3.h"
#include "MPU.h"
#include "MPL.h"

#define SIM_LAUNCH_ACCEL 	40 		//assuming sims are accurate, > 40
#define SIM_BURNOUT_ACCEL	1
#define NUM_CHECKS 			4
#define MAIN_ALT			15000 	//height at which to deploy the main

// Rocket State Contructor
Rocket::Rocket(int initialCurrentState, int initialNextState)
{
	currentState = initialCurrentState;
	nextState = initialNextState;
	//setRocketStates(currentState, nextState);
}

bool Rocket::detect_launch(float curr_accel, int launch_count) {
	//giant spike in acceleration 
	//return true when we've seen 4 positive accelerations in a row 
	
	if (curr_accel > SIM_LAUNCH_ACCEL)
		launch_count++;
	
	if (launch_count > NUM_CHECKS)
		return true;
	else{
		launch_count = 0;
		return false;
	}
}

bool Rocket::detect_burnout(float curr_accel, float prev_accel, int burnout_count, unsigned long launch_time, unsigned long curr_time) {
	//giant drop in acceleration, max velocity (baby delay)
	
	if (abs(prev_accel - curr_accel) > SIM_BURNOUT_ACCEL ) //will the data be stable enough for this..?
		burnout_count++;
	
	if ((burnout_count > NUM_CHECKS) || (curr_time > (launch_time + 4500)))
		return true;
	else{
		burnout_count = 0;
		return false;
	}
		
}

/*
 * Coasting
 * check if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::coasting(float curr_accel, int coasting_count){
	//TODO: figure out what kind of data threshold we should be looking for
	//Returns true if one of the following is true: timeout, decrease in delta 
	//accel, or the barometric data stabalizes a lot
	
	if (curr_accel < 10.5)
		coasting_count++;
	
	if(coasting_count > NUM_CHECKS)
		return true;
	
	else{
		coasting_count = 0;
		return false;
	}
		
}

/*
 * Test_Apogee 
 * confirm if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::test_apogee(float curr_altitude, float prev_altitude, int test_apogee_count){
	//Passes if one of the following is true: 
	//delta pressure is positive or delta accel is BIG
	//check this a few times
	
	if((curr_altitude - prev_altitude) > 0)
		test_apogee_count++;
	
	if(test_apogee_count > NUM_CHECKS)
		return true;
	else{
		test_apogee_count = 0;
		return false;
		
	}
	
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
bool Rocket::detect_main_alt(float curr_altitude, int detect_main_alt_count){
	
	//want it less than main alt in case something goes horribly wrong 
	//and we should deploy it when we're close to the ground 
	if (curr_altitude < MAIN_ALT){ 
		detect_main_alt_count++;
	}
	
	if (detect_main_alt_count < NUM_CHECKS){
		return true;
	}
	else{
		detect_main_alt_count = 0;
		return false;
	}
  
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
bool Rocket::final_descent(float curr_altitude, float prev_altitude, float curr_accel, float prev_accel, int final_descent_count){
	//read accel data to see if we've stopped moving
	//return true when we've landed
	if (((curr_altitude - prev_altitude) == 0)&& (curr_accel - prev_accel) == 0)
		final_descent_count++;
	
	if (final_descent_count > NUM_CHECKS)
		return true;
	else{
		final_descent_count = 0;
		return false;
	}
		
}

void Rocket::flight_complete(){
	//stop recording data to the SD card 
}
