/*
 * objectVisualization.h
 *
 *  Created on: Apr 25, 2022
 *      Author: Nouth
 */

#ifndef INC_OBJECTVISUALIZATION_H_
#define INC_OBJECTVISUALIZATION_H_

void drawObjectToLcd(struct object o, uint8_t *data, int state);

void drawStringToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, int hSpacing, int vSpacing, char *string);

void drawCharToLcd5x7(int x, int y, int pixelScaling, uint16_t textColor, int useBackground, uint16_t bgColor, char c);




#endif /* INC_OBJECTVISUALIZATION_H_ */
