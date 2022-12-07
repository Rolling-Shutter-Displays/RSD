/*
  Streaming.ino

  This is a example sketch to show how to stream images and videos from PC through the serial
  port to the RSD.
  
  Upload the scketch and run the Processing scketch given in this directory, or copy and paste it
  from the bottom of this file.

  *For full explanation see: https://github.com/Rolling-Shutter-Displays/RSD
  
  created 6 Mar 2019
  revised 5 Dic 2022
  by derfaq
  
  This example code is in the public domain.
*/

// Include RSD Library
#include <RSD.h>
#include <Channel.h>
#include <Screen.h>

// RSD Objet
RSD rsd;

#define BWIDTH 80             // Byte resolution
#define WIDTH ((BWIDTH*8)-1)  // Lines resolution - 1

// Channel( pin , led mode , byte resolution ) Objet
Channel red( 2 , COMMON_CATHODE , BWIDTH );
Channel green( 3 , COMMON_CATHODE , BWIDTH );
Channel blue( 4 , COMMON_CATHODE , BWIDTH );

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
  Serial.begin( 115200 );
  
}

void loop() {
  
  // Run the engine
  rsd.update();
  
}

// Let's draw!
void draw() {
  Serial.write( 'S' );
  Serial.readBytes( red.get() , BWIDTH );
  Serial.readBytes( green.get() , BWIDTH );
  Serial.readBytes( blue.get() , BWIDTH );
}


/* Processing  code for this example
 // Streaming.pde - Stream a line of the display window to the RSD
 // created 6 Mar 2019
 // by derfaq
 // This example code is in the public domain
 
import processing.serial.*;
Serial myPort;

int BWIDTH = 80;
int WIDTH = ((BWIDTH*8)-1);

byte[] dbR = new byte[BWIDTH];
byte[] dbG = new byte[BWIDTH];
byte[] dbB = new byte[BWIDTH];

void setup() {
  size( 640 , 360 ); 
  background(0);

  println(Serial.list());
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 115200);

}

int barWidth = 20;
int lastBar = -1;

void draw() {
  //Draw something here
  
  //Hue Processing example
  noStroke();
  colorMode(HSB, height, height, height);
  int whichBar = mouseX / barWidth;
  if (whichBar != lastBar) {
    int barX = whichBar * barWidth;
    fill(mouseY, height, height);
    rect(barX, 0, barWidth, height);
    lastBar = whichBar;
  }
  
  //saveFrame line to transmit to RSD
  saveFrame(mouseY);
}

void saveFrame(int y) {
  loadPixels();
  for (int i = 0 ; i < BWIDTH ; i++) {
    byte byteR, byteG, byteB;
    byteR = byteG = byteB = 0;
    color c;
    for(int j = 0; j < 8; j++){
      c = get( i*8 + j , y );
      if (( c >> 16 & 0xFF ) > 127) {
        byteR |= (1 << j);
      }
      if (( c >> 8 & 0xFF ) > 127) {
        byteG |= (1 << j);
      }
      if (( c & 0xFF ) > 127) {
        byteB |= (1 << j);
      }
    }
    dbR[i] = byteR;
    dbG[i] = byteG;
    dbB[i] = byteB;
  }
}

void serialEvent(Serial myPort) {
  int inByte = myPort.read();
  if (inByte == 'S') {
      myPort.write(dbR);
      myPort.write(dbG);
      myPort.write(dbB);
    }
}
*/
