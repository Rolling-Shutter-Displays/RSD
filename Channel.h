#ifndef __Channel_RSD_library__
#define __Channel_RSD_library__

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

typedef enum { COMMON_CATHODE = 0, COMMON_ANODE = 1 } common_type;

class Channel {

public:
    
    uint8_t * pinPort;
    uint8_t pinMask;
    common_type ledType;
    uint8_t bwidth;
    
    //double buffer video memory
    uint8_t * buffer[ 2 ];
    
    uint8_t currentBuffer;
    uint8_t * currentBufferP = & currentBuffer;
    
    Channel( uint8_t pin , common_type commonType , uint8_t _bwidth );
    
    void copyBuffers();
    
    //Drawing primitives
    
    inline void line( uint16_t _pos ) {
        *( buffer[currentBuffer] + _pos / 8 ) |= ( 1 << _pos % 8 );
    }
    
    inline void fill( uint16_t x0 , uint16_t x1 ) {
        if ( x1 >= x0 ) {
            
            do {
                line( x1 );
                x1--;
            } while( x1 > x0 );
            
            line( x0 );
            
        } else {
            
            do {
                line( x0 );
                x0--;
            } while( x0 > x1 );
            
            line( x1 );
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
    
    inline void clear( uint16_t x0 , uint16_t x1 ) {
        if ( x1 >= x0 ) {
            
            do {
                clear( x1 );
                x1--;
            } while( x1 > x0 );
            
            clear( x0 );
            
        } else {
            
            do {
                clear( x0 );
                x0--;
            } while( x0 > x1 );
            
            clear( x1 );
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
    
    inline bool get( uint16_t _pos ) {
        return *( buffer[currentBuffer] + _pos / 8 ) & ( 1 << _pos % 8 ) ? true : false;
    }
    
    
private:
    
};

#endif
