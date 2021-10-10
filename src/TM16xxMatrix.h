/*

TM16xxMatrix.h - LED Matrix class for TM16xx.
The TM16xxMatrix class supports LED matrices of various sizes to be connected
to a TM16xx chip, such as TM1640 or TM1638. 

These are the resolutions supported by the most popular TM16xx chips:
   TM1637  8x6  (common anode)
   TM1638  10x8  (common cathode)
   TM1640  8x16  (common cathode)
	 TM1668  10x7  (common cathode)   

Made by Maxint R&D. See https://github.com/maxint-rd/

*/
#ifndef _TM16XX_MATRIX_H
#define _TM16XX_MATRIX_H

#include "TM16xx.h"
#include <cstdint>

#define TM16XX_MATRIX_MAXCOLUMNS 16

class TM16xxMatrix
{
 public:
	TM16xxMatrix(TM16xx *pTM16xx, uint8_t nColumns, uint8_t nRows);
	void setColumn(uint8_t nCol, uint8_t bPixels);
	void setAll(bool fOn);
	void setPixel(uint8_t nCol, uint8_t nRow, bool fOn);
	bool getPixel(uint8_t nCol, uint8_t nRow);
	inline uint8_t getNumRows() { return(_nRows); }
	inline uint8_t getNumColumns() { return(_nColumns); }

 protected:
  TM16xx *_pTM16xx;
  uint8_t _nColumns;
  uint8_t _nRows;
  
  uint8_t _btColumns[TM16XX_MATRIX_MAXCOLUMNS]={0};
};
#endif
