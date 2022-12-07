/*
  Manantial.ino

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
  rsd.begin( 30 , BWIDTH );

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

  // Copy the screen
  white.copy( &white );

  // Randomly draw a couple of lines in the center
  if ( !random( 3 ) ) {
    white.line( WIDTH/2 - 2 );
    white.line( WIDTH/2 + 3 );
  }

  // Move to sides
  ShiftByOne( WIDTH/2 + 3 , WIDTH - 2 , &white );
  ShiftByOne( WIDTH/2 - 2 ,         2 , &white );
  
}

void ShiftByOne( int begin , int end , Channel *ch ) {

  if ( begin > end ) { // Shift Right
    int i = end;
    do {
      ( ch->get( i + 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i++;
    } while ( i < begin );
    ch->clear( begin );
  }

  if ( begin < end ) { // Shift Left
    int i = end;
    do {
      ( ch->get( i - 1 ) ) ? ch->line( i ) : ch->clear( i ) ;
      i--;
    } while ( i > begin );
    ch->clear( begin );
  }

}
