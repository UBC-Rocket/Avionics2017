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
	bool detect_burnout(int curr_accel, int prev_accel, int burnout_count);
    bool standby();
	bool powered_ascent();
	bool coasting();
	bool test_apogee();
	bool deploy_drogue();
	bool deploy_payload();
	bool initial_descent();
	bool deploy_main(float curr_altitude);
	void final_descent();
	
	int currentState;
	int nextState;
	

};

#endif
