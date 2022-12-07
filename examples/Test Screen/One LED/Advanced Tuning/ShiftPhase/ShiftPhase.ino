/*
  ShiftPhase.ino

  This is a example sketch to show how to shift the screen of the
  RSD.

  Use the potentiometer to tuning the RSD in the range of +-0.5 Hz arround the center frequency.
  To scroll the screen use the serial monitor with the + and - keys

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Mar 2019
  modified 12 Apr 2020
  revised 6 Dic 2022
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
  
  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( 30 , BWIDTH );

  // Attach the channel (LED)
  rsd.attachChannel( &white ); // attachChannel( &channel )

  // attachDraw( callback function ) | Attach the draw function
  rsd.attachDraw( draw );

  // Initialize the serial port
  Serial.begin( 9600 );
  
}

void loop() {

  // Run the engine
  rsd.update();

  // Tuning: Analog way
  float freq = 30 - ( ( (float)analogRead( A0 ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );

  // Serial console
  if ( Serial.available() ) {
    char p = Serial.read();

    if( p == '+' ) rsd.shiftPhase( 2 );
    if( p == '-' ) rsd.shiftPhase( -1 ); 
  }

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
