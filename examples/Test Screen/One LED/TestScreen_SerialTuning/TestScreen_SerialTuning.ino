/*
  TestScreen_SerialTuning.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using just one led.

  For tuning, open the serial monitor, type the frequency and hit enter. The data shows the
  current frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 24 Feb 2019
  modified 12 Apr 2020
  revised 2 Dic 2022
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
  rsd.begin( 30 , BWIDTH );    // begin( frequency , byte resolution )

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
  
  // Tuning: Serial way
  if ( Serial.available() ) {
    float freq = Serial.parseFloat();
    if ( freq != 0 ) rsd.setFrequency( freq );
  }
  
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
