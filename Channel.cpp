/*
Channel.cpp - Part of RSD library.
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

#include "Channel.h"

Channel::Channel( uint8_t pin , common_type commonType , uint8_t _bwidth ) {
    
    ledType = commonType;
    
    pinPort = portOutputRegister( digitalPinToPort( pin ) );
    pinMask = digitalPinToBitMask( pin );
    pinMode( pin , OUTPUT );
    
    bwidth = _bwidth;
    
    // Allocate and clean memory for buffers
    buffer[0] = calloc( _bwidth , 1 );
    buffer[1] = calloc( _bwidth , 1 ); 
    
}

void Channel::copyBuffers() {
    
    for( uint8_t i = 0 ; i < bwidth ; i++ ) {
        *( buffer[ currentBuffer ] + i ) = *( buffer[ 1 - currentBuffer ] + i );
    }
    
}
