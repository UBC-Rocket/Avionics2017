#include "MPUSim.h"
#include "MPLSim.h"

MPUSim *mpu;
MPLSim *mpl;

void setup() {
  mpu = new MPUSim();
  mpl = new MPLSim();

  mpu->begin(0,0);
  mpl->begin(0,0);
}

void loop() {
  // put your main code here, to run repeatedly:

}
