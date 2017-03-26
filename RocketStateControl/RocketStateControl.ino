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

#define NUM_CHECKS      4               //each condition has to pass 5 times 

/*
 * Ignition Circuit Definitions
 */
const int DROGUE_IGNITION_CIRCUIT = 7;  //pin to drogue chute ignition circuit
const int PAYLOAD_IGNITION_CIRCUIT = 8; //pin to nose cone ignition circuit
const int MAIN_IGNITION_CIRCUIT = 9;

Rocket rocket(STANDBY, STANDBY);
DataCollection* dataCollector;

//count variables for decisions
int launch_count = 0;
int burnout_count = 0;
int coasting_count = 0;
int test_apogee_count = 0;
int temp_apogee_count = 0;
int test_apogee_failed = 0;
int detect_main_alt_count = 0;
int landed_count = 0;

//time variables
unsigned long curr_time;
unsigned long launch_time;
unsigned long deploy_drogue_time;

//temp for printing and TESTING
float curr_altitude;
float prev_altitude;
int16_t curr_Acc[3];
int16_t prev_Acc[3];

void setup() {

  Serial.begin(9600);
  delay(500);

  //Initialize pins for ignition circuits as outputs
  pinMode(DROGUE_IGNITION_CIRCUIT, OUTPUT);
  pinMode(PAYLOAD_IGNITION_CIRCUIT, OUTPUT);
  pinMode(MAIN_IGNITION_CIRCUIT, OUTPUT);
  
  dataCollector = new DataCollection();

  //do we wanna turn on an LED to confirm that we're all initialized?
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
  //TODO: sqrt squared of these values??
  Serial.println("Current Acceleration X: " + (String)curr_Acc[0] + " Y: "+(String)curr_Acc[1] + " Z: " +(String)curr_Acc[2]);

  //MAKE A STATE CHANGE----------------------------------------
  switch (rocket.currentState){
      
    case STANDBY:
      if ((rocket.detect_launch(curr_Acc[2], launch_count)) > NUM_CHECKS){
        rocket.nextState = POWERED_ASCENT;
        launch_time = millis();
      }
      break;
      
    case POWERED_ASCENT:
      if ( rocket.detect_burnout(curr_Acc[2], burnout_count) > NUM_CHECKS ){
        rocket.nextState = COASTING;
      }
      else if (curr_time > (launch_time + 4500)){
        rocket.nextState = COASTING;
      }
      break;
      
    case COASTING:
      if( rocket.coasting(curr_Acc[2], coasting_count) > NUM_CHECKS ){
        rocket.nextState = TEST_APOGEE;
      }
    
      break;
      
    case TEST_APOGEE:
      temp_apogee_count = test_apogee_count;
      test_apogee_count = rocket.test_apogee(curr_altitude, prev_altitude, test_apogee_count);
      
      if ( test_apogee_count > NUM_CHECKS ){
        rocket.nextState = DEPLOY_DROGUE;
        test_apogee_failed = 0;
      }
      //this is gross and im embarassed - will fix this shit
      else if (test_apogee_count == temp_apogee_count){
        test_apogee_failed++;
      }

      if (test_apogee_failed > NUM_CHECKS){
        rocket.nextState = COASTING;
      }
      
      break;
      
    case DEPLOY_DROGUE:
      deploy_drogue_time = millis();
      digitalWrite(DROGUE_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
      //TODO: pop in "delay" to keep signal on for 2 seconds
      rocket.nextState = DEPLOY_PAYLOAD;
      break;
      
    case DEPLOY_PAYLOAD:
      if (curr_time > deploy_drogue_time + 3000){
        digitalWrite(PAYLOAD_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
        //TODO: pop in "delay" to keep signal on for 2 seconds
        rocket.nextState = INITIAL_DESCENT;
      }
      
      break;
      
    case INITIAL_DESCENT:
      if( rocket.detect_main_alt(curr_altitude, detect_main_alt_count) > NUM_CHECKS ){
        rocket.nextState = DEPLOY_MAIN;
      }
      break;
      
    case DEPLOY_MAIN:
      digitalWrite(MAIN_IGNITION_CIRCUIT, HIGH); 
      //TODO: pop in "delay" to keep signal on TIRTH seconds
      rocket.nextState = FINAL_DESCENT;
      break;
      
    case FINAL_DESCENT:
      if (rocket.final_descent(curr_altitude, prev_altitude, curr_Acc[2], prev_Acc[2], landed_count) > NUM_CHECKS ) //returns true when we've landed
        rocket.nextState = LANDED;

      break;

    case LANDED:
      //do something better when SD card is good to go
      rocket.flight_complete();
      break;
      
  }

  //Get Ready for next loop
  rocket.currentState = rocket.nextState;
  delay(50);

  Serial.print("\nCurrent State: ");
  Serial.println(rocket.currentState);
  Serial.print("Next State: ");
  Serial.println(rocket.nextState);

  prev_Acc[0] = curr_Acc[0];
  prev_Acc[1] = curr_Acc[1];
  prev_Acc[2] = curr_Acc[2];

  prev_altitude = curr_altitude;
  
}
