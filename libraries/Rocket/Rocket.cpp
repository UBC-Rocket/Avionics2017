/*
 * Rocket.cpp 
 * Here are all the functions that the rocket can use 
 */

#include "Rocket.h"
#include "i2c_t3.h"


#define SIM_LAUNCH_ACCEL 	4 		//assuming sims are accurate, launch accel > 50
#define SIM_BURNOUT_ACCEL	2 		//post bunrout accel is 16 ish (but drops from 50 to 16)
#define MAIN_ALT			396 	//altitude at which to deploy the main in METERS

#define ALT_TAKEOFF_THRESHOLD 50	//alt that the rocket has definitely taken off by
#define G_FORCE				1		//use in coasting - little larger than G 
#define LANDED_THRESH		0.2		//use in landed - looking for hella stable data

// Rocket State Contructor
Rocket::Rocket(int initialCurrentState, int initialNextState)
{
	currentState = initialCurrentState;
	nextState = initialNextState;
	//setRocketStates(currentState, nextState);
}

bool Rocket::detect_launch(float accel, float alt) {
	//giant spike in acceleration 
	//return true when we've seen 4 positive accelerations in a row 
	
	if (accel > SIM_LAUNCH_ACCEL)
		return true;
	else if (alt > ALT_TAKEOFF_THRESHOLD)
		return true; 
	else
		return false;
}

bool Rocket::detect_burnout(float accel) {
	//giant drop in acceleration, max velocity (baby delay)
	
	if (accel < SIM_BURNOUT_ACCEL ) //will the data be stable enough for this..?
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
bool Rocket::coasting(float accel){
	
	//small delta accel
	//small delta alt
	//large timeout
	
	if (abs(accel) < G_FORCE) 
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
bool Rocket::test_apogee(float curr_alt, float prev_alt){
	//Passes if one of the following is true: 
	//delta pressure is positive or delta accel is BIG
	//check this a few times
	
	//big timeout following coasting
	
	if((curr_alt - prev_alt) < 0)
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
bool Rocket::detect_main_alt(float curr_alt){
	
	//want it less than main alt in case something goes horribly wrong 
	//and we should deploy it when we're close to the ground 
	if (curr_alt < MAIN_ALT) 
		return true;
	else
		return false;
  
}

/*
 * Final_Descent 
 */
bool Rocket::final_descent(float curr_alt, float prev_alt, float curr_accel, float prev_accel){
	//read accel data to see if we've stopped moving
	//return true when we've landed
	if ((abs(curr_alt - prev_alt) < LANDED_THRESH)&& abs(curr_accel - prev_accel) < LANDED_THRESH)
		return true;
	else
		return false;
}

void Rocket::flight_complete(){
	//stop recording data to the SD card 
}