/*
  TestScreen_RGB_LED.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using a RGB led.

  Use the potentiometer to tuning the RSD in the range of +-0.5 Hz.
  The data in the serial monitor shows the current frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 5 Mar 2019
  modified 12 Apr 2020
  by derfaq
  
  This example code is in the public domain.
*/

#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

RSD rsd;
#define BWIDTH 32              //Byte resolution
#define WIDTH ((BWIDTH*8)-1)   //Line resolution

//Channel( pin , led mode , byte resolution )
Channel red( 11 , COMMON_ANODE , BWIDTH );
Channel green( 3 , COMMON_ANODE , BWIDTH );
Channel blue( 6 , COMMON_ANODE , BWIDTH );

Screen display( &red , &green , &blue );

void setup() {

  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );      //begin( frequency , byte resolution )
  
  rsd.attachChannel( &red );     //attachChannel( channel )
  rsd.attachChannel( &green );
  rsd.attachChannel( &blue );

  rsd.attachDraw( draw );        //attachDraw( callback function )

  Serial.begin( 9600 );
  
}

void loop() {

  //Run the engine
  rsd.update();
  
  //Tuning: Fixed way
  /*
  rsd.setFrequency( 29.987 ); // Samsung S6 Edge 
  */

  //Tuning: Analog way
  float freq = 30.0 - ( ( (float)analogRead( A0 ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );
  
  //Tuning: Serial way
  /*
  if ( Serial.available() ) {
    float freq = Serial.parseFloat();
    if ( freq != 0 ) rsd.setFrequency( freq );
  }
  */
  
  //Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 6 );
  Serial.print(" Lines: ");
  Serial.println( BWIDTH*8 );
  
}

//Let's draw!
void draw() {

  display.clear();
  
  //Standarized order of the SMPTE/EBU color bar image : https://en.wikipedia.org/wiki/SMPTE_color_bars
  //from left to right, the colors are white, yellow, cyan, green, magenta, red, blue and black
  for( int i = 0 ; i <= WIDTH ; i++ ) {
    colour c = ( i * 8 ) / WIDTH;
    display.line( i , c );
  }

  //Grid resolution
  for( int i = 0 ; i < BWIDTH/2 ; i++ ) {
    if( 1 - i%2 ) display.line( i , WHITE );
  }

}