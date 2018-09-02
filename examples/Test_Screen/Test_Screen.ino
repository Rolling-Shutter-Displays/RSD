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
}

void loop() {
  // put your main code here, to run repeatedly:

}
