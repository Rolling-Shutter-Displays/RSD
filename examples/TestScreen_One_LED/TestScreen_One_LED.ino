/*
  TestScreen_One_LED.ino

  This is a example sketch to show how to use the RSD library to make a Rolling Shutter Display
  using just one led.

  For tuning, open the serial monitor, type the frequency and hit enter. The data shows the
  current frequency of the RSD.

  For further explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 24 Feb 2019
  modified 12 Apr 2020
  by derfaq
  
  This example code is in the public domain.
*/

#include <RSD.h>
RSD rsd;

#define BWIDTH 32             //Byte resolution
#define WIDTH (BWIDTH*8 - 1)  //Line resolution

#define LED_PIN 13

//Channel( pin , led mode , byte resolution )
Channel white( LED_PIN , COMMON_CATHODE , BWIDTH );

void setup() {
  //Setup of the RSD 
  rsd.begin( 30 , BWIDTH );    //begin( frequency , byte resolution )
  
  rsd.attachChannel( &white ); //attachChannel( channel )
  
  rsd.attachDraw( draw );      //attachDraw( callback function )

  Serial.begin( 9600 );
}

void loop() {
  //Run the engine
  rsd.update();

  // Tuning: Analog way
  /*
  float freq = 30.0 - ( ( (float)analogRead( A0 ) / 1023.0 ) - 0.5 );
  rsd.setFrequency( freq );
  */
  
  //Tuning: Fixed way
  /*
  rsd.setFrequency( 29.987 ); // Samsung S6 Edge 
  */
  
  //Tuning: Serial way
  if ( Serial.available() ){
    float freq = Serial.parseFloat();
    if ( freq != 0 ) rsd.setFrequency( freq );
  }
  
  //Serial diagnosis
  Serial.print("@frsd: ");
  Serial.print( rsd.getFrequency() , 10 );
  Serial.print(" , BWIDHT: ");
  Serial.print( BWIDTH );
  Serial.print(" , thick: ");
  Serial.print( rsd.getThick() );
  Serial.print(" , fine: ");
  Serial.println( rsd.getFine() );
}

//Let's draw!
void draw() {
  
  white.fill( 0 , WIDTH/4 );

  for( int i = 1 ; i < (WIDTH/4) ; i++ ) {
    int val = (WIDTH/4)/i;
    if ( i%2 ) {
      white.clear( WIDTH/2 - val, WIDTH/2 + val);
    } else {
      white.fill( WIDTH/2 - val, WIDTH/2 + val);
    }
  }

  for( int i = 0 ; i < WIDTH/4 ; i++ ) {
    if( i%2 ) white.line( WIDTH - i );
  }
  
}