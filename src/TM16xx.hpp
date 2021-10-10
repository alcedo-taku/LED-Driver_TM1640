/*
TM16xx.h - Library for TM1637, TM1638 and similar chips.

Copyright (C) 2011 Ricardo Batista <rjbatista at gmail dot com>
Modified by Maxint R&D. See https://github.com/maxint-rd/

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TM16XX_h
#define TM16XX_h

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include "Arduino.h"
//#else
//	#include "WProgram.h"
//#endif

#define TM16XX_CMD_DATA_AUTO 0x40  // 1000000
#define TM16XX_CMD_DATA_READ 0x42			// command to read data used on two wire interfaces of TM1637
#define TM16XX_CMD_DATA_FIXED 0x44 // 01000100
#define TM16XX_CMD_DISPLAY 0x80    // 10000000
#define TM16XX_CMD_ADDRESS 0xC0    // 11000000

#include "TM16xxFonts.h"
#include <cstdint>
#include <algorithm>
#include "gpio.h"

struct GPIO{
	GPIO_TypeDef * GPIOx;
	uint16_t GPIO_Pin;
};

class TM16xx{
public:
    /**
     * Instantiate a tm16xx module specifying data, clock and stobe pins,
     * the maximum number of displays supported by the chip, 
     * the number of digits used to display numbers or text, 
     * display state and the starting intensity (0-7).
     */
    TM16xx(GPIO dataPin, GPIO clockPin, GPIO strobePin, uint8_t maxDisplays, uint8_t digits);

    virtual void init(bool activateDisplay=true, uint8_t intensity=7);
    /** Set the display (segments and LEDs) active or off and intensity (range from 0-7). */
    virtual void setupDisplay(bool active, uint8_t intensity);

    /** Clear the display */
	virtual void clearDisplay();
	virtual void setSegments(uint8_t segments, uint8_t position);
	  
	  // Basic display functions. For additional display features use the TM16xxDisplay class
    /** Set a single display at pos (starting at 0) to a digit (left to right) */
	// digit その桁の値、pos どこの位置か、dot 小数点はつけるか、numberFont 数字のフォント
    virtual void setDisplayDigit(uint8_t digit, uint8_t pos=0, bool dot=false, const uint8_t numberFont[] = TM16XX_NUMBER_FONT);
	/** Set the display to a decimal number */
    virtual void setDisplayToDecNumber(int nNumber, uint8_t bDots=0);
	/** Clear  a single display at pos (starting at 0, left to right) */
    virtual void clearDisplayDigit(uint8_t pos, bool dot=false);
    /** Set the display to the values (left to right) */
    virtual void setDisplay(const uint8_t values[], uint8_t size=8);

    /** Set the display to the string (defaults to built in font) */
	virtual void setDisplayToString(const char* string, const uint16_t dots=0, const uint8_t pos=0, const uint8_t font[] = TM16XX_FONT_DEFAULT);
	virtual void sendChar(uint8_t pos, uint8_t data, bool dot); // made public to allow calling from TM16xxDisplay

	// Key-scanning functions
	// Note: not all TM16xx chips support key-scanning and sizes are different per chip
	// Up to 32 key states are supported, but specific chips may support less keys or less combinations
	// The chip specific derived class method will return a 32-bit value representing the state of each key, containing 0 if no key is pressed
	virtual uint32_t getButtons();  // return state of up to 32 keys.

protected:
//	virtual void sendChar(uint8_t pos, uint8_t data, boolean dot);
	virtual void bitDelay();
	virtual void start();
	virtual void stop();
	virtual void send(uint8_t data);
	virtual void sendCommand(uint8_t led);
	virtual void sendData(uint8_t add, uint8_t data);
	virtual uint8_t receive();
	uint8_t _maxDisplays=2;		// maximum number of digits (grids), chip-dependent
	uint8_t _maxSegments=8;		// maximum number of segments per display, chip-dependent

	uint8_t digits;		// number of digits in the display, module dependent
	GPIO dataPin;
	GPIO clockPin;
	GPIO strobePin;
};

#endif
