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
    bool standby();
	bool powered_ascent();
	bool coasting();
	bool test_apogee();
	bool deploy_drogue();
	bool deploy_payload();
	bool initial_descent();
	bool deploy_main();
	void final_descent();
	
	int currentState;
	int nextState;
	

};

#endif
