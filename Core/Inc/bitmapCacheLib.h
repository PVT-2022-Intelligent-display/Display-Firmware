/*
 * bitmapCacheLib.h
 *
 *  Created on: May 7, 2022
 *      Author: Nouth
 */

#ifndef INC_BITMAPCACHELIB_H_
#define INC_BITMAPCACHELIB_H_

#include <stdlib.h>
#include "configStructs.h"

extern struct bitmapList globalBitmapList;

int fetchBitmap(uint16_t bitmapSector, struct bitmap *bitmapHeader, uint16_t *dataArray, uint16_t maxPixels);


#endif /* INC_BITMAPCACHELIB_H_ */
