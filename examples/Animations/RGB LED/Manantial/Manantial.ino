/*
  Manantial.ino

  This is a example sketch to show how to use the RSD library primitives to draw in a Rolling Shutter Display
  using just one RGB led.

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

// Channel( pin , led mode , byte resolution ) Objet
Channel red( 2 , COMMON_CATHODE , BWIDTH );
Channel green( 3 , COMMON_CATHODE , BWIDTH );
Channel blue( 4 , COMMON_CATHODE , BWIDTH );

// Screen (RGB type) Objet 
Screen display( &red , &green , &blue );

void setup() {

  // begin( frequency , byte resolution ) | Setup of the RSD
  rsd.begin( 30.05 , BWIDTH );

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

// Palette
colour palette[4] = { BLUE , MAGENTA , CYAN , BLACK };

// Let's draw!
void draw() {

  // Copy the screen
  display.copyBackground();

  // Randomly chose a colour
  colour c = palette[ random(4) ];
  
  // Randomly draw a couple of lines in the center
  display.line( WIDTH/2 - 2 , c );
  display.line( WIDTH/2 + 3 , c );

  // Move to sides
  ShiftByOne( WIDTH/2 + 3 , WIDTH - 2 );
  ShiftByOne( WIDTH/2 - 2 ,         2 );
  
}

void ShiftByOne( int begin , int end ) {
  
  if ( begin > end ) { // Shift Right
    int i = end;
    do {
      display.line( i , display.get( i + 1 ) );
      i++;
    } while ( i < begin );
    display.clear( begin );
  }

  if ( begin < end ) { // Shift Left
    int i = end;
    do {
      display.line( i , display.get( i - 1 ) );
      i--;
    } while ( i > begin );
    display.clear( begin );
  }

}
