/*
 * configStructs.h
 *
 *  Created on: Apr 22, 2022
 *      Author: Petr
 *
 *     	Structs used to save object to external flash
 *
 */

#ifndef INC_CONFIGSTRUCTS_H_
#define INC_CONFIGSTRUCTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stm32f4xx_hal.h>

#define MAX_SCREENS 256
#define MAX_BITMAPS 1024

//Struct holding general info about whole configuration
struct generalConfig{
	uint8_t totalScreens;
	uint16_t screenSectors[MAX_SCREENS]; //sectors of external flash at which individual screens begin. 0 when screen not configured.
};

struct bitmapList{
	uint16_t totalBitmaps;
	uint16_t lastUsedSector;
	uint16_t bitmapSectors[MAX_BITMAPS];
};

struct bitmap{
	uint16_t bitmapNumber;
	uint16_t xsize;
	uint16_t ysize;
};

struct screen{
	uint8_t screenNumber;
	uint16_t objectCount;
};


typedef enum{
	none,			//invalid value
	rectangle,		//monocolored rectangle
	button,			//button which is reported when pressed
	label,			//text label
	picture,		//used for displaying bitmaps from memory
	screenbutton		//button which when pressed changes current page (TODO Danylo)
} objectType_t;

struct object{
	objectType_t objectType;
	uint16_t objectId;
	uint16_t xstart;
	uint16_t ystart;
	uint16_t xend;
	uint16_t yend;
	uint16_t dataLen;
};



#endif /* INC_CONFIGSTRUCTS_H_ */
