#ifndef TOUCH_DRIVERC_H_
#define TOUCH_DRIVERC_H_


#include <stm32f4xx_hal.h>
#include "configStructs.h"

#define		LCD_PIN_PORT				GPIOB
#define		LCD_PIN_PORT_RCC			RCC_AHB1Periph_GPIOB
#define		LCD_PIN						GPIO_Pin_4
#define 	MAX_NUM_ELEMENTS			20
#define 	MAX_NUM_LAYERS				5
#define 	MAX_NUM_PAGES				5

typedef struct
{
	uint16_t x,y;
	uint16_t x1,y1;
	uint16_t pressed;
	uint16_t idle_timer;
	uint8_t  element_type;
	uint8_t  isAlive;

} touch_t;

struct element_t
{
	struct object obj;
	uint16_t x,y;
	uint16_t x1,y1;
	uint8_t  element_type;
	uint8_t  isAlive;
	uint8_t *element_data_pointer;
	uint16_t ID;
};
void set_page(uint8_t page); // set page of the elements
void set_layer(uint8_t layer); // set layer of the elemens
struct element_t get_last_pressed_element();
uint8_t touch_register_element(uint8_t page,uint8_t layer,struct object obj, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t element_type, uint32_t element_pointer,uint16_t ID);
void touch_unregister_element(uint8_t page,uint8_t layer,uint8_t idx);
void touch_periodic_process();
void touch_init(I2C_HandleTypeDef def);
void touch_reset();


#endif/*TOUCH_DRIVERC_H_*/
