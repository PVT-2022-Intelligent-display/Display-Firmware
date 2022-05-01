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
/*--------------------------------------------------MACRO define------------------------------------------------------*/

#define 	MAX_PWM_PULSE  			65535
#define 	TFT_REG      			(*((volatile unsigned short *) 0x60000000))

#define		LCD_BACKLIGHT_PORT		GPIOA
#define		LCD_BACKLIGHT_PORT_RCC()	__GPIOA_CLK_ENABLE();
#define		LCD_BACKLIGHT			GPIO_PIN_11

#define		LCD_CS_PORT				GPIOD
#define		LCD_CS_PORT_RCC()			 __GPIOD_CLK_ENABLE();
#define		LCD_CS					GPIO_PIN_7

#define		LCD_RES_PORT			GPIOA
#define		LCD_RES_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		LCD_RES					GPIO_PIN_12

#define		LCD_DCX_PORT			GPIOD
#define		LCD_DCX_PORT_RCC()		 __GPIOD_CLK_ENABLE();
#define		LCD_DCX					GPIO_PIN_13
#define		TOUCH_RESET_PORT		GPIOB
#define		TOUCH_RESET_PORT_RCC()	__GPIOB_CLK_ENABLE();
#define		TOUCH_RESET				GPIO_PIN_5

#define 	BUZZER_PORT				GPIOD
#define 	BUZZER_PORT_RCC()		__GPIOD_CLK_ENABLE();
#define 	BUZZER					GPIO_PIN_11

#define		BUTTON_0_PORT			GPIOA
#define		BUTTON_0_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		BUTTON_0				GPIO_PIN_8

#define		BUTTON_1_PORT			GPIOC
#define		BUTTON_1_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		BUTTON_1				GPIO_PIN_9

#define		RS485DIR_PORT			GPIOA
#define		RS485DIR_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		RS485DIR				GPIO_PIN_1

#define		ENCODER_PORT			GPIOC
#define		ENCODER_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		ENC_A					GPIO_PIN_7
#define		ENC_B					GPIO_PIN_6
#define		ENC_SW					GPIO_PIN_8

#define		LCD_BACKLIGHT_PORT		GPIOA
#define		LCD_BACKLIGHT_PORT_RCC()	 __GPIOA_CLK_ENABLE();
#define		LCD_BACKLIGHT			GPIO_PIN_11

#define		FLASH_WP_PORT			GPIOC
#define		FLASH_WP_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FLASH_WP				GPIO_PIN_4

#define		FLASH_CS_PORT			GPIOA
#define		FLASH_CS_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		FLASH_CS				GPIO_PIN_4

#define		LCD_CS_PORT				GPIOD
#define		LCD_CS_PORT_RCC()			 __GPIOD_CLK_ENABLE();
#define		LCD_CS					GPIO_PIN_7

#define		LCD_RES_PORT			GPIOA
#define		LCD_RES_PORT_RCC()		 __GPIOA_CLK_ENABLE();
#define		LCD_RES					GPIO_PIN_12

#define		LCD_DCX_PORT			GPIOD
#define		LCD_DCX_PORT_RCC()		 __GPIOD_CLK_ENABLE();
#define		LCD_DCX					GPIO_PIN_13

#define		LED_DBG_PORT			GPIOB
#define		LED_DBG_PORT_RCC()		 __GPIOB_CLK_ENABLE();
#define		LED_DBG_1				GPIO_PIN_3

#define		FRAM_WP_PORT			GPIOC
#define		FRAM_WP_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FRAM_WP					GPIO_PIN_0

#define		FRAM_HOLD_PORT			GPIOC
#define		FRAM_HOLD_PORT_RCC()		 __GPIOC_CLK_ENABLE();
#define		FRAM_HOLD				GPIO_PIN_1

#define		FRAM_CS_PORT			GPIOB
#define		FRAM_CS_PORT_RCC()		 __GPIOB_CLK_ENABLE();
#define		FRAM_CS					GPIO_PIN_9



#define 	ENC_FORWARD     0
#define 	ENC_BACKWARD    1

#define 	ENC_NOREADY     0
#define 	ENC_READY       1
#define 	ENC_INIT        3

/* Level 1 Commands */
#define LCD_SWRESET             0x01   /* Software Reset */
#define LCD_READ_DISPLAY_ID     0x04   /* Read display identification information */
#define LCD_RDDST               0x09   /* Read Display Status */
#define LCD_RDDPM               0x0A   /* Read Display Power Mode */
#define LCD_RDDMADCTL           0x0B   /* Read Display MADCTL */
#define LCD_RDDCOLMOD           0x0C   /* Read Display Pixel Format */
#define LCD_RDDIM               0x0D   /* Read Display Image Format */
#define LCD_RDDSM               0x0E   /* Read Display Signal Mode */
#define LCD_RDDSDR              0x0F   /* Read Display Self-Diagnostic Result */
#define LCD_SPLIN               0x10   /* Enter Sleep Mode */
#define LCD_SLEEP_OUT           0x11   /* Sleep out register */
#define LCD_PTLON               0x12   /* Partial Mode ON */
#define LCD_NORMAL_MODE_ON      0x13   /* Normal Display Mode ON */
#define LCD_DINVOFF             0x20   /* Display Inversion OFF */
#define LCD_DINVON              0x21   /* Display Inversion ON */
#define LCD_GAMMA               0x26   /* Gamma register */
#define LCD_DISPLAY_OFF         0x28   /* Display off register */
#define LCD_DISPLAY_ON          0x29   /* Display on register */
#define LCD_COLUMN_ADDR         0x2A   /* Colomn address register */
#define LCD_PAGE_ADDR           0x2B   /* Page address register */
#define LCD_GRAM                0x2C   /* GRAM register */
#define LCD_RGBSET              0x2D   /* Color SET */
#define LCD_RAMRD               0x2E   /* Memory Read */
#define LCD_PLTAR               0x30   /* Partial Area */
#define LCD_VSCRDEF             0x33   /* Vertical Scrolling Definition */
#define LCD_TEOFF               0x34   /* Tearing Effect Line OFF */
#define LCD_TEON                0x35   /* Tearing Effect Line ON */
#define LCD_MAC                 0x36   /* Memory Access Control register*/
#define LCD_VSCRSADD            0x37   /* Vertical Scrolling Start Address */
#define LCD_IDMOFF              0x38   /* Idle Mode OFF */
#define LCD_IDMON               0x39   /* Idle Mode ON */
#define LCD_PIXEL_FORMAT        0x3A   /* Pixel Format register */
#define LCD_WRITE_MEM_CONTINUE  0x3C   /* Write Memory Continue */
#define LCD_READ_MEM_CONTINUE   0x3E   /* Read Memory Continue */
#define LCD_SET_TEAR_SCANLINE   0x44   /* Set Tear Scanline */
#define LCD_GET_SCANLINE        0x45   /* Get Scanline */
#define LCD_WDB                 0x51   /* Write Brightness Display register */
#define LCD_RDDISBV             0x52   /* Read Display Brightness */
#define LCD_WCD                 0x53   /* Write Control Display register*/
#define LCD_RDCTRLD             0x54   /* Read CTRL Display */
#define LCD_WRCABC              0x55   /* Write Content Adaptive Brightness Control */
#define LCD_RDCABC              0x56   /* Read Content Adaptive Brightness Control */
#define LCD_WRITE_CABC          0x5E   /* Write CABC Minimum Brightness */
#define LCD_READ_CABC           0x5F   /* Read CABC Minimum Brightness */
#define LCD_READ_ID1            0xDA   /* Read ID1 */
#define LCD_READ_ID2            0xDB   /* Read ID2 */
#define LCD_READ_ID3            0xDC   /* Read ID3 */

/* Level 2 Commands */
#define LCD_RGB_INTERFACE       0xB0   /* RGB Interface Signal Control */
#define LCD_FRMCTR1             0xB1   /* Frame Rate Control (In Normal Mode) */
#define LCD_FRMCTR2             0xB2   /* Frame Rate Control (In Idle Mode) */
#define LCD_FRMCTR3             0xB3   /* Frame Rate Control (In Partial Mode) */
#define LCD_INVTR               0xB4   /* Display Inversion Control */
#define LCD_BPC                 0xB5   /* Blanking Porch Control register */
#define LCD_DFC                 0xB6   /* Display Function Control register */
#define LCD_ETMOD               0xB7   /* Entry Mode Set */
#define LCD_BACKLIGHT1          0xB8   /* Backlight Control 1 */
#define LCD_BACKLIGHT2          0xB9   /* Backlight Control 2 */
#define LCD_BACKLIGHT3          0xBA   /* Backlight Control 3 */
#define LCD_BACKLIGHT4          0xBB   /* Backlight Control 4 */
#define LCD_BACKLIGHT5          0xBC   /* Backlight Control 5 */
#define LCD_BACKLIGHT7          0xBE   /* Backlight Control 7 */
#define LCD_BACKLIGHT8          0xBF   /* Backlight Control 8 */
#define LCD_POWER1              0xC0   /* Power Control 1 register */
#define LCD_POWER2              0xC1   /* Power Control 2 register */
#define LCD_VCOM1               0xC5   /* VCOM Control 1 register */
#define LCD_VCOM2               0xC7   /* VCOM Control 2 register */
#define LCD_NVMWR               0xD0   /* NV Memory Write */
#define LCD_NVMPKEY             0xD1   /* NV Memory Protection Key */
#define LCD_RDNVM               0xD2   /* NV Memory Status Read */
#define LCD_READ_ID4            0xD3   /* Read ID4 */
#define LCD_PGAMMA              0xE0   /* Positive Gamma Correction register */
#define LCD_NGAMMA              0xE1   /* Negative Gamma Correction register */
#define LCD_DGAMCTRL1           0xE2   /* Digital Gamma Control 1 */
#define LCD_DGAMCTRL2           0xE3   /* Digital Gamma Control 2 */
#define LCD_INTERFACE           0xF6   /* Interface control register */

/* Extend register commands */
#define LCD_POWERA              0xCB   /* Power control A register */
#define LCD_POWERB              0xCF   /* Power control B register */
#define LCD_DTCA                0xE8   /* Driver timing control A */
#define LCD_DTCB                0xEA   /* Driver timing control B */
#define LCD_POWER_SEQ           0xED   /* Power on sequence register */
#define LCD_3GAMMA_EN           0xF2   /* 3 Gamma enable register */
#define LCD_PRC                 0xF7   /* Pump ratio control register */

/*----------------------------------------------Private functions defenition------------------------------------------*/
static void initialize_ili9488();
static void interrupt_initialize_priorities();
static void delay_ms(__IO uint32_t nCount1);
static void iopins_ini();
static void ili9488_showArray_wo_coordinates(uint16_t *data, uint32_t count);
/*----------------------------------------------Public  functions-----------------------------------------------------*/
//initialize LCD
void Init_LCD()
{
	interrupt_initialize_priorities();
	iopins_ini();
	initialize_ili9488();
	LCD_backlight_set(100);
}
//Draw filled rectangle
void LCD_fillRect(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t color)
{
	uint32_t count = w * h;
	LCD_set_coordinates(x1, y1, (uint16_t) (x1 + w - 1), (uint16_t) (y1 + h - 1));

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
//Set coordinates for LCD IC
void LCD_set_coordinates(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2)
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
//Set PWM duty cycle for LCD back light
void LCD_backlight_set(uint16_t pwm)
{

	if(pwm>=100)
	{
		pwm=100;
	}else if(pwm <= 0)
	{
		pwm = 0;
	}

	pwm=(unsigned int)((MAX_PWM_PULSE * pwm)/100);

	/* Common settings */
      TIM_HandleTypeDef htim1;
	  TIM_MasterConfigTypeDef sMasterConfig = {0};
	  TIM_OC_InitTypeDef sConfigOC = {0};
	  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	  htim1.Instance = TIM1;
	  htim1.Init.Prescaler = 0;
	  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim1.Init.Period = 65535;
	  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim1.Init.RepetitionCounter = 0;
	  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	  HAL_TIM_PWM_Stop(&htim1,TIM_CHANNEL_4);
	  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = pwm;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	  sBreakDeadTimeConfig.DeadTime = 0;
	  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  HAL_TIM_MspPostInit(&htim1);
	  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);


}
//Write one pixel to LCD
inline void LCD_WritePixel(uint16_t x, uint16_t y, uint16_t color){
	LCD_set_coordinates(x,y,x,y);
	ili9488_showArray_wo_coordinates(color,1);
}

/*----------------------------------------------Private functions-----------------------------------------------------*/
static void interrupt_initialize_priorities()
{
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4); //4 bits for preemp priority 0 bit for sub priority
}
static void iopins_ini()
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
static void initialize_ili9488()
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
	LCD_fillRect(0,0,LCD_PIXEL_WIDTH,LCD_PIXEL_HEIGHT,ORANGE);
}
static void ili9488_showArray_wo_coordinates(uint16_t *data, uint32_t count)
{
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_RESET);
	TFT_REG=0x002C;
	HAL_GPIO_WritePin(LCD_DCX_PORT, LCD_DCX,GPIO_PIN_SET);

	for(unsigned int i=0; i<count; i++)
	{
		TFT_REG=data[i];
	}
	HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS,GPIO_PIN_SET);
}
static void delay_ms(__IO uint32_t nCount1) {
	__IO uint32_t nCount=nCount1*23666;
	while(nCount--) {
  }
}
