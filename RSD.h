/*
RSD.h - A library to create a rolling shutter display
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

#ifndef __RSD_library__
#define __RSD_library__

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


#define BWIDTH 32 //number of bytes
#define WIDTH (BWIDTH*8) //number of lines (bits)


#define F_CAM 30 //Assumes that the frequency of the camera is 30 hz
#define TICK F_CPU/(F_CAM*WIDTH) //initial frequency


typedef enum 
{
  // Used to indicate the RGB LED is a Common Cathode LED
  COMMON_CATHODE = 0,
  // Used to indicate the RGB LED is a Common Anode LED
  COMMON_ANODE   = 1,
} common_type; //From https://github.com/BretStateham/RGBLED/blob/master/RGBLED.h
			   //See too https://github.com/VaSe7u/RGBLED/blob/master/src/RGBLED.hpp

typedef enum 
{
  BLACK,RED,GREEN,BLUE,YELLOW,CYAN,MAGENTA,WHITE
} colour;

// ----- Callback function types -----

extern "C" {
  typedef void (*callbackFunction)(void);
}

//Some kind of doble buffer (db) video memory, it's *relative* safe to write here
//to be truly safe we need to implement some sort of signaling
extern uint8_t dbR[BWIDTH];
extern uint8_t dbG[BWIDTH];
extern uint8_t dbB[BWIDTH];
//Signalig variable
		/* 0: Busy, not copy, from scketch to library, processing over dataBuffers
		   1: Ready for copy, from scketch to library, end of processing
		   2: Beging of frame, from library to scketch, ready for process, quick go! */
extern  volatile uint8_t  frameStatus; 

class RSD {
	
	public:
		//Needs to be called in the setup()
		static void begin(uint8_t pinR, uint8_t pinG, uint8_t pinB, common_type commonType = COMMON_ANODE);

		//Update
		static void update();

		//Callback function
		static void attachDraw(callbackFunction newFunction);

		static inline void beginDraw() {
			frameStatus = 0;
		}

		static inline void endDraw() {
			frameStatus = 1;
		}


		//unsafe (not boundary checks) but eficient methods to draw a bar
		static inline void putBar(uint8_t *channel, uint16_t _pos) {
			channel[_pos / 8] |= (1 << _pos % 8);
		}

		static void putBar(colour Color, uint16_t _pos);

		static inline void clearBar(uint8_t *channel, uint16_t _pos) {
			channel[_pos / 8] &= ~(1 << _pos % 8);
		}

		static inline void clearBar(uint16_t _pos) {
			clearBar(dbR, _pos);
			clearBar(dbG, _pos);
			clearBar(dbB, _pos);

		}

		static inline bool getChannel(uint8_t *channel, uint16_t _pos) {
			return channel[_pos / 8]&&(1 << _pos % 8) ? true : false;
		}

		static colour getBar(uint16_t _pos);


		static inline clearChannel(uint8_t *channel) {
			for(uint8_t i = 0; i < BWIDTH; i++) {
				channel[i] = 0;
			}
		}

		static inline fillChannel(uint8_t *channel) {
			for(uint8_t i = 0; i < BWIDTH; i++) {
				channel[i] = 0xff;
			}	
		}

		static inline clearScreen() {
			clearChannel(dbR);
			clearChannel(dbG);
			clearChannel(dbB);
		}

		static inline fillScreen() {
			fillChannel(dbR);
			fillChannel(dbG);
			fillChannel(dbB);
		}

		static void fillScreen(colour Color);

		//Tunning functions
		static uint16_t getTick();
		
		static uint16_t getLowerTick();

		static uint16_t getHigherTick();

		static uint16_t getLastTick();

		static uint16_t getLowerLastTick();

		static uint16_t getHigherLastTick();

		static bool setTick(int _tick);

		static bool setLastTick(int _last);

		static uint32_t getPeriod();

		static float getFrequency();

		static void changePos(bool direction);

	private:

		//Timer one initialization method
		static void initTimer1();


};

#endif