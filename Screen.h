/*
Screen.h - Part of RSD library.
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

#ifndef __Screen_RSD_library__
#define __Screen_RSD_library__

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Channel.h" 

class Screen {

public:
    
    Screen( Channel *red , Channel *green , Channel *blue ) {
        Red = red;
        Green = green;
        Blue = blue;
    }
    
    void line( uint16_t _pos , colour c ) { 
        ( 0xCC & (1 << c) ) ? Red->line( _pos ) : Red->clear( _pos ) ;
        ( 0xF0 & (1 << c) ) ? Green->line( _pos ) : Green->clear( _pos ) ;
        ( 0xAA & (1 << c) ) ? Blue->line( _pos ) : Blue->clear( _pos ) ;
    }
    
    bool lineSafe( int16_t _pos , colour c ) { 
        ( 0xCC & (1 << c) ) ? Red->lineSafe( _pos ) : Red->clearSafe( _pos ) ;
        ( 0xF0 & (1 << c) ) ? Green->lineSafe( _pos ) : Green->clearSafe( _pos ) ;
        ( 0xAA & (1 << c) ) ? Blue->lineSafe( _pos ) : Blue->clearSafe( _pos ) ;
    }
    
    void fill( uint16_t x0 , uint16_t x1,  colour c ) {
        if ( x1 >= x0 ) {
            do {
                line( x1 , c );
                x1--;
            } while( x1 > x0 );
            
            line( x0 , c );
        } else {
            do {
                line( x0 , c );
                x0--;
            } while( x0 > x1 );
            
            line( x1 , c );
        }
    }
    
    bool fillSafe( int16_t x0 , int16_t x1,  colour c ) {
        if ( x1 >= x0 ) {
            do {
                lineSafe( x1 , c );
                x1--;
            } while( x1 > x0 );
            
            lineSafe( x0 , c );
        } else {
            do {
                lineSafe( x0 , c );
                x0--;
            } while( x0 > x1 );
            
            lineSafe( x1 , c );
        }
        return true; //TODO
    }
    
    void fill( colour c = WHITE ) {
        ( 0xCC & (1 << c) ) ? Red->fill() : Red->clear() ;
        ( 0xF0 & (1 << c) ) ? Green->fill() : Green->clear() ;
        ( 0xAA & (1 << c) ) ? Blue->fill() : Blue->clear() ;
    }
    
    void clear( uint16_t x0 ) {
        line( x0 , BLACK );
    }
    
    void clear( uint16_t x0 , uint16_t x1 ) {
        fill( x0 , x1 , BLACK );
    }
    
    bool clearSafe( int16_t x0 ) {
        
        lineSafe( x0 , BLACK );
        return true; //TODO
    }
    
    bool clearSafe( int16_t x0 , int16_t x1 ) {
        fillSafe( x0 , x1 , BLACK );
        return true; //TODO
    }
    
    void clear( ) {
        fill( BLACK );
    }
    
    colour get( uint16_t _pos ) { 
        uint8_t c = 0x00;
        if ( Red->get( _pos ) ) c += 2;
        if ( Green->get( _pos ) ) c += 4;
        if ( Blue->get( _pos ) ) c += 1;
        return (colour)c;
    }
    
    colour getSafe( int16_t _pos ) { 
        uint8_t c = 0x00;
        if ( Red->getSafe( _pos ) ) c += 2;
        if ( Green->getSafe( _pos ) ) c += 4;
        if ( Blue->getSafe( _pos ) ) c += 1;
        return (colour)c;
    }
    
private:
    
    Channel *Red;
    Channel *Green;
    Channel *Blue;
    
};

#endif
