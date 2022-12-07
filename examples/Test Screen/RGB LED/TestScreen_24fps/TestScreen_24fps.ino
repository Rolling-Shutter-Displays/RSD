/*
  TestScreen_24fps.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using a RGB led.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 2 Dic 2022
  by derfaq
  
  This example code is in the public domain.
*/

// Include RSD Library
#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

// RSD Objet
RSD rsd;

#define BWIDTH 32             // Byte resolution
#define WIDTH ((BWIDTH*8)-1)  // Lines resolution - 1

// Channel( pin , led mode , byte resolution ) Objet
Channel red( 2 , COMMON_CATHODE , BWIDTH );
Channel green( 3 , COMMON_CATHODE , BWIDTH );
Channel blue( 4 , COMMON_CATHODE , BWIDTH );

// Screen (RGB type) Objet 
Screen display( &red , &green , &blue );

void setup() {

  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( 24 , BWIDTH );

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
  
  // Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 6 );
  Serial.print(" Lines: ");
  Serial.println( BWIDTH*8 );
  
}

// Let's draw!
void draw() {

  display.clear();
  
  // Standarized order of the SMPTE/EBU color bar image : https://en.wikipedia.org/wiki/SMPTE_color_bars
  // from left to right, the colors are white, yellow, cyan, green, magenta, red, blue and black
  for( int i = 0 ; i <= WIDTH ; i++ ) {
    colour c = ( i * 8 ) / WIDTH;
    display.line( i , c );
  }

  // Grid resolution
  for( int i = 0 ; i < BWIDTH/2 ; i++ ) {
    if( 1 - i%2 ) display.line( i , WHITE );
  }

}
