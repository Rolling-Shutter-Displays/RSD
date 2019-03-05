/*
  ShiftPhase.ino

  This is a example sketch to show how to shift the screen of the
  RSD once you have the frequency, for this last use the FineTuning example.

  Use the potentiometer to shift the phase of the RSD.

  *For full explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Mar 2019
  by derfaq
  
  This example code is in the public domain.
*/

#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

RSD rsd;
#define BWIDTH 32
#define WIDTH ((BWIDTH*8)-1) 

Channel red( 11 , COMMON_CATHODE , BWIDTH );
Channel green( 3 , COMMON_CATHODE , BWIDTH );
Channel blue( 6 , COMMON_CATHODE , BWIDTH );

Screen display( &red , &green , &blue );

void setup() {
  //Setup of the RSD
  rsd.begin( 30 , BWIDTH );
  
  rsd.attachChannel( red );
  rsd.attachChannel( green );
  rsd.attachChannel( blue );

  rsd.attachDraw( draw );
}

void loop() {
  //Run the engine
  rsd.update();
  
  //Tuning: Fixed way
  // @frsd: 29.9819164276 , BWIDTH : 32 , tick: 2084 , fine: 2235 //Samsung S6
  rsd.setTick( 2084 );
  rsd.setFine( 2235 );
}

//Let's draw!
void draw() {
  display.clear();
  
  //Standarized order of the SMPTE/EBU color bar image : https://en.wikipedia.org/wiki/SMPTE_color_bars
  //from left to right, the colors are white, yellow, cyan, green, magenta, red,  blue and black
  for( int i = 0 ; i <= WIDTH ; i++ ) {
    colour c = ( i * 8 ) / WIDTH;
    display.line( i , c );
  }

  //Grid resolution
  for( int i = 0 ; i < BWIDTH/2 ; i++ ) {
    if( 1 - i%2 ) display.line( i , WHITE );
  }

  //Shift phase: Analog way
  rsd.shiftPhase( map( analogRead(0) , 0 , 1023 , -1 , +2 ) );

}
