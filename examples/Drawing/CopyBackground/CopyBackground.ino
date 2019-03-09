/*
  CopyBackground.ino

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
  
  //Tuning: Analog way
  int tick = map( analogRead(A0) , 0 , 1023 , rsd.getLowerTick() , rsd.getHigherTick() );
  rsd.setTick( tick );
  rsd.setFine( tick );
}

colour palette[3] =  { BLACK , BLUE , MAGENTA };

//Let's draw!
void draw(){
  //Copy background
  red.copy( &red );
  green.copy( &green );
  blue.copy( &blue );
  
  ShiftByOne( 130 , 255 );
  ShiftByOne( 115 , 0 );
  colour c = palette[(int)random(3)];
  display.line( 115 , c );
  display.line( 130 , c );

}

void ShiftByOne( int begin , int end ) {
  
  if( begin > end ) { //Shift Right
    int i = end;
    do {
      display.line( i , display.get( i + 1 ) );
      i++;
    } while ( i < begin );
    display.line( begin , BLACK );
  }

  if( begin < end ) { //Shift Left
    int i = end;
    do {
      display.line( i , display.get( i - 1 ) );
      i--;
    } while ( i > begin );
    display.line( begin ,  BLACK );
  }
  
}
