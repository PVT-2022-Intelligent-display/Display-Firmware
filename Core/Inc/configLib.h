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


#endif /* INC_CONFIGLIB_H_ */
