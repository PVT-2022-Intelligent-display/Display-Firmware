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
#define UART_READ_LINE() do{ count = read_usart_message(msg, &huart1, sizeof(msg), NEWLINE); } while(count==0); \
			msg[count-1] = 0

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

	UART_READ_LINE(); //read general config - number of screens

	int totalScreens = atoi(msg);

	if(totalScreens < 1 || totalScreens >256){
		printf("[cl] Got %d, which is not a valid number of screens.\n\r", totalScreens);
		return 2;
	}

	struct generalConfig gconf;

	memset(&gconf.screenSectors, 0, sizeof(gconf.screenSectors));
	gconf.totalScreens = totalScreens;

	printf("[cl] Ok, expecting %d screens worth of data.\n\r", totalScreens);

	uint8_t curentScreenIndex = 0;
	uint16_t currentScreenSector = GENERAL_CONFIG_SECTOR + 1;
	uint32_t currentScreenByteBudget = 0;
	uint16_t currentScreenObjectsLeft = 0;

	char screenStr[] = "screen";

	while(1){

		if(curentScreenIndex == totalScreens){
			printf("[cl] All screens' data received.\n\r");
			break;
		}

		UART_READ_LINE();

		if(strcmp(screenStr, msg) != 0){
			printf("[cl] Unexpected input - wanted %s, got %s. \n\r",screenStr, msg);
			return 3;
		}

		UART_READ_LINE();

		currentScreenObjectsLeft = atoi(msg);

		printf("[cl] Ok, expecting %d objects for current screen.\n\r", currentScreenObjectsLeft);

		struct screen screenVar;
		screenVar.objectCount = currentScreenObjectsLeft;
		screenVar.screenNumber = curentScreenIndex;

		ext_flash_erase_4kB(currentScreenSector);
		currentScreenByteBudget = sizeof(screenVar);

		//TODO write screen header

		while(currentScreenObjectsLeft > 0){
			struct object currentObject;
			UART_READ_LINE();
			currentObject.objectType = parseTypeFromString(msg);
			if(currentObject.objectType == none){
				printf("[cl] Object type %s not recognised. \n\r", msg);
				return 4;
			}
			UART_READ_LINE();
			currentObject.objectId = (uint16_t) atoi(msg);
			UART_READ_LINE();
			currentObject.xstart = (uint16_t) atoi(msg);
			UART_READ_LINE();
			currentObject.ystart = (uint16_t) atoi(msg);
			UART_READ_LINE();
			currentObject.xend = (uint16_t) atoi(msg);
			UART_READ_LINE();
			currentObject.yend = (uint16_t) atoi(msg);
			UART_READ_LINE();
			currentObject.dataLen = (uint16_t) atoi(msg);

		}




	}



	return 0;

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






