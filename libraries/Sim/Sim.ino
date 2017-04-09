#include "MPUSim.h"
#include "MPLSim.h"

MPU *mpu;
MPL *mpl;

void setup() {
  mpu = new MPU();
  mpl = new MPL();

  mpu->begin(0,0);
  mpl->begin(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
