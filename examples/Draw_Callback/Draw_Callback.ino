#include <RSD.h>

RSD rsd;

void setup() {
  // put your setup code here, to run once:
  rsd.begin(9,10,11,COMMON_ANODE);
  rsd.attachDraw(draw);
}

int incomingByte;

void loop() {
  // put your main code here, to run repeatedly:
  rsd.update();
  
  //Adjustment of frequency
  int thick = map(analogRead(A0),0,1024,rsd.getLowerTick(),rsd.getHigherTick());
  rsd.setTick(thick);
  int fine = map(analogRead(A1),0,1024,rsd.getLowerLastTick(),rsd.getHigherLastTick());
  rsd.setLastTick(fine);

  //Adjustment of position screen
  if (Serial.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital A, bring forward screen:
    if (incomingByte == 'A') {
      rsd.changePos(true);
    }
    // if it's an S, bring backward screen:
    if (incomingByte == 'S') {
      rsd.changePos(false);
    }
  }
}

//This function will be called in the start of every new frame
void draw(){
  //Test Screen for the Rolling Shutter Display
  for(int i = 0; i < WIDTH; i++) {
    colour color = ( i * 8 ) / WIDTH;
    rsd.putBar(color,i);
  }
}

