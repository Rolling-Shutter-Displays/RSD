/*
RSD.h - A library to make rolling shutter displays
Copyright (c) 2018-2019 Facundo Daguerre (a.k.a derfaq).  All right reserved.
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
#include "Screen.h"

#define MAX_CHANNELS 12 //Maybe more, depends on a freq cam, resolution and f_cpu

extern volatile uint32_t frameCount;
extern volatile uint32_t frameLost; 

//Callback function types
extern "C" {
  typedef void ( *callbackFunction )( void );
}

class RSD {
	
	public:
        
    // Begin function. Needs to be called in the setup()
        
    static void begin( uint8_t f_cam , uint8_t _bwidth );
        
    // Attach function.
        
    static void attachChannel( Channel ch );
        
    // Update function. Needs to be called in the loop() 
        
    static void update();
    
    // Callback function attachment. Needs to be called in the setup(), and update() running in the loop()
    
    static void attachDraw( callbackFunction newFunction );

    // Tunning functions
    
    static uint16_t getTick();
    
    static uint16_t getLowerTick();

    static uint16_t getHigherTick();

    static uint16_t getFine();

    static uint16_t getLowerFine();

    static uint16_t getHigherFine();

    static bool setTick( int _tick ) ;

    static bool setFine( int _last );

    static uint32_t getPeriod();

    static float getFrequency();
        
    static void shiftPhase( int _phase );
        
        
private:
        
    //Timer one initialization method
    static void initTimer1();
        
};

#endif
