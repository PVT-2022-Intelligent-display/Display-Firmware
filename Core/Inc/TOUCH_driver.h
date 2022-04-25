#ifndef TOUCH_DRIVERC_H_
#define TOUCH_DRIVERC_H_


#include <stm32f4xx_hal.h>

#define		LCD_PIN_PORT				GPIOB
#define		LCD_PIN_PORT_RCC			RCC_AHB1Periph_GPIOB
#define		LCD_PIN						GPIO_Pin_4

#define		MIN_X					0
#define		MAX_X					1024
#define		MIN_Y					0
#define		MAX_Y					1024

#define		X_SCALE					0.46875
#define		Y_SCALE					0.3125

#define 	MAX_TP_BUTTONS			4
#define 	SLAVE_ADDRESS_TOUCH		0x90
#define 	I2C_DELAY				1000

typedef struct
{
	unsigned short x,y;
	unsigned short x1,y1;
	unsigned char pressed;
	unsigned int idle_timer;
	unsigned char 	key[4][8];
	unsigned short	coordinates[4][4];
	unsigned char 	key_pressed[8];
} touch_t;


//void get_touched_button();
//void clear_touched_button();
//char validate_touch_coordinates();

void touch_periodic_process();
void Init_TOUCH(I2C_HandleTypeDef def);
void touch_reset();

#endif/*TOUCH_DRIVERC_H_*/
