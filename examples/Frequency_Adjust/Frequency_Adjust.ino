#include <RSD.h>

RSD rsd;

void setup() {
  // put your setup code here, to run once:
  rsd.begin(9,10,11,COMMON_ANODE);

  //Test Screen for the Rolling Shutter Display
  rsd.beginDraw();
  
  for(int i = 0; i < WIDTH; i++) {
    colour color = ( i * 8 ) / WIDTH;
    rsd.putBar(color,i);
  }
  
  rsd.endDraw();

  Serial.begin(9600);
}

int incomingByte;

void loop() {
  // put your main code here, to run repeatedly:

  //Adjustment of frequency
  int thick = map(analogRead(A0),0,1024,rsd.getLowerTick(),rsd.getHigherTick());
  rsd.setTick(thick);
  int fine = map(analogRead(A1),0,1024,rsd.getLowerLastTick(),rsd.getHigherLastTick());
  rsd.setLastTick(fine);

  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital A forward screen:
    if (incomingByte == 'A') {
      rsd.changePos(true);
    }
    // if it's an S backward screen:
    if (incomingByte == 'S') {
      rsd.changePos(false);
    }
  }
  
  //Print the frequency parameters
  Serial.print(rsd.getTick());Serial.print(", ");
  Serial.print(rsd.getLastTick());Serial.print(", ");
  Serial.print(rsd.getPeriod());Serial.print(", ");
  Serial.print(rsd.getFrequency(),10);Serial.println("");
}
