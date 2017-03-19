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
	bool coasting(float curr_accel, int coasting_count);
	
	bool test_apogee(float curr_altitude, float prev_altitude, int test_apogee_count);
	
	bool deploy_drogue();
	bool deploy_payload();
	bool detect_main_alt(float curr_altitude, int detect_main_alt_count);
	bool deploy_main();
	bool final_descent(float curr_altitude, float prev_altitude, float curr_accel, float prev_accel, int final_descent_count);
	void flight_complete();
	
	int currentState;
	int nextState;

};

#endif
