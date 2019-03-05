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
#define WIDTH (BWIDTH*8 - 1)  //Line resolution

//Channel( pin , led mode , byte resolution )
Channel white( 8 , COMMON_CATHODE , BWIDTH );

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );   //begin( frequency , byte resolution )
  
  rsd.attachChannel( white ); //attachChannel( channel )
  
  rsd.attachDraw( draw );     //attachDraw( callback function )

  Serial.begin( 9600 );
}

void loop() {
  //Run the engine
  rsd.update();

  /*
  // Tuning: Analog way
  int tick = map( analogRead( A0 ), 0 , 1023 , rsd.getLowerTick() , rsd.getHigherTick() );
  rsd.setTick( tick );
  rsd.setFine( tick ); 
  */

  /*
  //Tuning: Fixed way
  //@frsd: 29.9819164276 , BWIDTH: 32 , tick: 2084 , fine: 2235 // Samsung S6 Edge 
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
    rsd.setFine( tick );
   }
}

//Let's draw!
void draw() {
  
  white.fill( 0 , WIDTH/4 );

  for( int i = 1 ; i < (WIDTH/4) ; i++ ) {
    int val = (WIDTH/4)/i;
    if ( i%2 ) {
      white.clear( WIDTH/2 -val, WIDTH/2 + val);
    } else {
      white.fill( WIDTH/2 - val, WIDTH/2 + val);
    }
  }

  for( int i = 0 ; i < WIDTH/4 ; i++ ) {
    if( i%2 ) white.line( WIDTH - i );
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
