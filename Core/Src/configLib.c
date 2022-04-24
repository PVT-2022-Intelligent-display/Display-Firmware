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


#define NEWLINE 13
#define UART_READ_STRING() do{ count = read_usart_message(msg, &huart1, sizeof(msg), NEWLINE); } while(count==0); \
			msg[count-1] = 0

#define PRINT_OBJECT_HEADER(o) printf("[Type %2d; id %3d, beg<%3d:%3d>, end<%3d:%3d>, dLen %4d]\n\r", o.objectType, o.objectId, o.xstart, o.ystart, o.xend, o.yend, o.dataLen)

objectType_t parseTypeFromString(char *str);

/*
 * Attempts to read configuration data from UART and save them to external flash.
 * Returns: 0 when configuration successful, 1 when no configuration data detected on uart, 2 or greater when configuration failed
 */
int configFromUart(){

	char msg[128];

	int count = read_usart_message(msg, &huart1, sizeof(msg), NEWLINE);



	if(count == 0){
		printf("[cl] No data on uart.\n\r");
		return 1;
	}

	msg[count-1] = 0; //replace newline delimiter with null-termination

	char expectedFirstMsg[] = "config";

	if(strcmp(expectedFirstMsg, msg) != 0){
		printf("[cl] Unexpected first msg. Got %s, wanted %s.\n\r", msg, expectedFirstMsg);
		return 1;
	}

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
		ext_flash_erase_4kB(currentSector);

		memset(sectorBuffer, 0, SECTOR_SIZE);
		sectorBufferIndex = 0;

		memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &screenVar, sizeof(screenVar));
		sectorBufferIndex += sizeof(screenVar);

		while(currentScreenObjectsLeft > 0){
			currentScreenObjectsLeft--;
			struct object currentObject;
			UART_READ_STRING();
			currentObject.objectType = parseTypeFromString(msg);
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
				printf("[cl] Object (id=%d) data too large. %d\n\r", currentObject.objectId);
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
				ext_flash_write(currentSector, sectorBuffer, SECTOR_SIZE);

				currentSector += 1;
				if(currentSector > MAX_SECTOR){
					printf("[cl] External memory size exceeded trying to save header of object id %d\n\r", currentObject.objectId);
					return 6;
				}
				ext_flash_erase_4kB(currentSector);
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
					ext_flash_write(currentSector, sectorBuffer, SECTOR_SIZE);
					currentSector += 1;
					if(currentSector > MAX_SECTOR){
						printf("[cl] External memory size exceeded while trying to save data of object id %d\n\r", currentObject.objectId);
						return 8;
					}
					ext_flash_erase_4kB(currentSector);
					memset(sectorBuffer, 0, SECTOR_SIZE);
					sectorBufferIndex = 0;
				}
			}

			//loop above finished, meaning that all data of current object is either in external flash or in sectorbuffer

			//proceed to next object.
		}

		//all objects of current screen received

		//save the last WIP sector to flash
		ext_flash_write(currentSector, sectorBuffer, SECTOR_SIZE);
		//proceed to next screen
	}

	//all screens have been written.
	//Now save generic config, since sector address array has been completed:

	printf("Gconf screens %d \n\r", gconf.totalScreens);

	sectorBufferIndex = 0;
	memcpy(sectorBuffer+sectorBufferIndex, (uint8_t *) &gconf, sizeof(gconf));

	int j;
	for(j=0; j<sizeof(gconf); j++){
		printf("%x ", sectorBuffer[j]);
	}

	ext_flash_erase_4kB(52);
	ext_flash_write(52, sectorBuffer, sizeof(gconf));
	printf("[cl] Config from UART finished. Furthest sector written to: %d \n\r", currentSector);

	return 0;

}

/*
 * Reads the general configuration struct from external flash into variable pointed at by @destination.
 */
void readGeneralConfig(struct generalConfig *destination){
	uint8_t buffer[sizeof(struct generalConfig)];
	ext_flash_read(52, buffer, sizeof(struct generalConfig));
	int i;
	for(i = 0; i<sizeof(struct generalConfig); i++){
		printf("%x,", buffer[i]);
	}
	printf("\n\r");
	*destination = *((struct generalConfig *) buffer);
}




objectType_t parseTypeFromString(char *str){
	const char *typeNames[] 	= 		{"rectangle", "button", "label"};
	const objectType_t types[] 	= 		{rectangle, button, label};

	int typeCount = 3;
	int i;

	for(i=0; i<typeCount; i++){
		if(strcmp(typeNames[i], str) == 0){
			return types[i];
		}
	}

	return none;
}






