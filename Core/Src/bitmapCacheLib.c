/*
 * bitmapCacheLib.c
 *
 *  Created on: May 7, 2022
 *      Author: Nouth
 */

#include <stdlib.h>
#include "bitmapCacheLib.h"
#include "configLib.h"
#include "configStructs.h"

/*
 * Loads bitmap @bitmapNumber header into @bitmapHeader and it's pixels into @pixelArray, which is supplied by user and must be at least @maxPixels long.
 *
 * TODO: This function first attempts to fetch the bitmap from local cache, and failing that, loads it from external memory.
 *
 * @returns number of pixels read. When requested bitmap does not exist, zero is returned.
 *
 */
int fetchBitmap(uint16_t bitmapNumber, struct bitmap *bitmapHeader, uint16_t *pixelArray, uint16_t maxPixels){

	//TODO: bitmap caching


	if(bitmapNumber >= globalBitmapList.totalBitmaps){
		printf("[BCL] Can't fetch bitmap #%d, since total bitmaps = %d. \n\r", bitmapNumber, globalBitmapList.totalBitmaps);
		return 0;
	}

	uint16_t bitmapSector = globalBitmapList.bitmapSectors[bitmapNumber];
	int count = readBitmap(bitmapSector, bitmapHeader, pixelArray, maxPixels);
	return count;

}

