/*
 * configStateLib.c
 *
 *  Created on: Apr 22, 2022
 *      Author: Petr
 */


#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <string.h>
#include "extFlash.h"
#include "configStructs.h"
#include "configLib.h"

#define NEWLINE 13
#define UART_READ_STRING() do{ count = read_usart_message(msg, &huart1, sizeof(msg), NEWLINE); } while(count==0); \
			msg[count-1] = 0

#define PRINT_OBJECT_HEADER(o) printf("[Type %2d; id %3d, beg<%3d:%3d>, end<%3d:%3d>, dLen %4d]\n\r", o.objectType, o.objectId, o.xstart, o.ystart, o.xend, o.yend, o.dataLen)

void objectTypeToString(objectType_t type, char *str);

objectType_t stringToObjectType(char *str);

int configScreensFromUart();
int configBitmapFromUart();
int configMarkBitmapsForDelete();
void reportBitmaps(int printHex);


/*
 * Attempts to read configuration / debug data from UART and save them to external flash.
 * Returns: 0 when successful, 1 when no configuration data detected on uart, 2 or greater when configuration failed
 */
int configFromUart(){

	char msg[128];

	int count = read_usart_message(msg, &huart1, sizeof(msg), NEWLINE);

	if(count == 0){
		printf("[cl] No data on uart.\n\r");
		return 1;
	}

	msg[count-1] = 0; //replace newline delimiter with null-termination

	char configMsg[] = "config";
	char bitmapMsg[] = "bitmap";
	char deleteMsg[] = "delete bitmaps";
	char sreportMsg[] = "report screens";
	char breportMsg[] = "report bitmaps";
	char hreportMsg[] = "hex report bitmaps";

	if(strcmp(configMsg, msg) == 0){
		printf("[cl] Okay, proceeding to configure screens.\n\r");
		return configScreensFromUart();
	}
	else if(strcmp(bitmapMsg, msg) == 0){
		printf("[cl] Okay, proceeding to write bitmap.\n\r");
		return configBitmapFromUart();
	}
	else if(strcmp(deleteMsg, msg) == 0){
		printf("[cl] Okay, marking all saved bitmaps for deletion.\n\r");
		return configMarkBitmapsForDelete();
	}
	else if(strcmp(sreportMsg, msg) == 0){
		printf("[cl] Okay, reporting screens.\n\r");
		struct generalConfig gconf;
		readGeneralConfig(&gconf);
		printAllScreens(gconf);
		return 0;
	}
	else if(strcmp(breportMsg, msg) == 0){
		printf("[cl] Okay, reporting bitmaps.\n\r");
		reportBitmaps(0);
		return 0;
		}
	else if(strcmp(hreportMsg, msg) == 0){
		printf("[cl] Okay, printing bitmap hex data...\n\r");
		reportBitmaps(1);
		return 0;
		}
	else{
		printf("[cl] Unexpected first msg. Got %s, which is not a recognised command.\n\r", msg);
		printf("[cl] Known commands: \n\r");
		printf("[cl] %s \n\r", configMsg);
		printf("[cl] %s \n\r", bitmapMsg);
		printf("[cl] %s \n\r", deleteMsg);
		printf("[cl] %s \n\r", sreportMsg);
		printf("[cl] %s \n\r", breportMsg);
		printf("[cl] %s \n\r", hreportMsg);
		return 1;
	}
}

int configBitmapFromUart(){
	int count;
	char msg[128];
	struct bitmapList blist;
	readBitmapList(&blist);
	int currentSector = blist.lastUsedSector + 1;
	if(currentSector > MAX_SECTOR){
		printf("[cl] Error trying to write bitmap to memory: No more free sectors. Last used: %d \n\r", blist.lastUsedSector);
		return 1000;
	}
	if(blist.totalBitmaps>=MAX_BITMAPS){
		printf("[cl] Error trying to write bitmap to memory: Maximum number of bitmaps (%d) exceeded. \n\r", MAX_BITMAPS);
		return 1001;
	}
	blist.bitmapSectors[blist.totalBitmaps] = currentSector;
 	blist.totalBitmaps++;

 	struct bitmap bm;

 	UART_READ_STRING();
 	bm.xsize = atoi(msg);
 	UART_READ_STRING();
 	bm.ysize = atoi(msg);

 	bm.bitmapNumber = blist.totalBitmaps-1;

 	int expectedBytes = 2*bm.xsize*bm.ysize;
 	int spaceLeft = SECTOR_SIZE*(MAX_SECTOR+1-currentSector) - sizeof(bm);

 	printf("[cl] Okay, ready to parse bitmap #%d, size %dx%d pixels.\n\r", bm.bitmapNumber, bm.xsize, bm.ysize);

 	if(expectedBytes > spaceLeft){
 		printf("[cl] WARNING: This bitmap is not going to fit into memory! Only first %d bytes of total %d will be written. \n\r", spaceLeft, expectedBytes);
 	}

 	uint8_t sectorBuffer[SECTOR_SIZE];
 	int sectorBufferIndex = 0;

 	memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &bm, sizeof(bm));
 	sectorBufferIndex += sizeof(bm);

 	int bytesLeft = expectedBytes;

 	while(bytesLeft > 0){
 		do{count = read_usart_message(msg, &huart1, 2, NEWLINE);} while (count==0);
 		if(count!=2){
 			printf("[cl] Error: Unexpected end of bitmap #%d data.\n\r.", bm.bitmapNumber);
 			break;
 		}
 		msg[2] = 0;
		uint8_t deHexedByte = (uint8_t) strtol(msg, NULL, 16);
		sectorBuffer[sectorBufferIndex] = deHexedByte;
		sectorBufferIndex++;
		bytesLeft--;
		if(sectorBufferIndex == SECTOR_SIZE || bytesLeft == 0){
			ext_flash_erase_4kB(currentSector*SECTOR_SIZE);
			ext_flash_write_multipage(currentSector*SECTOR_SIZE, sectorBuffer, sectorBufferIndex);
			sectorBufferIndex = 0;
			if(bytesLeft > 0){
				if(currentSector == MAX_SECTOR){
					printf("[cl] Warning: Ran out of external memory while writing bitmap #%d. Last %d bytes not saved. \n\r", bm.bitmapNumber, bytesLeft);
					break;
				}
				currentSector++;
			}
		}
 	}
 	read_usart_message(msg, &huart1, 2, NEWLINE); //get rid of newline left in buffer after reading last 2 chars

 	blist.lastUsedSector = currentSector;
 	memcpy(sectorBuffer, (uint8_t *) &blist, sizeof(blist));
 	ext_flash_erase_4kB(BITMAP_LIST_SECTOR*SECTOR_SIZE);
 	ext_flash_write_multipage(BITMAP_LIST_SECTOR*SECTOR_SIZE, sectorBuffer, sizeof(blist));

	if(bytesLeft > 0){
		printf("[cl] Bitmap #%d has been PARTIALLY written.\n\r", bm.bitmapNumber);
		return 1005;
	}
	printf("[cl] Bitmap #%d has been written.\n\r", bm.bitmapNumber);
	return 0;
}

int configMarkBitmapsForDelete(){
	ext_flash_erase_4kB(BITMAP_LIST_SECTOR*SECTOR_SIZE);
	struct bitmapList bl;
	bl.totalBitmaps = 0;
	bl.lastUsedSector = BITMAP_LIST_SECTOR;
	uint8_t buff[sizeof(bl)];
	memcpy(buff, (uint8_t *) &bl, sizeof(bl));
	ext_flash_write_multipage(BITMAP_LIST_SECTOR*SECTOR_SIZE, buff, sizeof(bl));
	printf("[cl] Bitmaps marked for delete.\n\r");
	return 0;
}


int configScreensFromUart(){
	int count;
	char msg[128];
	UART_READ_STRING(); //read general config - number of screens
	int totalScreens = atoi(msg);

	if(totalScreens < 1 || totalScreens >256){
		printf("[cl] Got %d, which is not a valid number of screens.\n\r", totalScreens);
		return 2;
	}

	struct generalConfig gconf;

	memset(&gconf.screenSectors, 0, sizeof(gconf.screenSectors));
	gconf.totalScreens = totalScreens;

	printf("[cl] Ok, expecting %d screens worth of data.\n\r", totalScreens);

	uint8_t currentScreenIndex = -1;
	uint16_t currentSector = GENERAL_CONFIG_SECTOR;
	uint16_t currentScreenObjectsLeft = 0;

	char screenStr[] = "screen";

	#define MAX_DATA_SIZE 8192
	uint8_t sectorBuffer[SECTOR_SIZE]; //holds sector before writing to external flash
	int sectorBufferIndex = 0;
	uint8_t dataBuffer[MAX_DATA_SIZE];
	int dataBufferIndex = 0;

	while(1){

		if(currentScreenIndex + 1 == totalScreens){
			printf("[cl] All screens' data received.\n\r");
			break;
		}

		UART_READ_STRING();

		if(strcmp(screenStr, msg) != 0){
			printf("[cl] Unexpected input - wanted %s, got %s. \n\r",screenStr, msg);
			return 3;
		}

		UART_READ_STRING();

		currentScreenObjectsLeft = atoi(msg);

		printf("[cl] Ok, expecting %d objects for current screen.\n\r", currentScreenObjectsLeft);

		struct screen screenVar;
		screenVar.objectCount = currentScreenObjectsLeft;
		screenVar.screenNumber = currentScreenIndex;

		currentSector += 1; //begin new sector -- screens are sector-aligned for easier access
		currentScreenIndex += 1;

		gconf.screenSectors[currentScreenIndex] = currentSector;
		ext_flash_erase_4kB(currentSector*SECTOR_SIZE);

		memset(sectorBuffer, 0, SECTOR_SIZE);
		sectorBufferIndex = 0;

		memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &screenVar, sizeof(screenVar));
		sectorBufferIndex += sizeof(screenVar);

		while(currentScreenObjectsLeft > 0){
			currentScreenObjectsLeft--;
			struct object currentObject;
			UART_READ_STRING();
			currentObject.objectType = stringToObjectType(msg);
			if(currentObject.objectType == none){
				printf("[cl] Object type %s not recognized. \n\r", msg);
				return 4;
			}
			UART_READ_STRING();
			currentObject.objectId = (uint16_t) atoi(msg);
			UART_READ_STRING();
			currentObject.xstart = (uint16_t) atoi(msg);
			UART_READ_STRING();
			currentObject.ystart = (uint16_t) atoi(msg);
			UART_READ_STRING();
			currentObject.xend = (uint16_t) atoi(msg);
			UART_READ_STRING();
			currentObject.yend = (uint16_t) atoi(msg);
			UART_READ_STRING();
			currentObject.dataLen = (uint16_t) atoi(msg);

			PRINT_OBJECT_HEADER(currentObject);

			if(currentObject.dataLen > MAX_DATA_SIZE){
				printf("[cl] Object (id=%d) data exceeds MAX_DATA_SIZE = %d.\n\r", currentObject.objectId, MAX_DATA_SIZE);
				return 5;
			}

			//try to append object header to buffer
			int spaceLeft = SECTOR_SIZE - sectorBufferIndex;

			if(spaceLeft >= sizeof(currentObject)){
				memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &currentObject, sizeof(currentObject));
				sectorBufferIndex += sizeof(currentObject);
			}
			else{
				//add what fits and begin new sector
				memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &currentObject, spaceLeft);
				ext_flash_write_multipage(currentSector*SECTOR_SIZE, sectorBuffer, SECTOR_SIZE);

				currentSector += 1;
				if(currentSector > MAX_SCREEN_SECTOR){
					printf("[cl] Screen memory size exceeded trying to save header of object id %d\n\r", currentObject.objectId);
					return 6;
				}
				ext_flash_erase_4kB(currentSector*SECTOR_SIZE);
				memset(sectorBuffer, 0, SECTOR_SIZE);
				sectorBufferIndex = 0;
				memcpy(sectorBuffer+sectorBufferIndex, ((uint8_t *) &currentObject)+spaceLeft, (sizeof(currentObject) - spaceLeft));
				sectorBufferIndex += (sizeof(currentObject) - spaceLeft);
			}

			//object header appended to buffer. Now move on to the hex data...

			uint16_t objectDataBytesLeft = currentObject.dataLen;
			dataBufferIndex = 0;

			while(objectDataBytesLeft > 0){
				do{count = read_usart_message(msg, &huart1, 2, NEWLINE);} while (count==0);
				if(count!=2){
					printf("[cl] Error reading hex data of object with id %d \n\r", currentObject.objectId);
					return 7;
				}
				msg[2] = 0;
				//two bytes representing hex string read.
				uint8_t deHexedByte = (uint8_t) strtol(msg, NULL, 16);
				//printf("REMOVEME: read %s -> %d \n\r", msg, deHexedByte);
				dataBuffer[dataBufferIndex] = deHexedByte;
				dataBufferIndex++;
				objectDataBytesLeft--;
			}
			read_usart_message(msg, &huart1, 2, NEWLINE); //to remove newline character from uart input buffer

			//dataBuffer now contains data of object, to be written to external flash.

			//transfer data to sectorBuffer so it can be written out:

			objectDataBytesLeft = currentObject.dataLen;
			dataBufferIndex = 0;

			while(objectDataBytesLeft > 0){
				spaceLeft = SECTOR_SIZE - sectorBufferIndex;
				if(spaceLeft >= objectDataBytesLeft){
					//data fits, just copy
					memcpy(sectorBuffer+sectorBufferIndex, dataBuffer + dataBufferIndex, objectDataBytesLeft);
					sectorBufferIndex += objectDataBytesLeft;
					objectDataBytesLeft = 0;
				}
				else{
					//data does not fit, copy what fits, write sector, start new sector
					memcpy(sectorBuffer+sectorBufferIndex, dataBuffer + dataBufferIndex, spaceLeft);
					dataBufferIndex += spaceLeft;
					objectDataBytesLeft -= spaceLeft;
					ext_flash_write_multipage(currentSector*SECTOR_SIZE, sectorBuffer, SECTOR_SIZE);
					currentSector += 1;
					if(currentSector > MAX_SCREEN_SECTOR){
						printf("[cl] Screen memory size exceeded while trying to save data of object id %d\n\r", currentObject.objectId);
						return 8;
					}
					ext_flash_erase_4kB(currentSector*SECTOR_SIZE);
					memset(sectorBuffer, 0, SECTOR_SIZE);
					sectorBufferIndex = 0;
				}
			}

			//loop above finished, meaning that all data of current object is either in external flash or in sectorbuffer

			//proceed to next object.
		}

		//all objects of current screen received

		ext_flash_write_multipage(currentSector*SECTOR_SIZE, sectorBuffer, sectorBufferIndex);
		//proceed to next screen
	}

	//all screens have been written.
	//Now save generic config, since sector address array has been completed:

	printf("Gconf screens %d \n\r", gconf.totalScreens);

	sectorBufferIndex = 0;
	memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &gconf, 100);//sizeof(gconf));

	printf("J ");
	int j;
	for(j=0; j<sizeof(gconf); j++){
		printf("%x ", sectorBuffer[j]);
	}

	ext_flash_erase_4kB(GENERAL_CONFIG_SECTOR*SECTOR_SIZE);
	ext_flash_write_multipage(GENERAL_CONFIG_SECTOR*SECTOR_SIZE, sectorBuffer, sizeof(gconf));
	printf("[cl] Config from UART finished. Furthest sector written to: %d \n\r", currentSector);

	return 0;

}

/*
 * Reads the general configuration struct from external flash into variable pointed at by @destination.
 */
void readGeneralConfig(struct generalConfig *destination){
	uint8_t buffer[sizeof(struct generalConfig)];
	ext_flash_read(GENERAL_CONFIG_SECTOR*SECTOR_SIZE, buffer, sizeof(struct generalConfig));
	*destination = *((struct generalConfig *) buffer);
	/*int i;
	for(i = 0; i<sizeof(struct generalConfig); i++){
		printf("%x,", buffer[i]);
	}
	printf("\n\r");*/
}

/*
 * Reads the list of bitmaps struct from external flash into variable pointed at by @destination.
 */
void readBitmapList(struct bitmapList *destination){
	uint8_t buffer[sizeof(struct bitmapList)];
	ext_flash_read(BITMAP_LIST_SECTOR*SECTOR_SIZE, buffer, sizeof(struct bitmapList));
	*destination = *((struct bitmapList *) buffer);
}

/*
 *	Reads screen from external flash, starting at sector number @screenSector. Its header is saved to *screenHeader.
 *
 *	Caller needs to provide @objectArray and @dataPointerArray, both at least @maxObjects long.
 *	Caller also needs to provide @dataArray, which will be used to store additional data of objects, and needs to be at least @maxData long.
 *
 *	Function will attempt to read first @maxObjects objects from stored screen. It will populate @objectArray with headers of individual objects and @dataPointerArray
 *	with pointers to data of individual objects (all pointers in @dataPointerArray will point into @dataArray).
 *
 *	@returns: number of objects read. If maxData and maxObjects are sufficient, this will be the same as @screenHeader.objectCount. If @maxObjects limit was reached,
 *	then return value will be equal to @maxObjects. If @maxData limit was reached, then return value will be lower than @maxObjects.
 *
 */
int openScreen(uint16_t screenSector, struct screen *screenHeader, struct object *objectArray, uint8_t *dataArray, uint8_t **dataPointerArray, uint16_t maxData, uint16_t maxObjects){
	uint32_t flashAddr = screenSector*SECTOR_SIZE;
	uint8_t screenHeaderBuffer[sizeof(struct screen)];
	ext_flash_read(flashAddr, screenHeaderBuffer, sizeof(struct screen));
	flashAddr += sizeof(struct screen);
	*screenHeader = *((struct screen *) screenHeaderBuffer);

	uint16_t objectsToRead = (*screenHeader).objectCount;
	uint16_t objectIndex = 0;
	uint16_t dataIndex = 0;
	if(objectsToRead > maxObjects){
		printf("[cl] Limiting objects to be read from screen to maxObjects! (down to %d from %d)", maxObjects, objectsToRead);
		objectsToRead = maxObjects;
	}

	uint8_t objectHeaderBuffer[sizeof(struct object)];
	while(objectIndex < objectsToRead){
		ext_flash_read(flashAddr, objectHeaderBuffer, sizeof(struct object));
		flashAddr += sizeof(struct object);
		*(objectArray + objectIndex) = *((struct object *) objectHeaderBuffer);
		uint16_t currentObjectDataLen = (objectArray + objectIndex)->dataLen;
		if(currentObjectDataLen > 0){
			if(dataIndex + currentObjectDataLen > maxData){
				printf("[cl] Ran out of data buffer when trying to add object #%d \n\r", objectIndex);
				return(objectIndex);
			}
			//object data fits in buffer
			ext_flash_read(flashAddr, (dataArray + dataIndex), currentObjectDataLen);
			*(dataPointerArray + objectIndex) = (dataArray + dataIndex);
			flashAddr += currentObjectDataLen;
			dataIndex += currentObjectDataLen;
			}
		else{
			*(dataPointerArray + objectIndex) = NULL;
		}

		objectIndex++;
	}

	return objectIndex;

}

/*
 * Reads bitmap from external flash, starting at sector number @bitmapSector. Its header is saved to *bitmapHeader.
 *
 * Caller needs to provide an array @dataArray at least @maxData pixels long. Each pixel is 2 bytes.
 *
 * @returns: number of pixels read. If maxPixels is sufficient, this will be the same as bitmapHeader.xsize*bitmapHeader.ysize.
 * */
int readBitmap(uint16_t bitmapSector, struct bitmap *bitmapHeader, uint16_t *dataArray, uint16_t maxPixels){
	uint32_t flashAddr = bitmapSector*SECTOR_SIZE;
	uint8_t bitmapHeaderBuffer[sizeof(struct bitmap)];
	ext_flash_read(flashAddr, bitmapHeaderBuffer, sizeof(struct bitmap));
	flashAddr += sizeof(struct bitmap);
	*bitmapHeader = *((struct bitmap *) bitmapHeaderBuffer);

	uint16_t pixelsToRead = (*bitmapHeader).xsize*(*bitmapHeader).ysize;
	if(pixelsToRead > maxPixels){
		printf("[cl] Limiting number of pixels of bitmap %d down to %d (from total size %d)\n\r", (*bitmapHeader).bitmapNumber, maxPixels, pixelsToRead);
		pixelsToRead = maxPixels;
	}

	uint16_t pixelIndex = 0;
	uint8_t pixelBuffer[2];
	while(pixelIndex < pixelsToRead){
		ext_flash_read(flashAddr, pixelBuffer, 2);
		*(dataArray + pixelIndex) = *((uint16_t *) pixelBuffer);
		flashAddr += 2;
		pixelIndex += 1;
	}
}

/*
 * debug function to check what is actually stored in flash
 */
void printAllScreens(struct generalConfig gconf){
	uint8_t screenIndex = 0;
	printf("[PAS] Supplied gconf says there are %d screens.\n\r", gconf.totalScreens);
	uint16_t maxObjects = 128;
	uint16_t maxData = SECTOR_SIZE*4;

	struct screen screenHeader;
	struct object objArr[maxObjects];
	uint8_t dataArr[maxData];
	uint8_t *pointerArray[maxObjects];

	while(screenIndex < gconf.totalScreens){
		printf("[PAS] Opening screen #%d stored @%d \n\r", screenIndex, gconf.screenSectors[screenIndex]);
		int objectsRead = openScreen(gconf.screenSectors[screenIndex], &screenHeader, objArr, dataArr, pointerArray, maxData, maxObjects);
		printf("[PAS] [%d / %d] objects have been read.\n\r", objectsRead, screenHeader.objectCount);
		printf("[PAS] Object report:\n\r");
		uint16_t objectIndex = 0;
		while(objectIndex < objectsRead){
			struct object thisObject = *(objArr + objectIndex);
			char typeStr[30];
			objectTypeToString(thisObject.objectType, typeStr);
			printf("[PAS] %s id=%d from (%d, %d) to (%d,%d), hex data (%db) =[", typeStr, thisObject.objectId, thisObject.xstart, thisObject.ystart, thisObject.xend, thisObject.yend, thisObject.dataLen);
			int dataIndex = 0;
			while(dataIndex < thisObject.dataLen){
				printf("%02x", *(pointerArray[objectIndex] + dataIndex));
				dataIndex++;
			}
			printf("]\n\r");
			objectIndex++;
		}
		printf("[PAS] End of screen #%d \n\r", screenIndex);
		screenIndex++;
	}

	printf("[PAS] Finished. \n\n\n\r");
}


void reportBitmaps(int printHex){
	struct bitmapList blist;
	readBitmapList(&blist);
	int total = blist.totalBitmaps;
	printf("[BR] There are %d bitmaps in memory spanning sectors %d to %d.\n\r", total, BITMAP_LIST_SECTOR+1, blist.lastUsedSector);
	int i = 0;
	while(i < total){
		int thisSector = blist.bitmapSectors[i];
		struct bitmap bm;
		uint16_t pixelBuffer[128];
		int pixelsRead = readBitmap(thisSector, &bm, pixelBuffer, 128);
		printf("[BR] Bitmap #%d stored @ sector %d is %dx%d pixels.\n\r", bm.bitmapNumber, thisSector, bm.xsize, bm.ysize);
		if(printHex){
			printf("[BR] It's first %d pixels are (newlines don't correspond to rows in bitmap!!):", pixelsRead);
			int printIndex = 0;
			while(printIndex < pixelsRead){
				if(printIndex%8==0){
					printf("\n\r    ");
				}
				printf("%04X ", pixelBuffer[printIndex++]);
			}
			printf("\n\r");

		}
		i++;
	}
	printf("[BR] Bitmap report finished.\n\r");
}



//-------------------------------------------------------------------------------

const char *typeNames[] 	= 		{"rectangle", "button", "label", "picture"};
const objectType_t types[] 	= 		{rectangle, button, label, picture};
const int typeCount = 3;

// Make sure that str is long enough for longest member of typeNames!
void objectTypeToString(objectType_t type, char *str){
	int i;
	for(i=0; i<typeCount; i++){
		if(type == types[i]){
			strcpy(str, typeNames[i]);
			return;
		}
	}

}

objectType_t stringToObjectType(char *str){
	int i;
	for(i=0; i<typeCount; i++){
		if(strcmp(typeNames[i], str) == 0){
			return types[i];
		}
	}
	return none;
}






