/*
  Filling.ino

  This is part of a series of sketch examples to show the primitives for drawing 
  of the RSD library.

  Use the potentiometer to tuning the RSD in the range of +-1hz.

  *For full explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 9 Mar 2019
  by derfaq
  
  This example code is in the public domain.
*/

#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

RSD rsd;
#define BWIDTH 32
#define WIDTH ((BWIDTH*8)-1) 

Channel red( 11 , COMMON_ANODE , BWIDTH );
Channel green( 3 , COMMON_ANODE , BWIDTH );
Channel blue( 6 , COMMON_ANODE , BWIDTH );

Screen display( &red , &green , &blue );

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );
  
  rsd.attachChannel( &red );
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

  rsd.attachDraw( draw );
}

void loop() {
  //Run the engine
  rsd.update();
  
  //Tuning: Analog way
  int tick = map( analogRead(A0) , 0 , 1023 , rsd.getLowerTick() , rsd.getHigherTick() );
  rsd.setTick( tick );
  rsd.setFine( tick );
}


//Let's draw!
void draw() {
  display.clear(); 

  red.fill( 0 , frameCount % WIDTH );
  green.fill( 0 , ( ( uint16_t )( (float) frameCount * 1.7 ) % WIDTH ) );
  blue.fill( WIDTH - ( ( uint16_t )( (float) frameCount * 2.1 ) % WIDTH ) ,  WIDTH );
  
}
