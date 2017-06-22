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

	bool detect_launch(float accel, float alt);
	bool detect_burnout(float accel);
	bool coasting(float accel);
	
	bool test_apogee(float curr_alt, float prev_alt);
	
	bool detect_main_alt(float curr_alt);
	bool final_descent(float curr_alt, float prev_alt, float curr_accel, float prev_accel);
	void flight_complete();
	
	int currentState;
	int nextState;

};

#endif