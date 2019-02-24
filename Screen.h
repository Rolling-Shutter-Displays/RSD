/*
Screen.h - Part of RSD library.
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
    
    Screen( Channel *red , Channel *green , Channel *blue );
    
    void line( uint16_t _pos , colour c );
    
    void fill( uint16_t x0 , uint16_t x1,  colour c );
    
    void fill( colour c = WHITE );
    
    void clear( );
    
    
    colour get( uint16_t _pos );
    
private:
    
    Channel *Red;
    Channel *Green;
    Channel *Blue;
    
};

#endif
