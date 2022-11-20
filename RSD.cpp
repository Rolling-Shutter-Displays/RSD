/*
RSD.cpp - A library to create a rolling shutter display
Copyright (c) 2018-2020 Facundo Daguerre (a.k.a der faq).  All right reserved.
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

static uint8_t freq;
static uint8_t bwidth;
static volatile uint16_t width;

static volatile uint16_t thick;
static volatile uint16_t fine;

static volatile int16_t phase = 0;

static Channel* channels[MAX_CHANNELS];
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

volatile uint32_t frameCount;
volatile uint32_t frameLost;

//This variable will hold the function acting as event source.
static callbackFunction _draw;

//Interrupt rutine, all the magic happens here
static inline void interrupt() {
    
    
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
            if ( ( bitmask & ( *( (channels[i]->buffer[currentBuffer]) + idx ) ) ) && ( on ) ) {
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
        #if defined(__AVR_ATtinyX5__)
        OCR0A = fine; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine at 30 Hz
        #else
    	OCR1A = fine; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine at 30 Hz
        #endif
  	}
    
  	//If the position is the first
  	if ( pos == width ) { 
        
        // Restore the tick period (thick tuning), don't worry OCR1A is intrisic double buffer too
    	// it will be updated in the next frame
        
        #if defined(__AVR_ATtinyX5__)
        OCR0A = thick; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine at 30 Hz
        #else
        OCR1A = thick; //fine correction of frequency <<-- this method can't be for general frequency, luckily work's really fine at 30 Hz
        #endif

        pos = 0;
        
        if( frameStatus == 1 ) { //And the buffer is ready to swap
            
            //Swap buffers
            for( uint8_t i = 0 ; i < channelsCount ; i++ ) {
                *( channels[i]->currentBufferP ) = currentBuffer;
            }
            
            currentBuffer = 1 - currentBuffer;
            //Count frame
            frameCount++;
            //Tells to scketch that is a good moment for begin a process over buffers, via update() function
            frameStatus = 2;
        } else {
            //Count frame lost
            frameLost++;
        }
    	
  	}

}

#if defined(__AVR_ATtinyX5__)

#if F_CPU == 8000000L
    #warning "Implemented on Timer 0, this crashes delay() and millis(). To improve, collaborate in https://github.com/Rolling-Shutter-Displays/RSD"
#else
    #error "Not implemented with this clock. Make it true, collaborating in https://github.com/Rolling-Shutter-Displays/RSD"
#endif

ISR( TIMER0_COMPA_vect ) {
    interrupt(); 
}

void RSD::initTimer(){
	//Reset registers
    TCCR0B = (0<<FOC0A) | (0<<FOC0B) | (0<<WGM02) | (0<<CS02) | (0<<CS01) | (0<<CS00);
    TCCR0A = (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (0<<WGM00);
    // Reset the count to zero

    TCNT0 = 0;
    // Set the output compare registers to zero
    OCR0A = 0;
    OCR0B = 0;

    // Disable all Timer0 interrupts
    TIMSK &= ~((1<<OCIE0B) | (1<<OCIE0A) | (1<<TOIE0));
    // Clear the Timer0 interrupt flags
    TIFR |= ((1<<OCF0B) | (1<<OCF0A) | (1<<TOV0));

	
	//CTC Mode
    TCCR0A |= ( 1<<WGM01 );
    
    //Prescaler configuration (from https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/cores/tiny/Tone.cpp)
    uint32_t ocr = F_CPU / ( freq * width ) ;
    uint8_t prescalarbits = 0b001;  // ck/1
    
    if (ocr > 256) {
      ocr >>= 3; //divide by 8
      prescalarbits = 0b010;  // ck/8
      if (ocr > 256)  {
        ocr >>= 3; //divide by a further 8
        prescalarbits = 0b011; //ck/64
        if (ocr > 256) {
          ocr >>= 2; //divide by a further 4
          prescalarbits = 0b100; //ck/256
          if (ocr > 256) {
            // can't do any better than /1024
            ocr >>= 2; //divide by a further 4
            prescalarbits = 0b101; //ck/1024
          }
        }
      }
    }
    
    //Set preescaler
    TCCR0B |= (prescalarbits << CS00);
    
    ocr -= 1; //Note we are doing the subtraction of 1 here to save repeatedly calculating ocr from just the frequency in the if tree above
    OCR0A = ocr;
    thick = fine = ocr;
	
	//Enable Interrupt
    TIMSK |= (1<<OCIE0A); //Set Output Compare A Match Interrupt Enable
}


#else

ISR( TIMER1_COMPA_vect ) {
    interrupt();
}

void RSD::initTimer(){
	//Reset registers
    TCCR1A = 0; // Timer/Counter1 Control Register A, reset
	TCCR1B = 0; // Timer/Counter1 Control Register B, reset
	
    //CTC Mode
	TCCR1B |= ( 1<<WGM12 ); //CTC w/ TOP in 0CRA
	
    //Preescaler configuration
    TCCR1B |= ( 1<<CS10 );  //No preescaling F_CPU (fine tunning)
	
	OCR1A = thick;
	//Enable Interrupt
    TIMSK1 |= ( 1<<OCIE1A ); //Set Output Compare A Match Interrupt Enable
}

#endif

//Begin
void RSD::begin( float _freq , uint8_t _bwidth ) {
    bwidth = _bwidth;
    width = _bwidth*8;
    
    /*
    RSD::setFrequency( _freq );
    freq = (uint8_t) _freq;
    */
    thick = F_CPU / ( _freq * width );
    fine = thick;
    

    //Initialize the timer
	RSD::initTimer();
}

//Attach channel objet
void RSD::attachChannel ( Channel *ch ) {
    channels[ channelsCount ] = ch;
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

uint16_t RSD::getThick() {
	return	thick;
}

uint16_t RSD::getLowerThick() {
	return	F_CPU / ( ( freq + 1 ) * width );
}

uint16_t RSD::getHigherThick() {
	return	F_CPU / ( ( freq - 1 ) * width );
}

uint16_t RSD::getFine() {
	return fine;
}

uint16_t RSD::getLowerFine() {
	return	thick - ( width - 1 ); 
}

uint16_t RSD::getHigherFine() {
	return	thick + ( width - 1 ); 
}

bool RSD::setThick( int _thick ) {

#if defined(__AVR_ATtinyX5__)
    
    //Not ready implemented

#else
	
    if( ( _thick > getLowerThick() ) && (  _thick < getHigherThick() ) ) {
		uint8_t oldSREG = SREG;
        cli();
        
        thick = _thick;
		
        SREG = oldSREG;
        
        return true;
	} else {
		return false; //Maybe we want to change the frequency?
	}

#endif
}

bool RSD::setFine( int _fine ) {

#if defined(__AVR_ATtinyX5__)
    
    //Not ready implemented

#else
	
	if( ( _fine  > getLowerFine() ) && ( _fine  < getHigherFine() ) ) {
			uint8_t oldSREG = SREG;
            cli();
            
            fine = _fine;
			
            SREG = oldSREG;
            
            return true;
		} else {
			return false; //Maybe we want to change the tick?
		}

#endif
}

void RSD::setFrequency( float _freq ) {
    unsigned int _ticks = F_CPU / ( _freq * width ) ;
    thick = _ticks;

    unsigned int _fine = F_CPU / _freq - _ticks * ( width - 1 ) ;
    fine = _fine;
}

uint32_t RSD::getPeriod() {
#if defined(__AVR_ATtinyX5__)
    
    //Not ready implemented

#else
    
	return (uint32_t) thick * (width - 1) + fine;

#endif
}

float RSD::getFrequency() {
#if defined(__AVR_ATtinyX5__)
    
    //Not ready implemented

#else

	return (float) F_CPU / getPeriod();

#endif
}

void RSD::shiftPhase( int _phase ) {
    phase += _phase;
}

void RSD::switchOn() {
    on = true;
}

 void RSD::switchOff() {
    on = false;
}
