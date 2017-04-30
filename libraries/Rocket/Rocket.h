/*
 * Rocket Header File 
 */

#ifndef Rocket_h
#define Rocket_h

#include "Arduino.h"

class Rocket
{
  private:
	
  public:
    Rocket(int currentState, int nextState);
	void setRocketStates(int currentState, int nextState);

	bool detect_launch(float curr_accel);
	bool detect_burnout(float curr_accel);
	bool coasting(float curr_accel);
	
	bool test_apogee(float curr_altitude, float prev_altitude);
	
	bool detect_main_alt(float curr_altitude);
	bool final_descent(float curr_altitude, float prev_altitude, float curr_accel, float prev_accel);
	void flight_complete();
	
	int currentState;
	int nextState;

};

#endif