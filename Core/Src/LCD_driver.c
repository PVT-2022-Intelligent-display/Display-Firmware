/*
 * LCD_driver.c
 *
 *  Created on: 21 мар. 2022 г.
 *      Author: Carne
 */
#include <stm32f4xx.h>
#include <stm32f4xx_hal_spi.h>
#include <LCD_driver.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
//#include "iopins.h"
//#include "usart_routines.h"
//#include "stdio.h"
//#include "ext_flash.h"
//#include "spi_routines.h"
//#include "rtc_routines.h"
//#include "i2c_routines.h"
//#include "ILI9488/fsmc_ili9488.h"
//#include "ILI9488/config.h"
//#include "lcd/lcd_touch.h"
void initialize_ili9488();
void interrupt_initialize_priorities();
void iopins_ini();
void ili9488_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color);
void delay_ms(__IO uint32_t nCount1);
void ili9488_set_coordinates(uint16_t x1,uint16_t y1, uint16_t x2,uint16_t y2);

void Init_LCD()
{
	interrupt_initialize_priorities();		// initialize IRQ
	//*****************************************************************************
		iopins_ini();							// initialize IO pins

	//*****************************************************************************
		initialize_ili9488();					// initialize LCD
}

void interrupt_initialize_priorities()
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //4 bits for preemp priority 0 bit for sub priority
}

void iopins_ini()
{

	 __GPIOC_CLK_ENABLE();
	 __GPIOD_CLK_ENABLE();
	 __GPIOA_CLK_ENABLE();
	 __GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct;

    BUZZER_PORT_RCC();
	GPIO_InitStruct.Pin = BUZZER;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);

    RS485DIR_PORT_RCC();
	GPIO_InitStruct.Pin = RS485DIR;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(RS485DIR_PORT, &GPIO_InitStruct);

//  (LCD_BACKLIGHT_PORT_RCC();
//	GPIO_InitStruct.Pin = LCD_BACKLIGHT;
//	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	 HAL_GPIO_Init(LCD_BACKLIGHT_PORT, &GPIO_InitStruct);
//	HAL_GPIO_WritePin(LCD_BACKLIGHT_PORT,LCD_BACKLIGHT);

    LCD_CS_PORT_RCC();
	GPIO_InitStruct.Pin = LCD_CS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(LCD_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_CS_PORT,LCD_CS,GPIO_PIN_SET);

    LCD_RES_PORT_RCC();
	GPIO_InitStruct.Pin = LCD_RES;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(LCD_RES_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(LCD_RES_PORT,LCD_RES,GPIO_PIN_RESET);
	delay_ms(50);
	HAL_GPIO_WritePin(LCD_RES_PORT,LCD_RES,GPIO_PIN_SET);

    LCD_DCX_PORT_RCC();
	GPIO_InitStruct.Pin = LCD_DCX;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(LCD_DCX_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_DCX_PORT,LCD_DCX,GPIO_PIN_SET);

    FLASH_CS_PORT_RCC();
	GPIO_InitStruct.Pin = FLASH_CS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(FLASH_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(FLASH_CS_PORT,FLASH_CS,GPIO_PIN_SET);

    FLASH_WP_PORT_RCC();
	GPIO_InitStruct.Pin = FLASH_WP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(FLASH_WP_PORT, &GPIO_InitStruct);
	 HAL_GPIO_WritePin(FLASH_WP_PORT,FLASH_WP,GPIO_PIN_RESET);

    FRAM_CS_PORT_RCC();
	GPIO_InitStruct.Pin = FRAM_CS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(FRAM_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(FRAM_CS_PORT,FRAM_CS,GPIO_PIN_SET);

    FRAM_HOLD_PORT_RCC();
	GPIO_InitStruct.Pin = FRAM_HOLD;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed =GPIO_SPEED_FREQ_VERY_HIGH;
	 HAL_GPIO_Init(FRAM_HOLD_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(FRAM_HOLD_PORT,FRAM_HOLD,GPIO_PIN_SET);

    FRAM_WP_PORT_RCC();
	GPIO_InitStruct.Pin = FRAM_WP;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(FRAM_WP_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(FRAM_WP_PORT,FRAM_WP,GPIO_PIN_SET);


    LED_DBG_PORT_RCC();
	GPIO_InitStruct.Pin = LED_DBG_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(LED_DBG_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LED_DBG_PORT,LED_DBG_1,GPIO_PIN_SET);


    BUTTON_0_PORT_RCC();
	GPIO_InitStruct.Pin = BUTTON_0;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull= GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(BUTTON_0_PORT, &GPIO_InitStruct);

    BUTTON_1_PORT_RCC();
	GPIO_InitStruct.Pin = BUTTON_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull= GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(BUTTON_1_PORT, &GPIO_InitStruct);


    TOUCH_RESET_PORT_RCC();
	GPIO_InitStruct.Pin = TOUCH_RESET;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(TOUCH_RESET_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_SET);
}
//*****************************************************************************
void initialize_ili9488()
{
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x0011;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS, GPIO_PIN_RESET);
	TFT_REG=0x0029;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS, GPIO_PIN_SET);


	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x003A;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);
	TFT_REG=0x0055;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x0036;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);
	TFT_REG=0x00E8;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);

	delay_ms(100);
	ili9488_fillRect(0,0,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT,ORANGE);
}
void delay_ms(__IO uint32_t nCount1) {
	__IO uint32_t nCount=nCount1*23666;
	while(nCount--) {
  }
}
void ili9488_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color)
{
	uint32_t count = w * h;
	ili9488_set_coordinates(x1, y1, (uint16_t) (x1 + w - 1), (uint16_t) (y1 + h - 1));

	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x002C;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);

	for(unsigned int i=0; i<count; i++)
	{
		TFT_REG=color;
	}
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_SET);
}
void ili9488_set_coordinates(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2)
{
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x002A;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);
	TFT_REG=x1>>8;
	TFT_REG=x1&0xFF;
	TFT_REG=x2>>8;
	TFT_REG=x2&0xFF;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_SET);

	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x002B;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);
	TFT_REG=y1>>8;
	TFT_REG=y1&0xFF;
	TFT_REG=y2>>8;
	TFT_REG=y2&0xFF;
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_SET);
}
