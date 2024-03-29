/*
  Cube.ino

  This is a example sketch to show how to use the RSD library primitives to draw in a Rolling Shutter Display
  using just one RGB led.

  For tuning, open the serial monitor, type the frequency and hit enter. The data shows the
  current frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 6 Dic 2022
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

// Tables with cube animation 
const PROGMEM uint8_t cube_r[60][32] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x00,0xFF,0x1F,0x00,0xFC,0xFF,0x03,0x00,0xF8,0xFF,0x03,0x00,0xFE,0x3F,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x00,0xFF,0x3F,0x00,0xF0,0xFF,0x0F,0x00,0xE0,0xFF,0x0F,0x00,0xFC,0x7F,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x07,0x00,0xFE,0x7F,0x00,0xC0,0xFF,0x7F,0x00,0x00,0xFF,0x3F,0x00,0xF0,0x7F,0x00,0xFC,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x07,0x00,0xFC,0xFF,0x00,0x80,0xFF,0xFF,0x01,0x00,0xFC,0xFF,0x00,0xE0,0xFF,0x00,0xF8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x0F,0x00,0xFC,0xFF,0x01,0x00,0xFE,0xFF,0x07,0x00,0xE0,0xFF,0x03,0xC0,0xFF,0x01,0xF8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x0F,0x00,0xF8,0xFF,0x03,0x00,0xF8,0xFF,0x1F,0x00,0x00,0xFF,0x0F,0x00,0xFF,0x03,0xF8,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0x00,0xF0,0xFF,0x7F,0x00,0x00,0xFC,0x3F,0x00,0xFE,0x07,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x1F,0x00,0xE0,0xFF,0x1F,0x00,0xC0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0x00,0xFC,0x07,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x1F,0x00,0xC0,0xFF,0x3F,0x00,0x00,0xFF,0xFF,0x07,0x00,0x80,0xFF,0x03,0xF0,0x0F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x3F,0x00,0x80,0xFF,0xFF,0x00,0x00,0xFC,0xFF,0x3F,0x00,0x00,0xFC,0x0F,0xE0,0x1F,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x3F,0x00,0x00,0xFF,0xFF,0x01,0x00,0xF8,0xFF,0xFF,0x00,0x00,0xF0,0x1F,0xC0,0x1F,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0x7F,0x00,0x00,0xFE,0xFF,0x07,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x80,0x7F,0x80,0x3F,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x00,0x00,0xFC,0xFF,0x0F,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0x01,0x3F,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0xF8,0xFF,0x3F,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF8,0x03,0x7E,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x01,0x00,0xF0,0xFF,0x7F,0x00,0x00,0xF8,0xFF,0xFF,0x00,0x00,0xC0,0x0F,0x7C,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x03,0x00,0xE0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x00,0x3F,0xF8,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x03,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0x7C,0xF0,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x0F,0x00,0x00,0xFF,0xFF,0x0F,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0xF0,0xE1,0x70,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x1F,0x00,0x00,0xFE,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0xC0,0xC3,0x39,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0xCE,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0x98,0x09,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0x20,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0xFF,0x00,0x00,0x40,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x07,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0xFF,0x03,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xE0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0x80,0xFF,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xE0,0xFF,0xFF,0x07,0x00,0x00,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x1F,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0x00,0xFF,0xFF,0x1F,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFF,0xFF,0x3F,0x00,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0xFD,0xFF,0xFF,0x00,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x50,0xFB,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x68,0xE6,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x8C,0xFF,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x3C,0xFC,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF1,0x78,0xF0,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x0F,0x00,0x00,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xF1,0xF0,0xC1,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0x3F,0x00,0x00,0xFE,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xF0,0xE0,0x03,0xFF,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0xF8,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x70,0xF0,0xC1,0x0F,0xFC,0xFF,0xFF,0x00,0x00,0xE0,0xFF,0xFF,0x01,0x00,0xF0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0xF0,0x81,0x3F,0xE0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x03,0x00,0xE0,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0xF0,0x03,0x7F,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x0F,0x00,0xC0,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0xF0,0x03,0xFE,0x01,0xFE,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0x1F,0x00,0x80,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xF0,0x07,0xFC,0x07,0xF0,0xFF,0xFF,0x00,0x00,0xE0,0xFF,0x7F,0x00,0x00,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x1F,0xF0,0x07,0xF8,0x1F,0xC0,0xFF,0xFF,0x03,0x00,0xC0,0xFF,0xFF,0x00,0x00,0xFE,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0F,0xF0,0x0F,0xF0,0x3F,0x00,0xFE,0xFF,0x1F,0x00,0x00,0xFF,0xFF,0x03,0x00,0xFC,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x0F,0xF0,0x1F,0xC0,0xFF,0x00,0xF8,0xFF,0x7F,0x00,0x00,0xFC,0xFF,0x07,0x00,0xF8,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0xF0,0x1F,0x80,0xFF,0x03,0xC0,0xFF,0xFF,0x01,0x00,0xF0,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x07,0xE0,0x3F,0x00,0xFF,0x0F,0x00,0xFF,0xFF,0x07,0x00,0xE0,0xFF,0x3F,0x00,0xE0,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x07,0xE0,0x7F,0x00,0xFC,0x3F,0x00,0xF8,0xFF,0x1F,0x00,0x80,0xFF,0x7F,0x00,0xC0,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x07,0xE0,0xFF,0x00,0xF8,0xFF,0x00,0xC0,0xFF,0x7F,0x00,0x00,0xFE,0xFF,0x00,0xC0,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0x07,0xC0,0xFF,0x01,0xF0,0xFF,0x03,0x00,0xFF,0xFF,0x01,0x00,0xFC,0xFF,0x01,0x80,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x07,0xC0,0xFF,0x01,0xC0,0xFF,0x0F,0x00,0xF8,0xFF,0x0F,0x00,0xF0,0xFF,0x03,0x00,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x07,0xC0,0xFF,0x03,0x80,0xFF,0x3F,0x00,0xE0,0xFF,0x3F,0x00,0xE0,0xFF,0x07,0x00,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x07,0x80,0xFF,0x07,0x00,0xFE,0xFF,0x00,0x00,0xFF,0xFF,0x00,0x80,0xFF,0x1F,0x00,0xFE,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

const PROGMEM uint8_t cube_w[60][32] = {
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x00,0xE0,0xFF,0x07,0x00,0xFC,0xFF,0x07,0x00,0xFC,0xFF,0x01,0xC0,0xFF,0x03,0xE0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x00,0xC0,0xFF,0x0F,0x00,0xF0,0xFF,0x1F,0x00,0xF0,0xFF,0x03,0x80,0xFF,0x03,0xE0,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x01,0x80,0xFF,0x3F,0x00,0x80,0xFF,0xFF,0x00,0xC0,0xFF,0x0F,0x80,0xFF,0x03,0xE0,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x03,0x00,0xFF,0x7F,0x00,0x00,0xFE,0xFF,0x03,0x00,0xFF,0x1F,0x00,0xFF,0x07,0xE0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x03,0x00,0xFE,0xFF,0x01,0x00,0xF8,0xFF,0x1F,0x00,0xFC,0x3F,0x00,0xFE,0x07,0xE0,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x07,0x00,0xFC,0xFF,0x07,0x00,0xE0,0xFF,0xFF,0x00,0xF0,0xFF,0x00,0xFC,0x07,0xE0,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0x00,0x80,0xFF,0xFF,0x03,0xC0,0xFF,0x01,0xF8,0x0F,0xF0,0x0F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x1F,0x00,0xE0,0xFF,0x3F,0x00,0x00,0xFE,0xFF,0x1F,0x00,0xFF,0x03,0xF8,0x0F,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0x3F,0x00,0xC0,0xFF,0xFF,0x00,0x00,0xF8,0xFF,0x7F,0x00,0xFC,0x0F,0xF0,0x0F,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0x7F,0x00,0x00,0xFF,0xFF,0x03,0x00,0xC0,0xFF,0xFF,0x03,0xF8,0x1F,0xE0,0x0F,0xF8,0x03,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x00,0x00,0xFE,0xFF,0x07,0x00,0x00,0xFF,0xFF,0x0F,0xE0,0x3F,0xE0,0x0F,0xF8,0x01,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x01,0x00,0xF8,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0x7F,0x80,0x7F,0xC0,0x0F,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x03,0x00,0xF0,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0xFE,0xC0,0x0F,0x7C,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x07,0x00,0xC0,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x07,0xFC,0x81,0x0F,0x3E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x0F,0x00,0x80,0xFF,0xFF,0x07,0x00,0x00,0xFF,0xFF,0x3F,0xF0,0x83,0x0F,0x1E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x1F,0x00,0x00,0xFE,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0xFF,0xC0,0x07,0x0F,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0xFC,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x83,0x0F,0x8F,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x00,0x00,0xF0,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x1E,0x8F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x3F,0x3C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0xFF,0x31,0x66,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x67,0x36,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0xDF,0x1A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0x00,0xFF,0xFF,0xBF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x00,0xFC,0xFF,0xFF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0xF8,0xFF,0xFF,0x00,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0xE0,0xFF,0xFF,0x03,0x00,0x00,0xFE,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0xF8,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0xFF,0x01,0x00,0x80,0xFF,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0xFF,0xFF,0x00,0x00,0xE0,0xFF,0xFF,0x07,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFF,0xFF,0x01,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xFF,0xFF,0x07,0x00,0x00,0xFE,0xFF,0x3F,0x00,0x00,0xF0,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0xC0,0xFF,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xE0,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA0,0x02,0x00,0x00,0xFF,0xFF,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0xC0,0xFF,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA0,0x04,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0xC0,0xFF,0xFF,0x03,0x00,0x00,0xFF,0xFF,0x07,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x90,0x19,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0x80,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x0F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x98,0x73,0x00,0x00,0xC0,0xFF,0xFF,0x0F,0x00,0x00,0xFE,0xFF,0x1F,0x00,0x00,0xFC,0xFF,0x1F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x9C,0xC3,0x03,0x00,0x00,0xFF,0xFF,0x3F,0x00,0x00,0xF8,0xFF,0x7F,0x00,0x00,0xF8,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x87,0x0F,0x00,0x00,0xFC,0xFF,0xFF,0x00,0x00,0xF0,0xFF,0xFF,0x00,0x00,0xF0,0xFF,0x3F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0F,0x3E,0x00,0x00,0xF0,0xFF,0xFF,0x03,0x00,0xC0,0xFF,0xFF,0x01,0x00,0xE0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0x1F,0xFC,0x00,0x00,0xC0,0xFF,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x07,0x00,0xC0,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x0F,0x3E,0xF0,0x03,0x00,0x00,0xFF,0xFF,0x1F,0x00,0x00,0xFE,0xFF,0x0F,0x00,0x80,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x0F,0x7E,0xC0,0x1F,0x00,0x00,0xFC,0xFF,0x7F,0x00,0x00,0xFC,0xFF,0x1F,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0F,0xFC,0x80,0x7F,0x00,0x00,0xF0,0xFF,0xFF,0x01,0x00,0xF0,0xFF,0x3F,0x00,0x00,0xFF,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x0F,0xFC,0x01,0xFE,0x01,0x00,0xC0,0xFF,0xFF,0x07,0x00,0xE0,0xFF,0x7F,0x00,0x00,0xFE,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x0F,0xF8,0x03,0xF8,0x0F,0x00,0x00,0xFF,0xFF,0x1F,0x00,0x80,0xFF,0xFF,0x00,0x00,0xFC,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x0F,0xF8,0x07,0xF0,0x3F,0x00,0x00,0xFC,0xFF,0x3F,0x00,0x00,0xFF,0xFF,0x01,0x00,0xFC,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0xF0,0x0F,0xC0,0xFF,0x01,0x00,0xE0,0xFF,0xFF,0x00,0x00,0xFC,0xFF,0x03,0x00,0xF8,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0xE0,0x3F,0x00,0xFF,0x07,0x00,0x80,0xFF,0xFF,0x03,0x00,0xF8,0xFF,0x07,0x00,0xF8,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F,0xE0,0x7F,0x00,0xFC,0x3F,0x00,0x00,0xFE,0xFF,0x0F,0x00,0xF0,0xFF,0x0F,0x00,0xF0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1F,0xC0,0xFF,0x00,0xF0,0xFF,0x00,0x00,0xF8,0xFF,0x1F,0x00,0xC0,0xFF,0x1F,0x00,0xF0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1F,0x80,0xFF,0x03,0xC0,0xFF,0x07,0x00,0xE0,0xFF,0x7F,0x00,0x80,0xFF,0x3F,0x00,0xF0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x1F,0x00,0xFF,0x07,0x00,0xFF,0x3F,0x00,0x80,0xFF,0xFF,0x01,0x00,0xFF,0x3F,0x00,0xE0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3F,0x00,0xFE,0x0F,0x00,0xFC,0xFF,0x00,0x00,0xFE,0xFF,0x03,0x00,0xFE,0x7F,0x00,0xE0,0xFF,0x01,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3F,0x00,0xFE,0x3F,0x00,0xF0,0xFF,0x07,0x00,0xF0,0xFF,0x0F,0x00,0xFC,0xFF,0x00,0xE0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x3F,0x00,0xFC,0x7F,0x00,0xC0,0xFF,0x1F,0x00,0xC0,0xFF,0x3F,0x00,0xF8,0xFF,0x00,0xE0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0x7F,0x00,0xF8,0xFF,0x01,0x00,0xFF,0xFF,0x00,0x00,0xFF,0x7F,0x00,0xE0,0xFF,0x01,0xE0,0xFF,0x00,0x00,0x00,0x00,0x00,0x00},
};

// Let's draw!
void draw() {

  // Clear the screen
  display.clear();

  // Direct access to buffer
  for( int i = 0 ; i < BWIDTH ; i++ ) {
    *( red.get() + i ) = pgm_read_byte( &cube_r[frameCount%60][i] );

    *( red.get() + i ) |= pgm_read_byte( &cube_w[frameCount%60][i] );
    *( green.get() + i ) = pgm_read_byte( &cube_w[frameCount%60][i] );
    *( blue.get() + i ) = pgm_read_byte( &cube_w[frameCount%60][i] );
  }
  
}
