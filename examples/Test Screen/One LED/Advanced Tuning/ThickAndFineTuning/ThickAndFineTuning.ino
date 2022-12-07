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
