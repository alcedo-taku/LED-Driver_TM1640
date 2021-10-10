/*

TM16xxMatrix.h - LED Matrix library for TM16xx.

Made by Maxint R&D. See https://github.com/maxint-rd/

*/
#include "TM16xxMatrix.h"

TM16xxMatrix::TM16xxMatrix(TM16xx *pTM16xx, uint8_t nColumns, uint8_t nRows)
{
	_pTM16xx=pTM16xx;
	_nColumns=nColumns;
	_nRows=nRows;
	
	// offscreen bitmap is required to set an individual pixel, while retaining the others 
	// TODO: use dynamic memory allocation for the off-screen bitmap
	// as different chips support different sizes
}

void TM16xxMatrix::setColumn(uint8_t nCol, uint8_t bPixels)
{
	_btColumns[nCol]=bPixels;
	_pTM16xx->setSegments(bPixels, nCol);
}

void TM16xxMatrix::setAll(bool fOn)
{
	for(uint8_t nCol=0; nCol<_nColumns; nCol++)
		setColumn(nCol, fOn?0xFF:0);
}

void TM16xxMatrix::setPixel(uint8_t nCol, uint8_t nRow, bool fOn)
{
	uint8_t btColumn=_btColumns[nCol];
	// とりあえず動かすためにコメントアウト
//	if(fOn)
//		btColumn=btColumn | _BV(nRow);
//	else
//		btColumn=btColumn & ~_BV(nRow);
//	setColumn(nCol, btColumn);
}
	
bool TM16xxMatrix::getPixel(uint8_t nCol, uint8_t nRow)
{
	// とりあえず動かすためにコメントアウト
//	return((_btColumns[nCol]&_BV(nRow))!=0);
}
