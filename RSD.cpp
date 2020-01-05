/*
RSD.cpp - A library to create a rolling shutter display
Copyright (c) 2018-2019 Facundo Daguerre (a.k.a der faq).  All right reserved.
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

//Sync variables for the timer

static uint8_t fcam;
static uint8_t bwidth;
static volatile uint16_t width;

static volatile uint16_t tick;
static volatile uint16_t fine;

static volatile int16_t phase = 0;

static Channel *channels[MAX_CHANNELS];
static uint8_t channelsCount = 0;

//Signaling variable
/* 0: Busy, don't swap, from scketch ( via update() function ) to ISR ( via RSD library ), processing over buffers
   1: Ready for swap, from scketch to library, end of processing
   2: Beging of frame, from library to scketch, ready for process, quick go!
*/
static volatile uint8_t frameStatus = 1;
static volatile bool on = true;

static volatile uint16_t pos = 0;
static volatile uint8_t currentBuffer = 0;

static volatile uint32_t frameCount = 0;
static volatile uint32_t frameLost = 0;

//This variable will hold the function acting as event source.
static callbackFunction _draw;

//Interrupt rutine, all the magic happens here
ISR( TIMER1_COMPA_vect ) {
    
	//Calculate the bit position in the memory that now we need to looking for
	uint8_t idx =  pos / 8;
	uint8_t bitmask = ( 1 << pos % 8 ); //The bit-mask

	//Depends of the led type, for a 1 in memory (on), we need to turn on or turn off
	//the pins, acting like a source current or sink current respectively
    
    for ( uint8_t i = 0 ; i < channelsCount ; i++ ) {
        
        if ( channels[i]->ledType ) {
            
            if ( ( bitmask & ( *( channels[i]->buffer[currentBuffer] + idx ) ) ) && ( on ) ) {
                *(channels[i]->pinPort) &= ~( channels[i]->pinMask );
            } else {
                *(channels[i]->pinPort) |= channels[i]->pinMask;
            }
        } else {
            if ( ( bitmask & ( *( channels[i]->buffer[currentBuffer] + idx ) ) ) && ( on ) ) {
                *(channels[i]->pinPort) |= channels[i]->pinMask;
            } else {
                *(channels[i]->pinPort) &= ~( channels[i]->pinMask );
            }
        }
    }
    
  	//Phase shifter
    
    if (phase == 0) {
        
        pos++;
        
    } else if( phase > 0 ) {
        
        if ( pos + phase%width < width ) {
            pos = pos + phase%width;
        } else {
            pos = phase%width - ( width - pos );
        }
        
        phase = 0;
    
        
    } else {
        
        phase++;
        
    }
    
  	//Special moments
  	if ( pos == width - 1 ) { 
    	OCR1A = fine; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine at 30 Hz
  	}

  	//If the position is the first
  	if ( pos == width ) { 
        
        // Restore the tick period (thick tuning), don't worry OCR1A is intrisic double buffer too
    	// it will be updated in the next frame
        OCR1A = tick; 
    	
        pos = 0;
        
        if( frameStatus == 1 ) { //And the buffer is ready to swap
            
            //Swap buffers
            for( uint8_t i = 0 ; i < channelsCount ; i++ ) {
                *( channels[i]->currentBufferP ) = currentBuffer;
            }
            
            currentBuffer = 1 - currentBuffer;
            
            frameCount++;
            //Tells to scketch that is a good moment for begin a process over buffers, via update() function
            frameStatus = 2;
            
        } else {
            frameLost++;
        }
    	
  	}

}

void RSD::initTimer1(){
	TCCR1A = 0; // Timer/Counter1 Control Register A, reset
	TCCR1B = 0; // Timer/Counter1 Control Register B, reset
	TCCR1B |= ( 1<<WGM12 ); //CTC w/ TOP in 0CRA
	TCCR1B |= ( 1<<CS10 );  //No preescaling F_CPU (fine tunning)
	OCR1A = tick;
	TIMSK1 |= ( 1<<OCIE1A ); //Set Output Compare A Match Interrupt Enable
}

//Begin
void RSD::begin( uint8_t _fcam , uint8_t _bwidth ) {
    fcam = _fcam;
    bwidth = _bwidth;
    width = _bwidth*8;
    
    tick = F_CPU / ( fcam * width );
    fine = tick;
    //Initialize the timer
	RSD:initTimer1();
}

//Attach channel objet
void RSD::attachChannel ( Channel ch ) {
    channels[ channelsCount ] = &ch;
    channelsCount++;
}

//Attach callback function
void RSD::attachDraw( callbackFunction newFunction ) {
	_draw = newFunction;
} 

//Update
void RSD::update() {
	if ( frameStatus == 2 ) {
        frameStatus = 0;
		
        //Callback Function, let's draw!
        _draw();
        
        frameStatus = 1;
	}
}

//Tuning functions

uint16_t RSD::getTick() {
	return	tick;
}

uint16_t RSD::getLowerTick() {
	return	F_CPU / ( ( fcam + 1 ) * width );
}

uint16_t RSD::getHigherTick() {
	return	F_CPU / ( ( fcam - 1 ) * width );
}

uint16_t RSD::getFine() {
	return fine;
}

uint16_t RSD::getLowerFine() {
	return	tick - ( width - 1 ); //It this right? I doubt it
}

uint16_t RSD::getHigherFine() {
	return	tick + ( width - 1 );  //It this right? I doubt it
}

bool RSD::setTick( int _tick ) {

	if( ( _tick > getLowerTick() ) && (  _tick < getHigherTick() ) ) {
		uint8_t oldSREG = SREG;
        cli();
        
        tick = _tick;
		
        SREG = oldSREG;
        
        return true;
	} else {
		return false; //Maybe we want to change the frequency?
	}
}

bool RSD::setFine( int _tick ) {
	
	if( ( _tick  > getLowerFine() ) && ( _tick  < getHigherFine() ) ) {
			uint8_t oldSREG = SREG;
            cli();
            
            fine = _tick;
			
            SREG = oldSREG;
            
            return true;
		} else {
			return false; //Maybe we want to change the tick?
		}
}

uint32_t RSD::getPeriod() {
	return (uint32_t) tick * (width - 1) + fine;
}

float RSD::getFrequency() {
	return (float) F_CPU / getPeriod();
}

static void RSD::shiftPhase( int _phase ) {
    phase += _phase;
}

static void RSD::switchOn() {
    on = true;
}

static void RSD::switchOff() {
    on = false;
}
