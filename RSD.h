/*
RSD.h - A library to create a rolling shutter display
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

#ifndef __RSD_library__
#define __RSD_library__

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Channel.h"

#define BWIDTH 32 //number of bytes
#define WIDTH ( BWIDTH * 8 ) //number of lines (bits)


#define F_CAM 30 //Assumes that the frequency of the camera is 30 hz
#define TICK F_CPU / ( F_CAM * WIDTH ) //initial frequency

#define MAX_CHANNELS 12

//Some kind of doble buffer (db) video memory, it's *relative* safe to write here
//to be truly safe we need to implement some sort of signaling
extern uint8_t dbR[ BWIDTH ];
extern uint8_t dbG[ BWIDTH ];
extern uint8_t dbB[ BWIDTH ];

//Signalig variable
		/* 0: Busy, not copy, from scketch to library, processing over dataBuffers
		   1: Ready for copy, from scketch to library, end of processing
		   2: Beging of frame, from library to scketch, ready for process, quick go! */
extern volatile uint8_t frameStatus;
extern volatile uint32_t frameCount;
extern volatile uint32_t frameLost; 

///---> Experimental
static Channel *channels[MAX_CHANNELS];

static uint8_t channelsCount = 0;
///<---

//Callback function types
extern "C" {
  typedef void ( *callbackFunction )( void );
}

typedef enum { COMMON_CATHODE = 0, COMMON_ANODE = 1 } common_type;

typedef enum { BLACK , RED , GREEN , BLUE , YELLOW , CYAN , MAGENTA , WHITE } colour;


class RSD {
	
	public:
		
        // Begin function. Needs to be called in the setup()
        
		static void begin( uint8_t pinR , uint8_t pinG , uint8_t pinB , common_type commonType = COMMON_ANODE );
        
        static void begin( uint8_t f_cam , uint8_t _bwidth );
        
        ///---> Experimental
        static void attachChannel ( Channel ch );
        ///<---

		// Update function. Needs to be called in the loop() 
        
		static void update();
		
		// Callback function attachment. Needs to be called in the setup(), and update() running in the loop()
		
		static void attachDraw( callbackFunction newFunction );

		// Tunning functions
		
        static uint16_t getTick();
		
		static uint16_t getLowerTick();

		static uint16_t getHigherTick();

		static uint16_t getLastTick();

		static uint16_t getLowerLastTick();

		static uint16_t getHigherLastTick();

		static bool setTick(int _tick);

		static bool setLastTick(int _last);

		static uint32_t getPeriod();

		static float getFrequency();

		static void changePos(bool direction);
        
        
        // Graphics primitives
        
        // Drawing methods
        
        // Unsafe and fast methods
        
		static inline void putLine( uint8_t *channel , uint16_t _pos ) {
			channel[ _pos / 8 ] |= ( 1 << _pos % 8 );
		}

		static void putLine( colour c , uint16_t _pos );
        
        // Safe methods
        
        static inline void fillRange( uint8_t *channel , uint16_t begin , uint16_t end ) {
            if ( !( ( begin >= WIDTH ) || ( end >= WIDTH ) || ( begin > end ) ) ) { 
                while ( end > begin ) {
                    RSD::putLine( channel , end );
                    end--;
                }
                RSD::putLine( channel , begin );
            }
        }
    
        static void fillRange( colour c , uint16_t begin , uint16_t end );
        
        
        static inline void fillChannel( uint8_t *channel ) {
			for( uint8_t i = 0 ; i < BWIDTH ; i++ ) {
				channel[ i ] = 0xff;
			}	
		}

		static void fillScreen( colour c );
        
        // Erasing methods
        
        // Unsafe methods
        
		static inline void clearLine( uint8_t *channel , uint16_t _pos) {
			channel[ _pos / 8 ] &= ~( 1 << _pos % 8 );
		}
		
		static inline void clearLine( uint16_t _pos ) {
			clearLine( dbR, _pos );
			clearLine( dbG, _pos );
			clearLine( dbB, _pos );
		}
		
		// Safe methods
		
		static inline void clearRange( uint8_t *channel , uint16_t begin , uint16_t end ) {
            if ( !( begin >= WIDTH ) || ( end >= WIDTH ) || ( begin > end ) ) { 
                while ( end > begin ) {
                    RSD::clearLine( channel , end );
                    end--;
                }
                RSD::clearLine( channel , begin );
            }
        }
        
        static inline void clearRange( uint16_t begin , uint16_t end ) {
            clearRange( dbR, begin , end );
			clearRange( dbG, begin , end );
			clearRange( dbB, begin , end );
        }
            
		
		static inline void clearChannel( uint8_t *channel ) {
			for(uint8_t i = 0; i < BWIDTH; i++) {
				channel[ i ] = 0;
			}
        }
        
        static inline void clearScreen() {
			clearChannel( dbR );
			clearChannel( dbG );
			clearChannel( dbB );
		}

		// Get methods
		
		// Unsafe methods
		
		static inline bool getChannel( uint8_t *channel , uint16_t _pos ) {
			return channel[ _pos / 8 ] & ( 1 << _pos % 8 ) ? true : false;
		}

		static colour getLine( uint16_t _pos );
        
	private:
        
		//Timer one initialization method
        
		static void initTimer1();
};

#endif
