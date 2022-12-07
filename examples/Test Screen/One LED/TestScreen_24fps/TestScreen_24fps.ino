/*
  TestScreen_24fps.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using just one led.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 2 Dic 2022
  by derfaq
  
  This example code is in the public domain.
*/

// Include RSD Library
#include <RSD.h>

// RSD Objet
RSD rsd;

#define BWIDTH 32             // Byte resolution
#define WIDTH (BWIDTH*8 - 1)  // Lines resolution - 1

#define LED_PIN 13            // LED Pin

// Channel( pin , led mode , byte resolution ) Objet
Channel white( LED_PIN , COMMON_CATHODE , BWIDTH );

void setup() {

  // Setup of the RSD 
  rsd.begin( 24 , BWIDTH );    // begin( frequency , byte resolution )

  // Attach the channel (LED)
  rsd.attachChannel( &white ); // attachChannel( &channel )

  // Attach the draw function
  rsd.attachDraw( draw );      // attachDraw( callback function )

  // Initialize the serial port
  Serial.begin( 9600 );
  
}

void loop() {

  // Run the engine
  rsd.update();
  
  // Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 6 );
  Serial.print(" Lines: ");
  Serial.println( BWIDTH*8 );
  
}

// Let's draw!
void draw() {
  
  white.clear();
  
  white.fill( 0 , WIDTH/4 );

  for( int i = 1 ; i < (WIDTH/4) ; i++ ) {
    int val = (WIDTH/4)/i;
    if ( i&1 ) {
      white.clear( WIDTH/2 - val , WIDTH/2 + val );
    } else {
      white.fill( WIDTH/2 - val , WIDTH/2 + val );
    }
  }

  for( int i = 0 ; i < WIDTH/4 ; i++ ) {
    if( i&1 ) white.line( WIDTH - i );
  }
  
}
