/*
 * UBC ROCKET
 */

#include <Rocket.h>
#include <MPU.h>
#include "MPL.h"
#include <i2c_t3.h>
#include <TimerOne.h>

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
 * MPU Definitions
 */
#define ADDR 0x68       //i2c address of sensor
#define PWR_MNG 0x6B    //register address of pwr management
#define USR_CNTRL 0x6A  //register address of user control
#define GYRO_SETUP 0x1B //register address of gryoscope setup
#define ACCEL_SETUP 0x1C

#define GYRO_BASE 0x43
#define ACCEL_BASE 0x3B

#define OFFSET_X_HIGH 0x0
#define OFFSET_X_LOW 0x1

#define OFFSET_Y_HIGH 0x2
#define OFFSET_Y_LOW 0x3

#define OFFSET_Z_HIGH 0x4
#define OFFSET_Z_LOW 0x5

/*
 * Ignition Circuit Definitions
 */
const int DROGUE_IGNITION_CIRCUIT = 7;  //pin to drogue chute ignition circuit
const int PAYLOAD_IGNITION_CIRCUIT = 8; //pin to nose cone ignition circuit

/*
 * MPU Initialization
 */
int16_t gyro[3];
int16_t linx, liny, linz;
float cleanGyro[3];
unsigned long elapsed = 0;
MPU* myMPU;

/*
 * MPL Initializations
 */
int16_t x, y, z;
int16_t x_off, y_off, z_off;
float x_pos, y_pos, z_pos;
bool w = false;
MPL* PSensor;
/*
 * Kalman Filter Initializations
 */
 float Predict_Constant = 1.10;
 float Predict_Error;
 float Previous_Predict;
 float Altimeter_Error = 50;
 float Gain = 0.0;
/*
 * State Machine Initialization
 */



//size of each buffer


//the posistion in the buffers
int bufPosition = 0;
/*
 * The Buffer for each seonsor type
 */
//-----------------------
//MPU
//find these by trial and error
    float MPUSensorError = 0.75;
    float MPUSensorPredConst = 1.2;

//Buffers
//MPU1
 #define BUFFER_SIZE 100
  float previousErrorGyroReadings1 = 1;
  float previousErrorAccelReadings1 = 1;
  float previousErrorMagReadings1 = 1;

  int16_t gyroReadingsRAW1[BUFFER_SIZE][3];
  int16_t accelReadingsRAW1[BUFFER_SIZE][3];
  int16_t magReadingsRAW1[BUFFER_SIZE][3];

  int16_t gyroReadingsFILTER1[BUFFER_SIZE][3];
  int16_t accelReadingsFILTER1[BUFFER_SIZE][3];
  int16_t magReadingsFILTER1[BUFFER_SIZE][3];

//MPU2


  float previousErrorGyroReadings2 = 1;
  float previousErrorAccelReadings2 = 1;
  float previousErrorMagReadings2 = 1;

  int16_t gyroReadingsRAW2[BUFFER_SIZE][3];
  int16_t accelReadingsRAW2[BUFFER_SIZE][3];
  int16_t magReadingsRAW2[BUFFER_SIZE][3];

  int16_t gyroReadingsFILTER2[BUFFER_SIZE][3];
  int16_t accelReadingsFILTER2[BUFFER_SIZE][3];
  int16_t magReadingsFILTER2[BUFFER_SIZE][3];


//MPU3


  float previousErrorGyroReadings3 = 1;
  float previousErrorAccelReadings3 = 1;
  float previousErrorMagReadings3 = 1;

  int16_t gyroReadingsFILTER3[BUFFER_SIZE][3];
  int16_t accelReadingsFILTER3[BUFFER_SIZE][3];
  int16_t magReadingsFILTER3[BUFFER_SIZE][3];

  int16_t gyroReadingsRAW3[BUFFER_SIZE][3];
  int16_t accelReadingsRAW3[BUFFER_SIZE][3];
  int16_t magReadingsRAW3[BUFFER_SIZE][3];


//-----------------------
//ALT
//find these by trial and error
    float MPLSensorError = 4;
    float MPLSensorPredConst = 1.000;

//Buffers
//ATL1
  float previousErrorAltReadings1 = 1;
  float previousFilterAltReadings1;
  float alt1RAW[BUFFER_SIZE];
  float alt1FILTER[BUFFER_SIZE];


void loadnewestRawValues(){
  if (bufPosition >= BUFFER_SIZE){
    for(int i = 0; i < BUFFER_SIZE; i++){
      alt1RAW[i] = 0.0;
      alt1FILTER[i] = 0.0;
    }
    bufPosition = 0;
  }
  //read alts
    alt1RAW[bufPosition] = PSensor->readAltitude();
  //read gyros
    /*myMPU->readGyro(gyro);
    myMPU->cleanGyro(cleanGyro, gyro);
    float gyro1rawx = cleanGyro[0];
    float gyro1rawy = cleanGyro[1];
    float gyro1rawz = cleanGyro[2];
    */
}
void filternewestValues(){
  //read alts
  if(previousFilterAltReadings1 == NULL || previousFilterAltReadings1 == 0.0){
    previousFilterAltReadings1 = alt1RAW[bufPosition];
  }
    alt1FILTER[bufPosition] = filterData(&previousErrorAltReadings1, MPLSensorError, MPLSensorPredConst, previousFilterAltReadings1, alt1RAW[bufPosition]);
    previousFilterAltReadings1 = alt1FILTER[bufPosition];
  //read MPUs


}



Rocket rocket(RESET, RESET);

/*
 * maybe sensor related setup?
 */

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize MPU
  myMPU = new MPU(0, ADDR);
  myMPU->initGyro(250);

  // Initialize MPL
  PSensor = new MPL(w);
  PSensor->init();
  PSensor->setGround();

  //Initialize pins for ignition circuits as outputs
  pinMode(DROGUE_IGNITION_CIRCUIT, OUTPUT);
  pinMode(PAYLOAD_IGNITION_CIRCUIT, OUTPUT);


}

/*
 * main loop function
 * uses a switch statement to switch between states
 * update next state after the switch statement
 */
void loop(){
  //make sure ignition pins stay low!
  digitalWrite(DROGUE_IGNITION_CIRCUIT, LOW);
  digitalWrite(PAYLOAD_IGNITION_CIRCUIT, LOW);


 loadnewestRawValues();
 filternewestValues();
  /*
   * Print MPL Data
   */
  //altitude = PSensor->readAltitude();

  Serial.println("MPL Data: ");
  Serial.print ("Offset: ");
  Serial.println(PSensor->getOffset());
  Serial.print("Current Raw Altitude: ");
  Serial.println(alt1RAW[bufPosition]);
  Serial.print("Current Filtered Altitude: ");
  Serial.println(alt1FILTER[bufPosition]);
  /*
   * Print MPU Data
   */
  myMPU->readGyro(gyro);
  myMPU->cleanGyro(cleanGyro, gyro);
  Serial.println("Gyro Data: ");
  Serial.println(cleanGyro[0]);
  Serial.println(cleanGyro[1]);
  Serial.println(cleanGyro[2]);

  /*
   *  Print Current and Next State
   */
  Serial.print("Current State: ");
  Serial.println(rocket.currentState);
  Serial.print("Next State: ");
  Serial.println(rocket.nextState);

  switch (rocket.currentState){
    case RESET:
      rocket.reset();
      rocket.nextState = STANDBY;
      break;
    case STANDBY:
      if (rocket.standby())
        rocket.nextState = POWERED_ASCENT;
      break;
    case POWERED_ASCENT:
      rocket.powered_ascent();
      rocket.nextState = COASTING;
      break;
    case COASTING:
      rocket.coasting();
      rocket.nextState = TEST_APOGEE;
      break;
    case TEST_APOGEE:
      if (rocket.test_apogee()){
        rocket.nextState = DEPLOY_DROGUE;
        break;
      }
      else{
        rocket.nextState = TEST_APOGEE;
        break;
      }
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
      rocket.initial_descent();
      rocket.nextState = DEPLOY_MAIN;
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
  bufPosition++;
  delay(50);
}

float filterData(float* predictionError, float sensorError, float predictConstant, float previousPredict, float reading) {
  //Predict:
  float filteredReading = predictConstant*previousPredict;
  *predictionError = predictConstant * (*predictionError) * predictConstant;
  Serial.print("Predict Stage: Filtered:");
  Serial.println(filteredReading);
  //Update:
  float Gain = (*predictionError)/(*predictionError + sensorError);
  filteredReading = filteredReading + Gain * (reading - filteredReading);
  *predictionError = (1-Gain) * (*predictionError);
  Serial.print("Update Stage: Error:");
  Serial.println(filteredReading);
  return filteredReading;
}
