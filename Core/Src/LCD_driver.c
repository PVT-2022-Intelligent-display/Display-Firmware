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

void interrupt_initialize_priorities();
void iopins_ini();

void Init_LCD()
{
	interrupt_initialize_priorities();		// initialize IRQ
	//*****************************************************************************
		iopins_ini();							// initialize IO pins
		spi3_ini();								// initialize SPI LCD interface
		i2c_ini();								// initialize I2C

		TFT_CtrlLinesConfig();					// initialize TFT
		TFT_FSMCConfig_write();					// initialize TFT

		ini_lcd_pwm();							// initalize TFT backlight
		pwm_backlight_set(70);					// backlight full
		touch_reset();							// reset TP controller
		touch_ini();							// initialize TP IRQ
		touch_write_register(0x0E,0xC000);
		touch_write_control(0x80);

	//*****************************************************************************
		initialize_ili9488();					// initialize LCD
		dmaInit();								// initialize DMA
}

void interrupt_initialize_priorities()
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //4 bits for preemp priority 0 bit for sub priority
}

void iopins_ini()
{
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
void MX_SPI3_Init(void)
{
   SPI_HandleTypeDef hspi3;
  /* USER CODE BEGIN SPI3_Init 0 */

  /* USER CODE END SPI3_Init 0 */

  /* USER CODE BEGIN SPI3_Init 1 */

  /* USER CODE END SPI3_Init 1 */
  /* SPI3 parameter configuration*/
  hspi3.Instance = SPI3;
  hspi3.Init.Mode = SPI_MODE_MASTER;
  hspi3.Init.Direction = SPI_DIRECTION_2LINES;
  hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi3.Init.NSS = SPI_NSS_SOFT;
  hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi3.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI3_Init 2 */
//   void spi3_ini()
//   {
//   	GPIO_InitTypeDef GPIO_InitStruct;
//   	SPI_InitTypeDef SPI_InitStruct;

//   	// enable clock for used IO pins
//   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

//   	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_11 | GPIO_PIN_10;
//   	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;;
//   	GPIO_InitStruct.Speed = GPIO_Speed_50MHz;
//   	GPIO_InitStruct.Pull = GPIO_NOPULL;
//   	GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
//   	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);



//   	// enable peripheral clock
//   	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

//   	/* configure SPI1 in Mode 0
//   	 * CPOL = 1 --> clock is high when idle
//   	 * CPHA = 1 --> data is sampled at the second edge
//   	 */
//   	SPI_InitStruct.Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
//   	SPI_InitStruct.Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
//   	SPI_InitStruct.DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
//   	SPI_InitStruct.CLKPolarity = SPI_CPOL_Low;        // clock is low
//   	SPI_InitStruct.CLKPhase = SPI_CPHA_1Edge;      // data sampled at first edge
//   	SPI_InitStruct.NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
//   	SPI_InitStruct.BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI frequency is APB2 frequency / 4
//   	SPI_InitStruct.FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
//   	SPI_Init(SPI3, &SPI_InitStruct);

//   	SPI_Cmd(SPI3, ENABLE); // enable SPI1
//   }

  /* USER CODE END SPI3_Init 2 */

}
