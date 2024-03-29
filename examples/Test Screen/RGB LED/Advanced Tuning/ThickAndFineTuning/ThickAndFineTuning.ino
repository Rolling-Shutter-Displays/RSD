/*
  ThickAndFineTuning.ino

  This is a example sketch to show how to fine tuning the RSD.

  Open the serial monitor to tuning the RSD. Send a number between 1 and 100, and
  you can tuning it in the range of +-1 Hz (thick tuning). Use the potentiometer for fine tuning.
  The data in the serial monitor shows the frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Mar 2019
  modified 12 Apr 2020
  revised 6 Dic 2022
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

  int thick;
  int fine;

  // Thick tuning: Serial way
  if ( Serial.available() ) {
    thick = Serial.parseInt();
    thick = constrain( thick , 1 , 100 );
    thick = map( thick , 1 , 100 , rsd.getLowerThick() , rsd.getHigherThick() );
    rsd.setThick( thick );
  }
  
  // Fine tuning: Analog way
  fine = map( analogRead( A0 ), 0 , 1023 , rsd.getLowerFine() , rsd.getHigherFine() );
  rsd.setFine( fine );

  // Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 6 );
  Serial.print(" Lines: ");
  Serial.print( BWIDTH*8 );
  Serial.print(" , Thick: ");
  Serial.print( rsd.getThick() );
  Serial.print(" | L: ");
  Serial.print( rsd.getLowerThick() );
  Serial.print(" | H: ");
  Serial.print( rsd.getHigherThick() );
  Serial.print(" , Fine: ");
  Serial.print( rsd.getFine() );
  Serial.print(" | L: ");
  Serial.print( rsd.getLowerFine() );
  Serial.print(" | H: ");
  Serial.println( rsd.getHigherFine() );

}

// Let's draw!
void draw() {

  // Clear the screen
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
