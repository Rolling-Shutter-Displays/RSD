/*
Channel.h - Part of RSD library.
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

#ifndef __Channel_RSD_library__
#define __Channel_RSD_library__

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

typedef enum { COMMON_CATHODE = 0, COMMON_ANODE = 1 } common_type;
typedef enum { BLACK , BLUE , RED , MAGENTA , GREEN , CYAN , YELLOW , WHITE } colour;

class Channel {

public:
    
    //Make up variables
    uint8_t *pinPort;
    uint8_t pinMask;
    common_type ledType;
    uint8_t bwidth;
    uint16_t width;
    
    //Double buffer video memory
    uint8_t *buffer[ 2 ];
    
    //Swap variables
    uint8_t currentBuffer;
    uint8_t * currentBufferP = & currentBuffer;
    
    //Constructor
    Channel( uint8_t pin , common_type commonType , uint8_t _bwidth ) {
    
        ledType = commonType;
    
        pinPort = portOutputRegister( digitalPinToPort( pin ) );
        pinMask = digitalPinToBitMask( pin );
        pinMode( pin , OUTPUT );
    
        bwidth = _bwidth;
        width = (_bwidth*8) - 1;
    
        // Allocate and clean memory for buffers
        buffer[0] = calloc( _bwidth , 1 );
        buffer[1] = calloc( _bwidth , 1 ); 
    
    }
    
    //Drawing primitives
    
    inline void line( uint16_t _pos ) {
        *( buffer[currentBuffer] + _pos / 8 ) |= ( 1 << _pos % 8 );
    }
    
    inline void line( uint16_t _pos , colour c) {
        if( c ) {
            line( _pos );
        } else {
            clear( _pos );
        }
    }
    
    inline void lineSafe( int16_t _pos ) {
        if (_pos > width ) _pos = width;
        if (_pos < 0 ) _pos = 0;
        *( buffer[currentBuffer] + _pos / 8 ) |= ( 1 << _pos % 8 );
    }
    
    inline void lineSafe( uint16_t _pos , colour c) {
        if( c ) {
            lineSafe( _pos );
        } else {
            clearSafe( _pos );
        }
    }
    
    inline void fill( uint16_t x0 , uint16_t x1 ) {
        
        if ( x1 > x0 ) {
            
            do {
                line( x1 );
                x1--;
            } while( x1 > x0 );
            
            line( x0 );
            
        } else if( x1 == x0 ) {
            
            line( x1 );
            
        } else {
            
            do {
                line( x0 );
                x0--;
            } while( x0 > x1 );
            
            line( x1 );
        }
    }
    
    inline void fill( uint16_t x0 , uint16_t x1 , colour c ) {
        if( c ) {
            fill( x0 , x1 );
        } else {
            clear( x0 , x1 );
        }
    }
    
    inline void fillSafe( int16_t x0 , int16_t x1 ) { //Check
        
        if ( x1 > x0 ) {
            
            do {
                lineSafe( x1 );
                x1--;
            } while( x1 > x0 );
            
            lineSafe( x0 );
            
        } else if( x1 == x0 ) {
            
            lineSafe( x1 );
            
        } else {
            
            do {
                lineSafe( x0 );
                x0--;
            } while( x0 > x1 );
            
            lineSafe( x1 );
        }
    }
    
    inline void fillSafe( uint16_t x0 , uint16_t x1 , colour c ) {
        if( c ) {
            fillSafe( x0 , x1 );
        } else {
            clearSafe( x0 , x1 );
        }
    }
    
    inline void fill() {
        for( uint8_t i = 0 ; i < bwidth ; i++ ) {
            *( buffer[currentBuffer] + i ) = 0xff;
        }
    }
    
    inline void clear( uint16_t _pos ) {
        *( buffer[currentBuffer] + _pos / 8 ) &= ~( 1 << _pos % 8 );
    }
    
    inline void clearSafe( int16_t _pos ) {
        if (_pos > width ) _pos = width;
        if (_pos < 0 ) _pos = 0;
        *( buffer[currentBuffer] + _pos / 8 ) &= ~( 1 << _pos % 8 );
    }
    
    inline void clear( uint16_t x0 , uint16_t x1 ) {
        
        if ( x1 > x0 ) {
            
            do {
                clear( x1 );
                x1--;
            } while( x1 > x0 );
            
            clear( x0 );
            
        } else if( x1 == x0 ) {
            
            clear( x1 );
            
        } else {
            
            do {
                clear( x0 );
                x0--;
            } while( x0 > x1 );
            
            clear( x1 );
        }
    }
    
    inline void clearSafe( int16_t x0 , int16_t x1 ) { //Check
        
        if ( x1 > x0 ) {
            
            do {
                clearSafe( x1 );
                x1--;
            } while( x1 > x0 );
            
            clearSafe( x0 );
            
        } else if( x1 == x0 ) {
            
            clearSafe( x1 );
            
        } else {
            
            do {
                clearSafe( x0 );
                x0--;
            } while( x0 > x1 );
            
            clearSafe( x1 );
        }
    }
    
    inline void clear() {
        for( uint8_t i = 0 ; i < bwidth ; i++ ) {
            *( buffer[currentBuffer] + i ) = 0x00;
        }
    }
    
    inline uint8_t * get() { 
        return buffer[ currentBuffer ]; 
    }
    
    inline bool get( int16_t _pos ) {
        if (_pos > width ) _pos = width;
        if (_pos < 0 ) _pos = 0;
        return *( buffer[currentBuffer] + _pos / 8 ) & ( 1 << _pos % 8 ) ? true : false;
    }
    
    inline bool getSafe( uint16_t _pos ) {
        return *( buffer[currentBuffer] + _pos / 8 ) & ( 1 << _pos % 8 ) ? true : false;
    }
    
    inline void invert() {
        for( uint8_t i = 0 ; i < bwidth ; i++ ) {
            *( buffer[currentBuffer] + i ) = ~(*( buffer[currentBuffer] + i ));
        }
    }
    
    inline void copy( Channel *ch ) {
        if( ch == this ) {
            for( uint8_t i = 0 ; i < bwidth ; i++ ) {
                *( buffer[ currentBuffer ] + i ) = *( buffer[ 1 - currentBuffer ] + i );
            }
        } else {
            for( uint8_t i = 0 ; i < bwidth ; i++ ) {
                *( buffer[ currentBuffer ] + i ) = *( ch->get() + i );
            }
        }
    }
    
private:
    
};

#endif
