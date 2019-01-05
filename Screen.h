#ifndef __Screen_RSD_library__
#define __Screen_RSD_library__

#if (ARDUINO >= 100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "Channel.h" 

typedef enum { BLACK , RED , GREEN , BLUE , YELLOW , CYAN , MAGENTA , WHITE } colour;

class Screen {

public:
    
    Channel * Red;
    Channel * Green;
    Channel * Blue;
    
    Screen( Channel * red , Channel * green , Channel * blue );
    
    void line( uint16_t _pos , colour c );
    
    void fill( uint16_t x0 , uint16_t x1,  colour c );
    
    void fill( colour c = WHITE );
    
    colour get( uint16_t _pos );
    
private:
    
};

#endif
