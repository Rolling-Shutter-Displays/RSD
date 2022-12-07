/*
  Clock.ino

  This is a example sketch to show how to use the RSD library primitives to draw in a Rolling Shutter Display
  using just one led.

  For tuning, open the serial monitor, type the frequency and hit enter. The data shows the
  current frequency of the RSD.

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

  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( 30.05 , BWIDTH );

  /// attachChannel( &channel ) | Attach the channel (LED)
  rsd.attachChannel( &white );

  // attachDraw( callback function ) | Attach the draw function
  rsd.attachDraw( draw );

  // Initialize the serial port
  Serial.begin( 9600 );
  
}

void loop() {

  // Run the engine
  rsd.update();
  
  // Tuning: Serial way
  if ( Serial.available() ) {
    float freq = Serial.parseFloat();
    if ( freq != 0 ) {
      rsd.setFrequency( freq );
      
      Serial.print( "Target freq: " ); 
      Serial.print( freq , 6 );
      Serial.print( " RSD freq: " ); 
      Serial.println( rsd.getFrequency() , 6 );
    }
  }

  // Tuning: Analog way
  /*
  float freq = 30 - ( ( (float)analogRead( A0 ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );
  */
  
}

// Let's draw!
void draw() {

  // Clear the screen
  white.clear();
  
  // if fps 30
  // Second
  white.fill( 0 , map( frameCount%30 , 0 , 30 , 0 , WIDTH/3 ) );
  
  static bool inv;
  if( !(frameCount%30) ) inv = !inv;
  if( inv ) white.invert( 0 , WIDTH/3 );
  
  // Minute
  white.fill( WIDTH/3 , WIDTH/3 + map( frameCount%1800 , 0 , 1800 , 0 , WIDTH/3 ) );

  // Hour
  white.fill( WIDTH*2/3 , WIDTH*2/3 + map( frameCount%108000 , 0 , 108000 , 0 , WIDTH/3 ) );

  // Boundaries status
  if( frameCount&4 ) {
    white.line( 0 ); white.line( WIDTH/3 ); white.line( WIDTH*2/3 );
  } else {
    white.clear( 0 ); white.clear( WIDTH/3 ); white.clear( WIDTH*2/3 );
  }

}
