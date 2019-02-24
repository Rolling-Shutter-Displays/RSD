/*
  TestScreen_Mono.ino

  This is a example sketch to show how to use the RSD library, to make a Rolling Shutter Display
  using just one led.

  Open the serial monitor, to tuning the RSD. The data shows the
  frequency of the RSD and the Tick*. Send a number between 1 and 255, and
  you can tuning the RSD in the range of +-1hz.

  *For full explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 24 Feb 2019
  by derfaq
  
  This example code is in the public domain.
*/

#include <RSD.h>
RSD rsd;

#define BWIDTH 32         //Byte resolution
#define WIDTH (BWIDTH*8)  //Line resolution

//Channel( pin , led mode , byte resolution )
Channel White( 8 , COMMON_CATHODE , BWIDTH );

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );   //begin( frequency , byte resolution )
  rsd.attachChannel( White ); //attachChannel( channel )
  rsd.attachDraw( draw );     //attachDraw( callback function )

  Serial.begin( 9600 );
}

void loop() {
  //Update the RSD
  rsd.update();
}

//Tuning the RSD: Serial way
void serialEvent() {
   int tick = Serial.parseInt();
   if ( tick ) { 
    tick = constrain( tick , 1 , 255 );
    tick = map( tick , 1 , 255 , rsd.getLowerTick() , rsd.getHigherTick() );
    
    rsd.setTick( tick );
    rsd.setLastTick( tick );
   }
}

//Let's draw!
void draw() {
  
  White.fill( 0 , WIDTH/4 );

  for( int i = 1 ; i < (WIDTH/4) ; i++ ) {
    int val = (WIDTH/4)/i;
    if ( i%2 ) {
      White.clear( WIDTH/2 -val, WIDTH/2 + val);
    } else {
      White.fill( WIDTH/2 - val, WIDTH/2 + val);
    }
  }

  for( int i = 0 ; i < WIDTH/4 ; i++ ) {
    if( i%2 ) White.line( WIDTH - 1 - i );
  }

  //Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 10 );
  Serial.print(" , tick: ");
  Serial.println( rsd.getTick() );
}
