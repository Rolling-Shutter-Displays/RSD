/*
  Zoom-In.ino

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

// Complementary colours
colour palette[3][2] = { { RED , CYAN } , { GREEN , MAGENTA } , { BLUE , YELLOW } };

// Let's draw!
void draw() {

  static int w = 1;
  static int p = 0;
  
  int c = 0;
  int i = 0;
  do {
    i++;
    c = 1 - c;
  } while( fillSafe( 0 , WIDTH/2 , (i - 1)*w , i*w , palette[p][c] ) );

  c = 1;
  i = 0;
  do {
    i++;
    c = 1 - c;
  } while( fillSafe( WIDTH/2 , WIDTH , WIDTH - (i - 1)*w , WIDTH - i*w , palette[p][c] ) );

  if( w < WIDTH/2 ) {
    w++ ;
  } else { 
    w = 1;
    ( p < 2 ) ? p++ : p = 0;
  }
  
}

// fillSafe with boundaries
bool fillSafe( int y0 , int y1 , int x0 , int x1 , colour c ) { 

  //Check boundaries
  if ( ( y0 < 0 ) || ( y1 < 0 ) ) return false;
  if ( ( y0 > WIDTH ) || ( y1 > WIDTH ) ) return false;
  
  if ( ( x0 < y0 ) && ( x1 < y0 ) ) return false;
  if ( ( x0 > y1 ) && ( x1 > y1 ) ) return false;
        
  if ( x1 > x0 ) {       
    if ( x1 > y1 ) x1 = y1;
    if ( x0 < y0 ) x0 = y0;
    
    do {
      display.line( x1 , c );
      x1--;
    } while( x1 > x0 );      
    display.line( x0 , c );
            
  } else if( x1 == x0 ) {    
    if ( ( x1 > y0 ) && ( x1 < y1 ) ) display.line( x1 , c );
    
  } else {
    if ( x0 > y1 ) x0 = y1;
    if ( x1 < y0 ) x1 = y0;
    
    do {
      display.line( x0 , c );
      x0--;
    } while( x0 > x1 );        
    display.line( x1 , c );   
  }
  return true;
}
