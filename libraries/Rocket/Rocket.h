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

	bool detect_launch(float curr_accel, int launch_count);
	bool detect_burnout(float curr_accel, float prev_accel, int burnout_count, unsigned long launch_time, unsigned long curr_time);

	bool coasting();
	bool test_apogee();
	bool deploy_drogue();
	bool deploy_payload();
	bool detect_main_alt(float curr_altitude);
	bool deploy_main();
	bool final_descent();
	void flight_complete();
	
	int currentState;
	int nextState;

};

#endif
