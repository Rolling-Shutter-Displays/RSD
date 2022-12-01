/*
RSD.h - A library to make rolling shutter displays
Copyright (c) 2018-2022 Facundo Daguerre (a.k.a derfaq).  All right reserved.
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

#define MAX_CHANNELS 12 // This needs to be experimented. Maybe more, depends on a freq cam, resolution and f_cpu...

extern volatile uint32_t frameCount;
extern volatile uint32_t frameLost; 

// Callback function types
extern "C" {
  typedef void ( *callbackFunction )( void );
}

#include "Channel.h"
#include "Screen.h"

class RSD {

public:
        
    // Begin function. Needs to be called in the setup()
        
    static void begin( float _freq , uint8_t _bwidth );
        
    // Attach function.
        
    static void attachChannel( Channel *ch );
        
    // Update function. Needs to be called in the loop() 
        
    static void update();
    
    // Callback function attachment. Needs to be called in the setup(), and update() running in the loop()
    
    static void attachDraw( callbackFunction newFunction );

    // Tunning functions

    static void setFrequency( float freq );

    static float getFrequency();
    
    static uint32_t getPeriod();


    static bool setThick( uint16_t _thick ) ;
    
    static uint16_t getThick();
    
    static uint16_t getLowerThick();

    static uint16_t getHigherThick();


    static bool setFine( uint16_t _fine );

    static uint16_t getFine();

    static uint16_t getLowerFine();

    static uint16_t getHigherFine();


    static void shiftPhase( int _phase );

    // TODO: static void disconect()
           
private:
        
    // Timer one initialization method
    static void initTimer();
        
};

#endif