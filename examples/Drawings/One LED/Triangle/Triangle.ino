/*
  Triangle.ino

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

int steps = 11;

// Let's draw!
void draw() {

  // Clear the screen
  white.clear();

  triangle( 0  , WIDTH/2 , steps , &white );
  triangle( WIDTH , WIDTH/2 , steps , &white );
  
}

// Triangle creator
void triangle( int begin , int end , int steps , Channel *ch ) {

    int interval = ( end - begin ) / steps;
    interval = abs( interval ) + 1;

    if ( begin < end ) {
      for ( int i = 0 ; i < steps ; i++ ) {
        fillSafe( begin , end , begin + interval * i , begin + interval * i + map( i , 0 , steps, 0 , interval ) , ch );
      }
    } else if( begin > end ) {
      for ( int i = 0 ; i < steps ; i++ ) {
        fillSafe( end , begin , begin - interval * i , begin - interval * i - map( i , 0 , steps, 0 , interval ) , ch );
      }
    } else {
      ch->lineSafe( begin );
    }
}

// fillSafe with boundaries
bool fillSafe( int y0 , int y1 , int x0 , int x1 , Channel *ch ) { 

  // Check boundaries
  if ( ( y0 < 0 ) || ( y1 < 0 ) ) return false;
  if ( ( y0 > WIDTH ) || ( y1 > WIDTH ) ) return false;
  
  if ( ( x0 < y0 ) && ( x1 < y0 ) ) return false;
  if ( ( x0 > y1 ) && ( x1 > y1 ) ) return false;
        
  if ( x1 > x0 ) {       
    if ( x1 > y1 ) x1 = y1;
    if ( x0 < y0 ) x0 = y0;
    
    ch->fill( x0 , x1 );
            
  } else if( x1 == x0 ) {    
      if ( ( x1 > y0 ) && ( x1 < y1 ) ) ch->line( x1 );
  } else {
    if ( x0 > y1 ) x0 = y1;
    if ( x1 < y0 ) x1 = y0;
    
    ch->fill( x0 , x1 );
  }
}
