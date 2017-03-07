/*
 * UBC ROCKET
 */

#include <Rocket.h>
#include <MPU.h>
#include "MPL.h"
#include <i2c_t3.h>
#include <TimerOne.h>
#include <DataCollection.h>

/*
 * State Definitions
 */
#define STANDBY         0
#define POWERED_ASCENT  1
#define COASTING        2
#define TEST_APOGEE     3
#define DEPLOY_DROGUE   4
#define DEPLOY_PAYLOAD  5
#define INITIAL_DESCENT 6
#define DEPLOY_MAIN     7
#define FINAL_DESCENT   8
#define LANDED          9

/*
 * Ignition Circuit Definitions
 */
const int DROGUE_IGNITION_CIRCUIT = 7;  //pin to drogue chute ignition circuit
const int PAYLOAD_IGNITION_CIRCUIT = 8; //pin to nose cone ignition circuit

Rocket rocket(STANDBY, STANDBY);
DataCollection* dataCollector;

//count variables for decisions
int launch_count = 0;
int burnout_count = 0;

//time variables
unsigned long launch_time;
unsigned long curr_time;

//temp for printing and TESTING
float curr_altitude;
int16_t curr_Acc[3];

void setup() {

  Serial.begin(9600);
  delay(500);

  //Initialize pins for ignition circuits as outputs
  pinMode(DROGUE_IGNITION_CIRCUIT, OUTPUT);
  pinMode(PAYLOAD_IGNITION_CIRCUIT, OUTPUT);
  
  dataCollector = new DataCollection();
}

/*
 * main loop function
 * uses a switch statement to switch between states
 * update next state after the switch statement
 */
void loop(){
  //Update The Data, Get next Best Guess at ALT ACC and VELO------------------
  dataCollector->update();
  
  //make sure ignition pins stay low!
  digitalWrite(DROGUE_IGNITION_CIRCUIT, LOW);
  digitalWrite(PAYLOAD_IGNITION_CIRCUIT, LOW);

  //PRINT THE DATA THIS IS FOR TESTING ONLY!
  curr_time = millis();
  Serial.println("Current time: " + (String)curr_time);

  curr_altitude = dataCollector->currentALTITUDE;
  Serial.println("Current Altitude: " + (String)curr_altitude);

  curr_Acc[0] = dataCollector->currentAcceleration[0];
  curr_Acc[1] = dataCollector->currentAcceleration[1];
  curr_Acc[2] = dataCollector->currentAcceleration[2];
  Serial.println("Current Acceleration X: " + (String)curr_Acc[0] + " Y: "+(String)curr_Acc[1] + " Z: " +(String)curr_Acc[2]);

  //MAKE A STATE CHANGE----------------------------------------
  switch (rocket.currentState){
      
    case STANDBY:
      if (rocket.detect_launch(0, launch_count)){
        rocket.nextState = POWERED_ASCENT;
        launch_time = millis();
      }
      break;
      
    case POWERED_ASCENT:
      rocket.detect_burnout(0, 0, burnout_count, launch_time, curr_time);
      rocket.nextState = COASTING;
      break;
      
    case COASTING:
      rocket.coasting();
      rocket.nextState = TEST_APOGEE;
      break;
      
    case TEST_APOGEE:
      if (rocket.test_apogee()){
        rocket.nextState = DEPLOY_DROGUE;
      }
      else{
        rocket.nextState = TEST_APOGEE;
      }
      break;
      
    case DEPLOY_DROGUE:
      digitalWrite(DROGUE_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
      rocket.deploy_drogue();
      rocket.nextState = DEPLOY_PAYLOAD;
      break;
      
    case DEPLOY_PAYLOAD:
      digitalWrite(PAYLOAD_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
      rocket.deploy_payload();
      rocket.nextState = INITIAL_DESCENT;
      break;
      
    case INITIAL_DESCENT:
      if(rocket.detect_main_alt(curr_altitude)){
        rocket.nextState = DEPLOY_MAIN;
      }
      break;
      
    case DEPLOY_MAIN:
      rocket.deploy_main();
      rocket.nextState = FINAL_DESCENT;
      break;
      
    case FINAL_DESCENT:
      if (rocket.final_descent()) //returns true when we've landed
        rocket.nextState = LANDED;
      else
        rocket.nextState = FINAL_DESCENT;
      break;

    case LANDED:
      rocket.flight_complete();
      
  }

  //Get Ready for next loop
  rocket.currentState = rocket.nextState;
  delay(50);

  Serial.print("\nCurrent State: ");
  Serial.println(rocket.currentState);
  Serial.print("Next State: ");
  Serial.println(rocket.nextState);
    
}
