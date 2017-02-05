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
	void reset();

	bool detect_launch(int curr_accel, int launch_count);
	bool detect_burnout(int curr_accel, int prev_accel, int burnout_count, unsigned long launch_time, unsigned long curr_time);

	bool coasting();
	bool test_apogee();
	bool deploy_drogue();
	bool deploy_payload();
	bool detect_main_alt(float curr_altitude);
	bool deploy_main();
	void final_descent();
	
	int currentState;
	int nextState;
	

};

#endif
