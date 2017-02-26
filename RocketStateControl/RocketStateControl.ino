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
#define RESET           0
#define STANDBY         1
#define POWERED_ASCENT  2
#define COASTING        3
#define TEST_APOGEE     4
#define DEPLOY_DROGUE   5
#define DEPLOY_PAYLOAD  6
#define INITIAL_DESCENT 7
#define DEPLOY_MAIN     8
#define FINAL_DESCENT   9

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

float curr_altitude;

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


dataCollector->update();
  Serial.print("Current State: ");
  Serial.println(rocket.currentState);
  Serial.print("Next State: ");
  Serial.println(rocket.nextState);

  //make sure ignition pins stay low!
  digitalWrite(DROGUE_IGNITION_CIRCUIT, LOW);
  digitalWrite(PAYLOAD_IGNITION_CIRCUIT, LOW);

  curr_time = millis();


    
  curr_altitude = dataCollector->currentALTITUDE;

  Serial.println("Current Altitude: " + (String)curr_altitude);
  
  Serial.println("Current time: " + (String)curr_time);

//  //keeping track of MPU's error codes, would ideally be 0
//  accel_error_code = myMPU.readAccel(raw_accel_data); //this is also the wrong number
//  //Serial.println("init mag " + (String)myMPU.initMag());
//  mag_error_code = myMPU.readMag(raw_mag_data);




  
//  Serial.println("Current Acceleration: " + (String)raw_accel_data[0]);
//  Serial.println("Accel Error Code: " + (String)accel_error_code);            //ideally 0
//  
//  Serial.println("Current Mag shit: " + (String)raw_mag_data[0]);
//  Serial.println("Mag Error Code: " + (String)mag_error_code);                //ideally 0
  

  //loadnewestRawValues();
  //filternewestValues();
  
  switch (rocket.currentState){
    case RESET:
      rocket.reset();
      rocket.nextState = STANDBY;
      break;
      
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
      rocket.final_descent();
      rocket.nextState = FINAL_DESCENT; //is there a better idea for here??
      break;
      
  }
  rocket.currentState = rocket.nextState;
  //prev_accel = curr_accel;
  
  //bufPosition++;
  delay(50);
}
/*
float filterData(float* predictionError, float sensorError, float predictConstant, float previousPredict, float reading) {
  //Predict:
  float filteredReading = predictConstant*previousPredict;
  *predictionError = predictConstant * (*predictionError) * predictConstant;
  //Serial.print("Predict Stage: Filtered:");
  //Serial.println(filteredReading);
  //Update:
  float Gain = (*predictionError)/(*predictionError + sensorError);
  filteredReading = filteredReading + Gain * (reading - filteredReading);
  *predictionError = (1-Gain) * (*predictionError);
  //Serial.print("Update Stage: Error:");
  //Serial.println(filteredReading);
  return filteredReading;
}*/

  /*
   * Print MPL Data
   */
  //altitude = PSensor->readAltitude();
/*
  Serial.println("MPL Data: ");
  Serial.print ("Offset: ");
  Serial.println(PSensor->getOffset());
  Serial.print("Current Raw Altitude: ");
  Serial.println(alt1RAW[bufPosition]);
  Serial.print("Current Filtered Altitude: ");
  Serial.println(alt1FILTER[bufPosition]); */
  /*
   * Print MPU Data
   */
   /*
  myMPU->readGyro(gyro);
  myMPU->cleanGyro(cleanGyro, gyro);
  Serial.println("Gyro Data: ");
  Serial.println(cleanGyro[0]);
  Serial.println(cleanGyro[1]);
  Serial.println(cleanGyro[2]);
*/
  /*
   *  Print Current and Next State
   */
   /*
  Serial.print("Current State: ");
  Serial.println(rocket.currentState);
  Serial.print("Next State: ");
  Serial.println(rocket.nextState);
  */
