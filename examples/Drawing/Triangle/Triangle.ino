/*
  Triangle.ino

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
  display.fill( BLUE );
  display.fill( WIDTH/2 - 10 , WIDTH/2 + 10 , RED );
  triangle( 0  , WIDTH/2 , 10 , RED );
  triangle( WIDTH , WIDTH/2 , 10 , RED );
}

void triangle( int begin , int end , int steps , colour c ) {
  int interval;
  if( begin <= end ) {
    interval = ( end - begin ) / steps;
    for ( uint8_t i = 1 ; i < steps ; i++ ) {
      display.fill( begin + interval*i , begin + interval*i + map( i , 1 , steps, 0 , interval ) , c ); 
    }
  } else {
    interval = ( begin - end ) / steps;
    for ( uint8_t i = 1 ; i < steps ; i++ ) {
      display.fill( begin - interval*i , begin - interval*i - map( i , 1 , steps, 0 , interval ) , c ); 
    }
  }
}
