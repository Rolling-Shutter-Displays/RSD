/*
  ShiftPhase.ino

  This is a example sketch to show how to shift the screen of the
  RSD.

  Use the potentiometer to tuning the RSD in the range of +-0.5 Hz arround the center frequency.
  To scroll the screen use the serial monitor with the + and - keys

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Mar 2019
  modified 12 Apr 2020
  by derfaq
  
  This example code is in the public domain.
*/

// Include RSD Library
#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

// RSD Objet
RSD rsd;

#define BWIDTH 32              // Byte resolution
#define WIDTH ((BWIDTH*8)-1)   // Lines resolution - 1

// Channel( pin , led mode , byte resolution ) Objet
Channel red( 2 , COMMON_CATHODE , BWIDTH );
Channel green( 3 , COMMON_CATHODE , BWIDTH );
Channel blue( 4 , COMMON_CATHODE , BWIDTH );

// Screen (RGB type) Objet 
Screen display( &red , &green , &blue );

void setup() {
  
  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( 30 , BWIDTH );

  // attachChannel( &channel ) | Attach the channels (LEDs)
  rsd.attachChannel( &red );
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

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

  // Clear the screen
  display.clear();
  
  // Standarized order of the SMPTE/EBU color bar image : https://en.wikipedia.org/wiki/SMPTE_color_bars
  // from left to right, the colors are white, yellow, cyan, green, magenta, red,  blue and black
  for( int i = 0 ; i <= WIDTH ; i++ ) {
    colour c = ( i * 8 ) / WIDTH;
    display.line( i , c );
  }

  // Grid resolution
  for( int i = 0 ; i < BWIDTH/2 ; i++ ) {
    if( 1 - i%2 ) display.line( i , WHITE );
  }

}
