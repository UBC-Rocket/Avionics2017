/*
 * UBC ROCKET
 */

#include <Rocket.h>
#include "Sim/MPUSim.h"
#include "Sim/MPLSim.h"
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

#define NUM_CHECKS        4     //each condition has to pass 5 times 

#define BURNOUT_TIME      4500
#define DROGUE_SIG_TIME   3000
#define PAYLOAD_SIG_TIME  3000
#define MAIN_SIG_TIME     5000  // TIRTH SECONDS
#define PRED_APOGEE_TIME  28000 //whatever time we think apogee will be at

/*
 * Ignition Circuit Definitions
 */
const int DROGUE_IGNITION_CIRCUIT = 7;  //pin to drogue chute ignition circuit
const int PAYLOAD_IGNITION_CIRCUIT = 8; //pin to nose cone ignition circuit
const int MAIN_IGNITION_CIRCUIT = 9;

Rocket rocket(STANDBY, STANDBY);
DataCollection dataCollector;
MPU *mpu1;
MPL *mpl1;

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
unsigned long deploy_drogue_time = 0;
unsigned long deploy_payload_time = 0;
unsigned long deploy_main_time = 0;

//temp for printing and TESTING
float curr_altitude;
float prev_altitude;
float curr_Acc[3];
float prev_Acc[3];

void setup() {

  Serial.begin(9600);
  delay(500);

  mpu1 = new MPU();
  mpl1 = new MPL();

  mpu1->begin(0, 0);
  mpl1->begin(0, 0);

  MPU *mpus[2] = {mpu1};
  MPL *mpls[1] = {mpl1};

  //Initialize pins for ignition circuits as outputs
  pinMode(DROGUE_IGNITION_CIRCUIT, OUTPUT);
  pinMode(PAYLOAD_IGNITION_CIRCUIT, OUTPUT);
  pinMode(MAIN_IGNITION_CIRCUIT, OUTPUT);

  dataCollector.begin(mpus, 1, mpls, 1);

  //do we wanna turn on an LED to confirm that we're all initialized?
}

/*
 * main loop function
 * uses a switch statement to switch between states
 * update next state after the switch statement
 */
void loop(){
  //Update The Data, Get next Best Guess at ALT ACC and VELO------------------
  dataCollector.collect();

  curr_time = millis();

  //make sure ignition pins stay low!
  if ( (deploy_drogue_time == 0) || (curr_time > deploy_drogue_time + DROGUE_SIG_TIME) )
    digitalWrite(DROGUE_IGNITION_CIRCUIT, LOW);
  if ( (deploy_payload_time == 0) || (curr_time > deploy_drogue_time + PAYLOAD_SIG_TIME) )
    digitalWrite(PAYLOAD_IGNITION_CIRCUIT, LOW);
  if ( (deploy_main_time == 0) || (curr_time > deploy_main_time + MAIN_SIG_TIME) )
    digitalWrite(MAIN_IGNITION_CIRCUIT, LOW);

  //PRINT THE DATA THIS IS FOR TESTING ONLY!
  Serial.println("Current time: " + (String)curr_time);

  curr_altitude = dataCollector.curr_alt;
  Serial.println("Current Altitude: " + (String)curr_altitude);

  dataCollector.popAccel(curr_Acc);

  //TODO: sqrt squared of these values??
  Serial.println("Current Acceleration X: " + (String)curr_Acc[0] + " Y: "+(String)curr_Acc[1] + " Z: " +(String)curr_Acc[2]);

  //update the current time one last time
  curr_time = millis();

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
      else if (curr_time > (launch_time + BURNOUT_TIME)){
        rocket.nextState = COASTING;
      }
      break;
      
    case COASTING:
      if( rocket.coasting(curr_Acc[2], coasting_count) > NUM_CHECKS ){
        rocket.nextState = TEST_APOGEE;
      }
      else if ((curr_time > launch_time + PRED_APOGEE_TIME) ){//TODO: make this agree with an altitude
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
      else if (curr_time > launch_time + PRED_APOGEE_TIME){//TODO: make this agree with an altitude
        rocket.nextState = DEPLOY_DROGUE;
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
        deploy_payload_time = millis();
        digitalWrite(PAYLOAD_IGNITION_CIRCUIT, HIGH); //send logic 1 to ignition circuit
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
