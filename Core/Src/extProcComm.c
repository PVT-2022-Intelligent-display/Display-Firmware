/*
 * extProcComm.c
 *
 *  Created on: May 18, 2022
 *      Author: Nouth
 */

#include "uart.h"
#include <stdio.h>
#include <stdlib.h>
#include <stm32f4xx_hal.h>
#include <string.h>
#include "extFlash.h"
#include "configStructs.h"
#include "configLib.h"
#include "objectVisualization.h"

#define NEWLINE 13

//#define DEBUG_EXTERNAL_UART

/*
 * Expected input: sXXX string\r
 * s is command char. Added to enable different future commands.
 * XXX is ID of target object (decimal)
 * string is the string to be displayed in that object.
 * \r is carriage return (Ascii 13)
 *
 * Example: To make interactivelabel id 15 show the text "Outer Wilds", you should send:
 * s15 Outer Wilds\r
 */
int handleIncomingProcessorMsg(int screenObjectCount, struct object *objectArray, uint8_t **pointerArray){

	char msg[128];
	char reply[128];

	int count = read_usart_message(msg, &huart2, sizeof(msg), NEWLINE);
	msg[count] = 0;


	if(count == 0){
		//printf("[epc] No data on uart.\n\r");
		return 1;
	}

	char commandChar = msg[0];
	if(commandChar != 's'){
		printf("[epc] Unexpected command char. Only 's' is currently supported.\n\r");
#ifdef DEBUG_EXTERNAL_UART
		sprintf(reply,"[epc] Unexpected command char. Only 's' is currently supported.\n\r");
		send_usart_message(reply, &huart2, strlen(reply));
#endif
		return 100;
	}

	char numStr[10];
	int spaceIndex = 1;

	//find space marking end of ID number
	while(spaceIndex < count){
		if(msg[spaceIndex++] == ' '){
			break;
		}
	}

	if(spaceIndex == count){
		printf("[epc] Space not found in message. (%s)\n\r", msg);
#ifdef DEBUG_EXTERNAL_UART
		sprintf(reply,"[epc] Space not found in message.\n\r");
		send_usart_message(reply, &huart2, strlen(reply));
#endif
		return 100;
	}

	//copy number to new string and convert to int
	memcpy(numStr, msg+1, spaceIndex-1);
	numStr[spaceIndex] = 0;
	int soughtId = atoi(numStr);

	//sprintf(reply,"[epc] scrn cnt  %d, seek %d.\n\r", screenObjectCount, soughtId);
	//send_usart_message(reply, &huart2, strlen(reply));

	struct object targetObject;
	uint8_t *targetObjectDataPointer;
	int found = 0;

	int i = 0;
	while(1){
		targetObject = *(objectArray + i);
		//sprintf(reply,"[epc] [%d] check id: %d, type: %d.\n\r", i, targetObject.objectId, (int) targetObject.objectType);
		//while(send_usart_message(reply, &huart2, strlen(reply)) == 0){;}
		if(targetObject.objectId == soughtId){
			targetObjectDataPointer = *(pointerArray + i);
			found = 1;
			break;
		}
		i++;
		if(i==screenObjectCount){
			break;
		}
	}

	if(!found){
		printf("[epc] Object id %d not on this page.\n\r",soughtId);
#ifdef DEBUG_EXTERNAL_UART
		sprintf(reply,"[epc] Object id %d not on this page.\n\r",soughtId);
		send_usart_message(reply, &huart2, strlen(reply));
#endif
		return 200;
	}


	drawInteractiveLabelToLcd(targetObject, targetObjectDataPointer, (msg + spaceIndex));

	return 0;
}












