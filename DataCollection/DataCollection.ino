#include <i2c_t3.h>
#include <MPL.h>
#include <DataCollection.h>


bool w = false;

MPL* PSensor;
DataCollection* collector;
int i;

void setup() {
  delay(1000);

  PSensor = new MPL(w);
  collector = new DataCollection();

  Serial.begin(9600);
  i = 0;
  delay(500);

   
}

void loop() {
  delay(04);
  if(i > 250){
    i=0;
    Serial.println(i);
  }
  i++;
  
  collector->filterData(PSensor);

}



