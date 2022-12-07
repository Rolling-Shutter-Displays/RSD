/*
  Paint.ino

  This is a example sketch to show how to use the RSD library primitives to draw in a Rolling Shutter Display
  using just one RGB led.

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

char commands[8] = { 'r' , 'g' , 'b' , 'm' , 'c' , 'y' , 'w' , 0x20 };
colour color[8] = { RED , GREEN , BLUE , MAGENTA , CYAN , YELLOW , WHITE , BLACK };
colour c;
colour cursorC;
int pos, oldPos;
bool newLine = false;

void loop() {

  // Run the engine
  rsd.update();
  
  // Serial console
  if ( Serial.available() ) {
    char p = Serial.read();
    
    for( int i = 0 ; i < 8 ; i++ ) {
      if( p == commands[i] ) {  c = color[i]; pos++; newLine = true; }
    }

    if( p == 'f' ) {
      float freq = Serial.parseFloat();
      if ( freq != 0 ) {
        rsd.setFrequency( freq );
      
        Serial.print( "Target freq: " ); 
        Serial.print( freq , 6 );
        Serial.print( " RSD freq: " ); 
        Serial.println( rsd.getFrequency() , 6 );
      }
    }
    
    if( p == 'a' ) pos--;
    if( p == 'd' ) pos++;

    if( p == '+' ) rsd.shiftPhase( 2 );
    if( p == '-' ) rsd.shiftPhase( -1 );
    
    if( pos > WIDTH ) pos = 0;
    if( pos < 0 ) pos = WIDTH;
    
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
  display.copyBackground();

  if( newLine ) {
    display.line( pos , c );
    newLine = false;
  }

  // Cursor
  display.line( oldPos , cursorC );
  
  cursorC = display.get( pos );
  oldPos = pos;
  ( frameCount%30 > 15 ) ? display.line( pos , WHITE ) : display.line( pos , BLACK );
  
}
