/*
TM16xx.h - Library for TM1637, TM1638 and similar chips.
Modified by Maxint R&D. See https://github.com/maxint-rd/

Copyright (C) 2011 Ricardo Batista (rjbatista <at> gmail <dot> com)

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

//#if defined(ARDUINO) && ARDUINO >= 100
//	#include "Arduino.h"
//#else
//	#include "WProgram.h"
//#endif

#include "TM16xx.hpp"
//#include "string.h"

TM16xx::TM16xx(GPIO dataPin, GPIO clockPin, GPIO strobePin, uint8_t maxDisplays, uint8_t digits)
{
	this->dataPin = dataPin;
	this->clockPin = clockPin;
	this->strobePin = strobePin;
	this->_maxDisplays = maxDisplays;
	this->digits = digits;

/*
	  sendCommand(TM16XX_CMD_DATA_AUTO);			// data command: set data mode to auto-increment write mode
		start();
	  send(TM16XX_CMD_ADDRESS);					// address command + address C0H
	  for (int i = 0; i < 16; i++) {		// TM1638 and TM1640 have 16 data addresses, TM1637 and TM1668 have less, but will wrap.
		send(0x00);
	  }
		stop();
*/	
	  // Note: calling these methods should be done in constructor of derived class in order to use properly initialized members!
/*
	clearDisplay();
	setupDisplay(activateDisplay, intensity);
*/
}

void TM16xx::init(bool activateDisplay,	uint8_t intensity){
	HAL_GPIO_WritePin(strobePin.GPIOx, strobePin.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(clockPin.GPIOx, clockPin.GPIO_Pin, GPIO_PIN_SET);
	clearDisplay();
	sendCommand(TM16XX_CMD_DISPLAY | (activateDisplay ? 8 : 0) | std::min((uint8_t)7, intensity));		// display command: on or intensity
}

void TM16xx::setupDisplay(bool active, uint8_t intensity)
{
	sendCommand(TM16XX_CMD_DISPLAY | (active ? 8 : 0) | std::min((uint8_t)7, intensity));
}

void TM16xx::clearDisplay()
{	// Clear all data registers. The number of registers depends on the chip.
	// TM1638 (10x8): 10 segments per grid, stored in two uint8_ts. The first uint8_t contains the first 8 display segments, second uint8_t has seg9+seg10  => 16 uint8_ts
	// TM1640 (8x16): one uint8_t per grid => 16 uint8_ts
	// TM1637 (8x6): one uint8_t per grid => 6 uint8_ts
	// TM1668 (10x7 - 14x3): two uint8_ts per grid => 14 uint8_ts
	sendCommand(TM16XX_CMD_DATA_AUTO);		// set auto increment addressing mode

	// send the address followed by bulk-sending of the data to clear the display memory
	start();
	send(TM16XX_CMD_ADDRESS);
	for (int i = 0; i < _maxDisplays; i++) {
		send(0x00);
		if(_maxSegments>8)
			send(0x00);		// send second uint8_t (applicable to TM1638 and TM1668)
	}
	stop();

}

void TM16xx::setSegments(uint8_t segments, uint8_t position)
{	// set 8 leds on common grd as specified
	// TODO: support 10-14 segments on chips like TM1638/TM1668
	if(position<_maxDisplays)
		sendData(position, segments);
		//sendData(TM16XX_CMD_ADDRESS | position, segments);
}

void TM16xx::sendChar(uint8_t pos, uint8_t data, bool dot)
{
/*
	if(pos<_maxDisplays)
	  sendData(pos, data | (dot ? 0b10000000 : 0));
*/
	setSegments(data | (dot ? 0b10000000 : 0), pos);
}

void TM16xx::setDisplayDigit(uint8_t digit, uint8_t pos, bool dot, const uint8_t numberFont[])
{
	// とりあえず動かすためにコメントアウト
	sendChar(pos, *(const unsigned char *)(numberFont + (digit & 0xF)), dot);
}

void TM16xx::setDisplayToDecNumber(int nNumber, uint8_t bDots)		// uint8_t bDots=0
{	// Function to display a decimal number on a n-digit clock display.
	// Kept simple to fit in ATtiny44A
	// For extended display features use the TM16xxDisplay class
	// 小数点の場所をdotで示している？

	// TODO: support large displays such as 8segx16 on TM1640
	for(uint8_t nPos=1; nPos<=digits; nPos++)
	{
		setDisplayDigit(nNumber % 10, digits - nPos, bDots&(1<<(nPos)));
		nNumber/=10;
	}
}

void TM16xx::clearDisplayDigit(uint8_t pos, bool dot)
{
	sendChar(pos, 0, dot);
}

void TM16xx::setDisplay(const uint8_t values[], uint8_t size)
{	// send an array of values to the display
	for (uint8_t i = 0; i < size; i++) {
		// とりあえず動かすためにコメントアウト
		sendChar(i, *(const unsigned char *)(values+i), 0);
	}
}

void TM16xx::setDisplayToString(const char* string, const uint16_t dots, const uint8_t pos, const uint8_t font[])
{
	for (int i = 0; i < digits - pos; i++) {
		if (string[i] != '\0') {
			// とりあえず動かすためにコメントアウト
			sendChar(i + pos, *(const unsigned char *)(font+(string[i] - 32)), (dots & (1 << (digits - i - 1))) != 0);
		} else {
			break;
		}
	}
}

// key-scanning method, implemented in chip specific derived class
uint32_t TM16xx::getButtons()
{	// return state of up to 32 keys.
	return(0);
}


//
//  Protected methods
//

void TM16xx::bitDelay()
{	// if needed derived classes can add a delay (eg. for TM1637)
	//delayMicroseconds(50);
//	asm("NOP");
//	uint16_t wait = 1;
//	wait *= 1;
//	do{
//		wait--;
//	}while(wait);
}

void TM16xx::start()
{	// if needed derived classes can use different patterns to start a command (eg. for TM1637)
	HAL_GPIO_WritePin(strobePin.GPIOx, strobePin.GPIO_Pin, GPIO_PIN_RESET);
	bitDelay();
}

void TM16xx::stop()
{	// if needed derived classes can use different patterns to stop a command (eg. for TM1637)
	HAL_GPIO_WritePin(strobePin.GPIOx, strobePin.GPIO_Pin, GPIO_PIN_SET);
	bitDelay();
}

void TM16xx::send(uint8_t data)
{
	// MMOLE 180203: shiftout does something, but is not okay (tested on TM1668)
	//shiftOut(dataPin, clockPin, LSBFIRST, data);
	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(clockPin.GPIOx, clockPin.GPIO_Pin, GPIO_PIN_RESET);
		bitDelay();
		HAL_GPIO_WritePin(dataPin.GPIOx, dataPin.GPIO_Pin, data & 1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
		bitDelay();
		data >>= 1;
		HAL_GPIO_WritePin(clockPin.GPIOx, clockPin.GPIO_Pin, GPIO_PIN_SET);
		bitDelay();
	}
	bitDelay();		// NOTE: TM1638 specifies a Twait between uint8_ts of minimal 1us.
}

void TM16xx::sendCommand(uint8_t cmd)
{
	start();
	send(cmd);
	stop();
}

void TM16xx::sendData(uint8_t address, uint8_t data)
{
	sendCommand(TM16XX_CMD_DATA_FIXED);							// use fixed addressing for data
	start();
	send(TM16XX_CMD_ADDRESS | address);						// address command + address
	send(data);
	stop();
}

uint8_t TM16xx::receive()
{
	uint8_t temp = 0;

	// Pull-up on
	HAL_GPIO_WritePin(dataPin.GPIOx, dataPin.GPIO_Pin, GPIO_PIN_SET);

	for (int i = 0; i < 8; i++) {
		temp >>= 1;

		HAL_GPIO_WritePin(clockPin.GPIOx, clockPin.GPIO_Pin, GPIO_PIN_RESET);
		bitDelay();		// NOTE: on TM1637 reading keys should be slower than 250Khz (see datasheet p3)

		if (HAL_GPIO_ReadPin(dataPin.GPIOx, dataPin.GPIO_Pin)) {
			temp |= 0x80;
		}

		HAL_GPIO_WritePin(clockPin.GPIOx, clockPin.GPIO_Pin, GPIO_PIN_SET);
		bitDelay();
	}

	// Pull-up off
	HAL_GPIO_WritePin(dataPin.GPIOx, dataPin.GPIO_Pin, GPIO_PIN_RESET);

	return temp;
}
