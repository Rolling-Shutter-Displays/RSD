#include "Screen.h"

Screen::Screen( Channel * red , Channel * green , Channel * blue ) {
    Red = red;
    Green = green;
    Blue = blue;
}

// Drawing primitives

void Screen::line( uint16_t _pos , colour c ) { 
	switch( c ) {
		case BLACK:
			Red->clear( _pos );
			Green->clear( _pos );
			Blue->clear( _pos );
			break;
		case RED:
			Red->line( _pos );
			Green->clear( _pos );
			Blue->clear( _pos );
			break;
		case GREEN:
			Red->clear( _pos );
			Green->line( _pos );
			Blue->clear( _pos );
			break;
		case BLUE:
			Red->clear( _pos );
			Green->clear( _pos );
			Blue->line( _pos );
			break;
		case YELLOW:
			Red->line( _pos );
			Green->line( _pos );
			Blue->clear( _pos );
			break;
		case CYAN:
			Red->clear( _pos );
			Green->line( _pos );
			Blue->line( _pos );
			break;
		case MAGENTA:
			Red->line( _pos );
			Green->clear( _pos );
			Blue->line( _pos );
			break;
		case WHITE:
			Red->line( _pos );
			Green->line( _pos );
			Blue->line( _pos );
			break;
		default: break;
	}

    
}

void Screen::fill( uint16_t x0 , uint16_t x1,  colour c ) {
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

void Screen::fill( colour c = WHITE ) {
	
    switch( c ) {
		case BLACK:
			Red->clear();
			Green->clear();
			Blue->clear();
			break;
        case RED:
			Red->fill();
			Green->clear();
			Blue->clear();
			break;
        case GREEN:
			Red->clear();
			Green->fill();
			Blue->clear();
			break;
		case BLUE:
			Red->clear();
			Green->clear();
			Blue->fill();
			break;
		case YELLOW:
			Red->fill();
			Green->fill();
			Blue->clear();
			break;
		case CYAN:
			Red->clear();
			Green->fill();
			Blue->fill();
			break;
		case MAGENTA:
			Red->fill();
			Green->clear();
			Blue->fill();
			break;
		case WHITE:
			Red->fill();
			Green->fill();
			Blue->fill();
			break;
		default: break;
	}
}

// Get functions

colour Screen::get( uint16_t _pos ) { 

	if( Red->get( _pos ) ) {
		
		if( Green->get( _pos ) ) {
			if ( Blue->get( _pos ) ) {
				return WHITE;
			} else {
				return YELLOW;
			}
		} else {
			if ( Blue->get( _pos ) ) {
				return MAGENTA;
			} else {
				return RED;
			}
		}

	} else if( Green->get( _pos ) ) {
		
		if( Blue->get( _pos ) ) {
			return CYAN;
		} else {
			return GREEN;
		}

	} else if ( Blue->get( _pos ) ) {
			return BLUE;
		} else {
			return BLACK;
	}
}
