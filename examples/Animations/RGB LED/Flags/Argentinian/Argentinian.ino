/*
  Argentinian.ino

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

  /// attachChannel( &channel ) | Attach the channels (LEDs)
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


colour palette[7] =  { BLACK , CYAN , WHITE , YELLOW , WHITE , CYAN , BLACK };
const int p[8] =  { 0 , WIDTH/8 , WIDTH*3/8 , WIDTH/2 - 3 , WIDTH/2 + 3 , WIDTH*5/8 , WIDTH*7/8 , WIDTH };
int diff[7];
int partition[8];

// Let's draw!
void draw() {
  
  // Clear the screen
  display.clear(); 

  for( int i = 0 ; i < 7 ; i++ ) {
    diff[i] = ( WIDTH/14 ) * sin(( TWO_PI * ( frameCount%120 ) / 120 ) + TWO_PI*i/14 );
  }

  partition[0] = p[0];
  for( int i = 0 ; i < 7 ; i++ ) {
    partition[ i+1 ] = p[i+1] + diff[i];
  }
  partition[7] = p[7];

  for( int i = 0 ; i < 7 ; i++ ) {
    display.fill( partition[i] , partition[i+1] , palette[i] );
  }
  
}
