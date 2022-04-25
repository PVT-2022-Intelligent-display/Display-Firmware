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

void objectTypeToString(objectType_t type, char *str);

objectType_t stringToObjectType(char *str);

void printAllScreens(struct generalConfig gconf);

#endif /* INC_CONFIGLIB_H_ */
