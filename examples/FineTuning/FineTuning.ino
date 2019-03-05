/*
  FineTuning.ino

  This is a example sketch to show how to fine tuning the RSD.

  Open the serial monitor to tuning the RSD. Send a number between 1 and 255, and
  you can tuning it in the range of +-1hz. Use the potentiometer to fine tuning.
  The data in the serial monitor shows the frequency of the RSD and the Tick*.

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

  Serial.begin( 9600 );
}

void loop() {
  //Run the engine
  rsd.update();
  
  // Fine tuning: Analog way
  int tick = map( analogRead( A0 ), 0 , 1023 , rsd.getLowerFine() , rsd.getHigherFine() );
  rsd.setFine( tick );

  /*
  //Tuning: Fix way
  // @frsd: 29.9819164276 , BWIDTH : 32 , tick: 2084 , fine: 2235 //Samsung S6
  rsd.setTick( 2084 );
  rsd.setFine( 2235 );
  */
}

//Tuning: Serial way
void serialEvent() {
   int tick = Serial.parseInt();
   if ( tick ) { 
    tick = constrain( tick , 1 , 255 );
    tick = map( tick , 1 , 255 , rsd.getLowerTick() , rsd.getHigherTick() );
    
    rsd.setTick( tick );
   }
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

  //Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 10 );
  Serial.print(" , BWIDHT: ");
  Serial.print(BWIDTH);
  Serial.print(" , tick: ");
  Serial.print( rsd.getTick() );
  Serial.print(" , fine: ");
  Serial.println( rsd.getFine() );
}
