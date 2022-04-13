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
#define SLAVE_ADDRESS_TOUCH		0x90
#define I2C_DELAY				50

void touch_ini(void);
void register_touch_button(unsigned char index, unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned char *p);
void get_touched_button();
void clear_touched_button();
void touch_periodic_process();
char validate_touch_coordinates();
void Init_TOUCH(I2C_HandleTypeDef def);

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

extern touch_t touch;

void touch_write_control(unsigned char ctrl);
unsigned short touch_read(unsigned char ctrl);
void touch_reset();
void touch_write_register(uint8_t adr, uint16_t val);

extern char i2c_failure;

#endif/*TOUCH_DRIVERC_H_*/
