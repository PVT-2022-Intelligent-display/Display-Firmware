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
#include "font.h"

void drawCharToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, char c);
void drawStringToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, int hSpacing, int vSpacing, char *string);


int drawObjectToLcd(struct object o, uint8_t *data, int state){
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

	if(o.objectType == rectangle){
		uint16_t color = BLACK;
		if(o.dataLen<2){
			printf("[OV] Warning: missing color byte in object id %d! Falling back on black. \n\r", o.objectId);
		}
		else{
			color = *((uint16_t *) data);
		}
		LCD_fillRect(o.xstart, o.ystart, (o.xend - o.xstart), (o.yend - o.ystart), color);
	}
	else if(o.objectType == picture){ //TODO fix data expectations
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
			LCD_fillRect(xpos, ypos, 1, 1, color);
			xpos++;
			if(xpos>o.xend){
				xpos = o.xstart;
				ypos++;
				if(ypos>o.yend){
					break;
				}
			}
		}
	}
	else if(o.objectType==label){
		int expectedChars = o.dataLen - 8;
		if(expectedChars < 0){
			printf("[OV] Error: Too few data bytes to draw string id %d. \n\r", o.objectId);
			return 3;
		}
		int dIndex = 0;
		uint8_t pixelScaling = *((uint8_t *) (data + dIndex++));
		uint8_t hSpace = *((uint8_t *) (data + dIndex++));
		uint8_t vSpace = *((uint8_t *) (data + dIndex++));
		uint8_t useBg = *((uint8_t *) (data + dIndex++));
		uint16_t textColor = *((uint16_t *) (data + dIndex));
		dIndex += 2;
		uint16_t bgColor = *((uint16_t *) (data + dIndex));
		dIndex += 2;

		char labelString[expectedChars + 1];
		int i;
		for(i = 0; i<expectedChars;i++){
			labelString[i] = *((uint8_t *) (data + dIndex++));
		}
		labelString[expectedChars] = 0;
		drawStringToLcd5x7(o.xstart, o.ystart, pixelScaling, textColor, useBg, bgColor, hSpace, vSpace, labelString);
	}
	else{
		printf("[OV] Error: drawObjectToLcd() not implemented for object type [%s]. \n\r", objName);
		return 100;
	}
	return 0;
}


/*
 * Draws a null-terminated string onto the LCD, starting at coordinates x,y, using 5x8 font (font is named 5x7 but is actually 5x8. Go figure.) scaled up by pixelscaling
 * (example - if pixelscaling == 5, a single char will occupy 25x40 pixels on the LCD). If useBackground is 1, then
 * background of chars will be filled with bgColor, otherwise, it will be left as-is.
 * hSpacing and vSpacing define the horizontal and vertical separation of chars in lcd pixels (independent of pixelScaling).
 * Newlines in string are treated as newline + cariiage return.
 */
void drawStringToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, int hSpacing, int vSpacing, char *string){

	uint16_t charX = x;
	uint16_t charY = y;

	int strIndex = 0;
	while(1){
		char c = *(string + strIndex++);
		if(c==0){
			break;
		}
		else if(c=='\r'){ //probably not needed, but just in case
			charX = x;
		}
		else if(c == '\n'){
			charX = x;
			charY += 8*pixelScaling + vSpacing;
		}
		else{ //normal character, presumably.
			if(charY > LCD_PIXEL_HEIGHT){
				break;
			}
			if(charX > LCD_PIXEL_WIDTH){
				continue; //because there might be a newline later in string
			}
			drawCharToLcd5x7(charX, charY, pixelScaling, textColor, useBackground, bgColor, c);
			charX += 5*pixelScaling + hSpacing;
		}
	}

}

void drawCharToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, char c){
	int ascii = (int) c;
	char *charFont = font5x7 + (5*ascii);
	int i;
	//printf("Drawing char %c @ [%d, %d] \n\r", c, x, y);

	//font is stored as columns!
	int column, row;
	for(column = 0; column < 5; column++){
		uint8_t columnByte = *(charFont + column);
		uint8_t mask = 0x01;
		for(row = 0; row < 8; row++){
			uint16_t pixelColor = bgColor;
			if(columnByte & mask){
				pixelColor = textColor;
			}
			else{
				if(!useBackground){
					mask = mask << 1;
					continue;
				}
			}

			uint16_t pixel_x_1 = x + pixelScaling*column;
			uint16_t pixel_y_1 = y + pixelScaling*row;
			uint16_t pixel_x_2 = pixel_x_1 + pixelScaling;
			uint16_t pixel_y_2 = pixel_y_1 + pixelScaling;

			if(pixel_x_1 >= LCD_PIXEL_WIDTH || pixel_y_1 >= LCD_PIXEL_HEIGHT){
				break;
			}

			if(pixel_x_2 >= LCD_PIXEL_WIDTH){
				pixel_x_2 = LCD_PIXEL_WIDTH - 1;
			}
			if(pixel_y_2 >= LCD_PIXEL_HEIGHT){
				pixel_y_2 = LCD_PIXEL_HEIGHT - 1;
			}

			LCD_fillRect(pixel_x_1, pixel_y_1, pixel_x_2-pixel_x_1, pixel_y_2 - pixel_y_1, pixelColor);
			mask = mask << 1;
		}
	}

}













