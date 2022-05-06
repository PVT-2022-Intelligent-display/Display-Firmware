/*
 * configLib.h
 *
 *  Created on: Apr 22, 2022
 *      Author: Petr
 */

#ifndef INC_CONFIGLIB_H_
#define INC_CONFIGLIB_H_

#include "configStructs.h"

int configFromUart();

void readGeneralConfig(struct generalConfig *destination);

void readBitmapList(struct bitmapList *destination);

void objectTypeToString(objectType_t type, char *str);

objectType_t stringToObjectType(char *str);

int openScreen(uint16_t screenSector, struct screen *screenHeader, struct object *objectArray, uint8_t *dataArray, uint8_t **dataPointerArray, uint16_t maxData, uint16_t maxObjects);


void printAllScreens(struct generalConfig gconf);

#endif /* INC_CONFIGLIB_H_ */
