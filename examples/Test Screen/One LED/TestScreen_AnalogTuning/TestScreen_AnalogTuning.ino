/*
  TestScreen_SerialTuning.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using just one led and one potentiometer.

  Use the potentiometer to tuning the RSD in the range of +-0.5 Hz arround the center frequency.
  The data in the serial monitor shows the current frequency of the RSD.
  
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
#define POTE_PIN A0           // Potentiometer Pin

// Channel( pin , led mode , byte resolution ) Objet
Channel white( LED_PIN , COMMON_CATHODE , BWIDTH );

float centerFrequency = 30;   // Center frequency (fps)

void setup() {

  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( centerFrequency , BWIDTH );

  // attachChannel( &channel ) | Attach the channel (LED)
  rsd.attachChannel( &white );

  // attachDraw( callback function ) | Attach the draw function
  rsd.attachDraw( draw );

  // Initialize the serial port
  Serial.begin( 9600 );
  
}

void loop() {

  // Run the engine
  rsd.update();

  // Tuning: Analog way
  float freq = centerFrequency - ( ( (float)analogRead( POTE_PIN ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );
  
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
