/*
RSD.cpp - A library to create a rolling shutter display
Copyright (c) 2018 Facundo Daguerre (a.k.a der faq).  All right reserved.
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "RSD.h"

//The pin numbers and the type of led
static uint8_t pin_R, pin_G, pin_B;
static volatile common_type led_type;

//For fast access to drive the pins in the interrupt rutine
static volatile uint8_t *pinR_port;
static volatile uint8_t pinR_mask;

static volatile uint8_t *pinG_port;
static volatile uint8_t pinG_mask;

static volatile uint8_t *pinB_port;
static volatile uint8_t pinB_mask;

//Sync variables for the timer
static volatile uint16_t pos = 0;
static volatile uint16_t tick = TICK;
static volatile uint16_t last = TICK;

//"Video" memory for each channel (df for dataFrame)
static volatile uint8_t dfR[ BWIDTH ];
static volatile uint8_t dfG[ BWIDTH ];
static volatile uint8_t dfB[ BWIDTH ];

// Some kind of doble buffer video memory (db for dataBuffer or doubleBuffer),
// it's *relative* safe to write here to be truly safe we need to implement some sort of signaling
uint8_t dbR[ BWIDTH ];
uint8_t dbG[ BWIDTH ];
uint8_t dbB[ BWIDTH ];

//Signalig variable
		/* 0: Busy, not copy, from scketch to library, processing over dataBuffers
		   1: Ready for copy, from scketch to library, end of processing
		   2: Beging of frame, from library to scketch, ready for process, quick go! */
volatile uint8_t frameStatus = 1; 
volatile uint32_t frameCount = 0;
volatile uint16_t frameLost = 0;

//This variable will hold the function acting as event source.
static callbackFunction _draw;

//Interrupt rutine, all the magic happens here
ISR( TIMER1_COMPA_vect ) {
	
	//Calculate the bit position in the memory that now we need to looking for
	//May be this could work better using pointers, isn't?
	uint8_t idx =  pos / 8;
	uint8_t bitmask = ( 1 << pos % 8 ); //The bit-mask

	//Depends of the led type, for a 1 in memory (on), we need to turn on or turn off
	//the pins, acting like a source current or sink current respectively
  	
  	if (led_type) { //If led_type = 1 = COMMON_ANODE

  		if ( bitmask & dfR[ idx ] ) { 	//If we have a one (on) in the memory
  		    *pinR_port &= ~( pinR_mask ); //We need to turn off the pin (sink current)
  		} else { 						//If we have a zero (off) in the memory
    		*pinR_port |= pinR_mask; 	//We need to turn on the pin (not current)
  		}

  		//Same for the rest of the channels, green and blue
  		if ( bitmask & dfG[ idx ] ) {
    		*pinG_port &= ~( pinG_mask );
  		} else {
    		*pinG_port |= pinG_mask; 
  		}

  		if ( bitmask & dfB[ idx ] ) {
    		*pinB_port &= ~( pinB_mask );
  		} else {
    		*pinB_port |= pinB_mask; 
  		}
  
	} else { // Else if led_type = 0 = COMMON_CATHODE

  		if ( bitmask & dfR[ idx ] ) { 	//If we have a one (on) in the memory
  			*pinR_port |= pinR_mask;    //We need to turn on the pin (source current)
  		} else { 						//If we have a zero (off) in the memory
    		*pinR_port &= ~( pinR_mask );	//We need to turn off the pin (not current)
  		}

  		//Same for the rest of the channels, green and blue
  		if ( bitmask & dfG[ idx ] ) {
  			*pinG_port |= pinG_mask;    
  		} else { 						
    		*pinG_port &= ~( pinG_mask );	
  		}

  		if ( bitmask & dfB[ idx ] ) { 	
  			*pinB_port |= pinB_mask;    
  		} else { 						
    		*pinB_port &= ~( pinB_mask );	
  		}
  	}
  	
  	//Increment the position
  	pos++;

  	//Special moments
  	if ( pos ==  WIDTH - 2 ) { 
    	OCR1A = last; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine in 30 Hz
  	}
  	
  	//Interesting 
  	if ( pos == WIDTH - 1 ) { 
    	OCR1A = tick; //backwards period
  	}

  	//If the position is the first
  	if ( pos == WIDTH ) { 
        
        // Restore the tick period (thick tuning), don't worry OCR1A is intrisic double buffer too
    	// it will be updated in the next frame
    	//OCR1A = tick;
        
    	pos = 0;
        frameCount++;
        
        if( frameStatus == 1 ) { //And the buffer is ready to copy
      		for( uint8_t i = 0 ; i < BWIDTH ; i++ ) { //Copy the buffer to "video" memory
        		dfR[i] = dbR[i]; 
        		dfG[i] = dbG[i]; 
        		dfB[i] = dbB[i]; 
            }
            
            //Tells to scketch that is a good moment for begin a process over buffers //This is the right place?
            frameStatus = 2;
        } else {
            frameLost++;
        }
    	// else: don't copy. It would nice to give a return, what would allow us to count the
    	// lost frames . 10/12/18 Okey.
    	
  	}
  
}


void RSD::begin( uint8_t pinR , uint8_t pinG , uint8_t pinB , common_type commonType ) {
	//Save the numers pins
	pin_R = pinR;
	pin_G = pinG;
	pin_B = pinB;

	//Save the led type
  	led_type = commonType;
	
	//Middle level arduino functions, coverts the number of the pin, to the
	//respectly port and bitmask, needs for quick access in the interrupt rutine
	pinR_port = portOutputRegister(digitalPinToPort(pin_R));
  	pinR_mask = digitalPinToBitMask(pin_R);

  	pinG_port = portOutputRegister(digitalPinToPort(pin_G));
  	pinG_mask = digitalPinToBitMask(pin_G);

  	pinB_port = portOutputRegister(digitalPinToPort(pin_B));
  	pinB_mask = digitalPinToBitMask(pin_B);

	//Set the outputs
	pinMode(pin_R, OUTPUT);
  	pinMode(pin_G, OUTPUT);
  	pinMode(pin_B, OUTPUT);

  	//Initialize the timer
	RSD:initTimer1();

	//no function attached yet: clear function pointer.
	_draw = NULL;
}

void RSD::initTimer1(){
	TCCR1A = 0; // Timer/Counter1 Control Register A, reset
	TCCR1B = 0; // Timer/Counter1 Control Register B, reset
	TCCR1B |= (1<<WGM12); //CTC w/ TOP in 0CRA
	TCCR1B |= (1<<CS10);  //No preescaling F_CPU (fine tunning)
	OCR1A = tick;
	last = tick;
	TIMSK1 |= (1<<OCIE1A); //Set Output Compare A Match Interrupt Enable
}

//Tunnig functions

uint16_t RSD::getTick() {
	return	tick;
}

uint16_t RSD::getLowerTick() {
	return	F_CPU / ( ( F_CAM + 1 ) * WIDTH );
}

uint16_t RSD::getHigherTick() {
	return	F_CPU / ( (F_CAM - 1) * WIDTH );
}

uint16_t RSD::getLastTick() {
	return	last;
}

uint16_t RSD::getLowerLastTick() {
	return	tick - (WIDTH - 1); //It's right?
}

uint16_t RSD::getHigherLastTick() {
	return	tick + (WIDTH - 1);
}

bool RSD::setTick(int _tick) {

	if( ( _tick > getLowerTick() ) &&	(  _tick < getHigherTick() ) ) {
		tick = _tick;
		return true;
	} else {
		return false; //Maybe we want to change the frequency?
	}
}

bool RSD::setLastTick( int _tick ) {
	
	if( ( _tick  > getLowerLastTick() ) && ( _tick  < getHigherLastTick() ) ) {
			last = _tick;
			return true;
		} else {
			return false; //Maybe we want to change the tick?
		}
}

uint32_t RSD::getPeriod() {
	return (uint32_t) tick * (WIDTH - 1) + last;
}

float RSD::getFrequency() {
	return (float) F_CPU / getPeriod();
}

void RSD::changePos(bool direction) {
	if ( direction ) { //If direction is forward
		if ( pos < WIDTH ) {
			pos++;
		} 
	} else { //If direction is backwards
		if ( pos > 0 ) {
			pos--;
		}
	}
	
}

void RSD::update() {
	if ( frameStatus == 2 ) {
		frameStatus = 0;
		_draw();
		frameStatus = 1;
	}
}

//Callback function, let's draw!
void RSD::attachDraw( callbackFunction newFunction ) {
	_draw = newFunction;
} // attachDraw


// Graphic screen primitives

// Drawing primitives

void RSD::putLine( colour c , uint16_t _pos ) { //Stupid way but, whaterever
												//Try rearranging the colors order
	switch( c ) {
		case BLACK:
			clearLine( dbR , _pos );
			clearLine( dbG , _pos );
			clearLine( dbB , _pos );
			break;
		case RED:
			putLine( dbR , _pos );
			clearLine( dbG , _pos );
			clearLine( dbB , _pos );
			break;
		case GREEN:
			clearLine( dbR , _pos );
			putLine( dbG , _pos );
			clearLine( dbB , _pos );
			break;
		case BLUE:
			clearLine( dbR , _pos );
			clearLine( dbG , _pos );
			putLine( dbB , _pos );
			break;
		case YELLOW:
			putLine( dbR , _pos );
			putLine( dbG , _pos );
			clearLine( dbB , _pos );
			break;
		case CYAN:
			clearLine( dbR , _pos );
			putLine( dbG , _pos );
			putLine( dbB , _pos );
			break;
		case MAGENTA:
			putLine( dbR , _pos );
			clearLine( dbG , _pos );
			putLine( dbB , _pos );
			break;
		case WHITE:
			putLine( dbR , _pos );
			putLine( dbG , _pos );
			putLine( dbB , _pos );
			break;
		default: break;
	}

    
}

void RSD::fillRange( colour c , uint16_t begin , uint16_t end ) {
    if ( !( begin >= WIDTH ) || ( end >= WIDTH ) || ( begin > end ) ) { 
        while ( end > begin ) {
            RSD::putLine( c , end );
            end--;
        }
        RSD::putLine( c , begin );
    }
}

void RSD::fillScreen( colour c = WHITE ) {
	
    switch( c ) {
		case BLACK:
			clearChannel( dbR );
			clearChannel( dbG );
			clearChannel( dbB );
			break;
        case RED:
			fillChannel( dbR );
			clearChannel( dbG );
			clearChannel( dbB );
			break;
        case GREEN:
			clearChannel( dbR );
			fillChannel( dbG );
			clearChannel( dbB );
			break;
		case BLUE:
			clearChannel( dbR );
			clearChannel( dbG );
			fillChannel( dbB );
			break;
		case YELLOW:
			fillChannel( dbR );
			fillChannel( dbG );
			clearChannel( dbB );
			break;
		case CYAN:
			clearChannel( dbR );
			fillChannel( dbG );
			fillChannel( dbB );
			break;
		case MAGENTA:
			fillChannel( dbR );
			clearChannel( dbG );
			fillChannel( dbB );
			break;
		case WHITE:
			fillChannel( dbR );
			fillChannel( dbG );
			fillChannel( dbB );
			break;
		default: break;
	}
}

// Get functions

colour RSD::getLine( uint16_t _pos ) { //Stupid way but, whaterever
									//Try rearranging the colors order
	if(getChannel(dbR,_pos)) {
		
		if(getChannel( dbG , _pos ) ) {
			if ( getChannel( dbB , _pos ) ) {
				return WHITE;
			} else {
				return YELLOW;
			}
		} else {
			if ( getChannel( dbB , _pos ) ) {
				return MAGENTA;
			} else {
				return RED;
			}
		}

	} else if( getChannel( dbG, _pos ) ) {
		
		if( getChannel( dbB, _pos) ) {
			return CYAN;
		} else {
			return GREEN;
		}

	} else if ( getChannel( dbB , _pos ) ) {
			return BLUE;
		} else {
			return BLACK;
	}
}
