/*
 * objectVisualization.c
 *
 *  Created on: Apr 25, 2022
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
#include "LCD_driver.h"

int drawObjectToLcd(struct object o, uint8_t *data){
	char objName[30];
	objectTypeToString(o.objectType, objName);

	//check that object coords make sense
	/*actually let's not waste time with it
	 * if(o.xend - o.xstart < 0 || o.yend - o.ystart < 0){
		printf("[OV] Error: width or height of object id %d is negative.\n\r", o.objectId);
		return 1;
	}
	*/

	printf("[OV] Drawing %s id %d \n\r", objName, o.objectId);

	switch(o.objectType){
	case rectangle:
		; //empty statement to shut compiler up
		uint16_t color = BLACK;
		if(o.dataLen<2){
			printf("[OV] Warning: missing color byte in object id %d! Falling back on black. \n\r", o.objectId);
		}
		else{
			color = *((uint16_t *) data);
		}
		ili9488_fillRect(o.xstart, o.ystart, (o.xend - o.xstart), (o.yend - o.ystart), color);
		break;

	case bitmap:
		; //empty statement to shut compiler up
		uint16_t xpos = o.xstart;
		uint16_t ypos = o.ystart;
		if(o.dataLen < (o.xend - o.xstart)*(o.yend - o.ystart)){
			printf("[OV] Error: %d pixel bytes needed for bitmap id %d but only got %d. \n\r", (o.xend - o.xstart)*(o.yend - o.ystart), o.objectId, o.dataLen);
			return 2;
		}
		uint16_t *colorArray = (uint16_t *) data;
		uint32_t dataIndex = 0;
		while(1){
			uint16_t color = *(colorArray + dataIndex);
			dataIndex++;
			ili9488_fillRect(xpos, ypos, 1, 1, color);
			xpos++;
			if(xpos>o.xend){
				xpos = o.xstart;
				ypos++;
				if(ypos>o.yend){
					break;
				}
			}
		}
		break;

	default:

		printf("[OV] Error: drawObjectToLcd() not implemented for object type [%s]. \n\r", objName);
		return 100;
	}
	return 0;
}


