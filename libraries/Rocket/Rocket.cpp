/*
 * Rocket.cpp 
 * Here are all the functions that the rocket can use 
 */

#include "Rocket.h"
#include "i2c_t3.h"

//note: these are calibrated(ish) for GIANT rocket, no little ones
#define SIM_LAUNCH_ACCEL 	4 		//assuming sims are accurate, launch accel > 50
#define SIM_BURNOUT_ACCEL	2 		//post bunrout accel is 16 ish (but drops from 50 to 16)
#define MAIN_ALT			396 	//altitude at which to deploy the main in METERS

//these guys should be chill
#define G_FORCE				1		//use in coasting - little larger than G 
#define LANDED_THRESH		0.2		//use in landed - looking for hella stable data

// Rocket State Contructor
Rocket::Rocket(int initialCurrentState, int initialNextState)
{
	currentState = initialCurrentState;
	nextState = initialNextState;
	//setRocketStates(currentState, nextState);
}

bool Rocket::detect_launch(float curr_alt, float prev_alt) {
	//giant spike in acceleration 
	//return true when we've seen 4 positive accelerations in a row 
	
	if (curr_alt > prev_alt && curr_alt > 5)
		return true;
	else
		return false;
}

bool Rocket::detect_burnout(float curr_accel) {
	//giant drop in acceleration, max velocity (baby delay)
	
	if (curr_accel < SIM_BURNOUT_ACCEL ) //will the data be stable enough for this..?
		return true;
	else
		return false;
		
}

/*
 * Coasting
 * check if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::coasting(float curr_accel){
	
	//small delta accel
	//small delta alt
	//large timeout
	
	if (abs(curr_accel) < G_FORCE) 
		return true;
	else
		return false; 
		
}

/*
 * Test_Apogee 
 * confirm if our current location is apogee
 * if yes, return TRUE
 * if no, return FALSE  
 */
bool Rocket::test_apogee(float curr_altitude, float prev_altitude){
	//Passes if one of the following is true: 
	//delta pressure is positive or delta accel is BIG
	//check this a few times
	
	//big timeout following coasting
	
	if((curr_altitude - prev_altitude) < 0)
		return true;
	else
		return false;
	
}

/*
 * Initial Descent
 * see if out current altitude is where we want to deploy the main chute 
 * if yes, return TRUE 
 * if no, return FALSE
 */
bool Rocket::detect_main_alt(float curr_altitude){
	
	//want it less than main alt in case something goes horribly wrong 
	//and we should deploy it when we're close to the ground 
	if (curr_altitude < MAIN_ALT) 
		return true;
	else
		return false;
  
}

/*
 * Final_Descent 
 */
bool Rocket::final_descent(float curr_altitude, float prev_altitude, float curr_accel, float prev_accel){
	//read accel data to see if we've stopped moving
	//return true when we've landed
	if ((abs(curr_altitude - prev_altitude) < LANDED_THRESH)&& abs(curr_accel - prev_accel) < LANDED_THRESH)
		return true;
	else
		return false;
}

void Rocket::flight_complete(){
	//stop recording data to the SD card 
}