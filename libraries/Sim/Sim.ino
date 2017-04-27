#include <SD.h>

#define CS 4 //chip select pin
#define DIN 12 // MISO data into teensy, out of sd pin
#define DOUT 11 // MOSI data out of teensy, into sd pin
#define CLK 13

#define BUF_LENGTH 100

unsigned int times[BUF_LENGTH];
float alts[BUF_LENGTH];

int lastPos = 0;
int bufPos = 0;

bool done = false;

void setup() {
  Serial.begin(9600);
  while(!Serial) {
    delay(100);
  }

  Serial.println("Beginning...");

  pinMode(10, OUTPUT); //neccesary for some reason?

  if(!SD.begin(CS)) Serial.println("SD failed to load");

  Serial.println("SD initialized");

  delay(2000);
}

void loop() {
  File test = SD.open("/test.bin", FILE_READ);

  unsigned int *lastTime;
  float *lastAlt;

  bufPos = 0;

  if(test.seek(lastPos) && test.available() >= 8*BUF_LENGTH) {
    while(bufPos < BUF_LENGTH && test.available() >= 8) {
      char data[4];
      test.read(data, 4);
      lastTime = (unsigned int *)data;
      times[bufPos] = *lastTime;

      test.read(data, 4);
      lastAlt = (float *)data;
      alts[bufPos] = *lastAlt;
      Serial.println(alts[bufPos]);

      bufPos++;
    }
    
    lastPos = test.position();
  } else {
    Serial.println("Failed to seek to position, probably at end of file");
    done = true;
  }

  test.close();

  bufPos = 0;

  Serial.println(times[bufPos]);
  
  while(times[BUF_LENGTH - 1] > micros()) {
    Serial.println(alts[bufPos]);
    if(bufPos < BUF_LENGTH) bufPos++;
    delay(10);
  }

  if(done) {
    while(true);
  }
}
