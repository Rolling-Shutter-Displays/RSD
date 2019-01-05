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
