#include "Channel.h"

Channel::Channel( uint8_t pin , commonn_type commonType , uint8_t _bwidth ) {
    
    ledType = commonType;
    
    pinPort = portOutputRegister( digitalPinToPort( pin ) );
    pinMask = digitalPinToBitMask( pin );
    pinMode( pin , OUTPUT );
    
    bwidth = _bwidth;
    
    buffer[0] = calloc( _bwidth , 1); // Front buffer
    buffer[1] = calloc( _bwidth , 1); // Back buffer
    
}

/*
 * 
void Channel::copyBuffers() {
    
    for( uint8_t i = 0 ; i < bwidth ; i++ ) {
        *( backBuffer + i ) = *( buffer[currentBuffer] + i );
    }
    
}
*/
