/*
  Clock.ino

  This is a example sketch to show how to use the RSD library primitives to draw in a Rolling Shutter Display
  using just one RGB led.

  For tuning, open the serial monitor, type the frequency and hit enter. The data shows the
  current frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Dic 2022
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
colour palette[4] = { GREEN , RED , YELLOW , WHITE };

// Let's draw!
void draw() {

  // Clear the screen
  display.clear();
  
  // if fps 30
  // Second
  static bool inv;
  if( !(frameCount%30) ) inv = !inv;
  if( inv ) {
    display.fill( 0 , WIDTH/3 , palette[0] );
  } else {
    display.clear( 0 , WIDTH/3 );
  }
  
  // Minute
  display.fill( WIDTH/3 , WIDTH/3 + map( frameCount%1800 , 0 , 1800 , 0 , WIDTH/3 ) , palette[1] );

  // Hour
  display.fill( WIDTH*2/3 , WIDTH*2/3 + map( frameCount%108000 , 0 , 108000 , 0 , WIDTH/3 ) , palette[2] );
  
  // Boundaries status
  if( frameCount&4 ) {
    display.line( 0 , palette[3] ); display.line( WIDTH/3 , palette[3] ); display.line( WIDTH*2/3 , palette[3] );
  } else {
    display.clear( 0 ); display.clear( WIDTH/3 ); display.clear( WIDTH*2/3 );
  }
  
}
