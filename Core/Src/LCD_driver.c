/*
 * LCD_driver.c
 *
 *  Created on: 21 мар. 2022 г.
 *      Author: Carne
 */
#include <stm32f4xx.h>
#include <stm32f4xx_hal_spi.h>
#include <LCD_driver.h>

static void LCD_pins_init(void) {

	GPIO_InitTypeDef GPIO_InitStruct;
    RCC_AHB1PeriphClockCmd(LCD_CS_PORT_RCC, ENABLE);
	GPIO_InitStruct.Pin = LCD_CS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH ;
	HAL_GPIO_Init(LCD_CS_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(LCD_CS_PORT_RCC,LCD_CS_PORT | LCD_CS, GPIO_PIN_SET);

    RCC_AHB1PeriphClockCmd(LCD_RES_PORT_RCC, ENABLE);
	GPIO_InitStruct.Pin = LCD_RES;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(LCD_RES_PORT, &GPIO_InitStruct);

	GPIO_ResetBits(LCD_RES_PORT,LCD_RES);
	delay_ms(50);
	GPIO_SetBits(LCD_RES_PORT,LCD_RES);

    RCC_AHB1PeriphClockCmd(LCD_DCX_PORT_RCC, ENABLE);
	GPIO_InitStruct.Pin = LCD_DCX;
	GPIO_InitStruct.Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.Speed = GPIO_Speed_50MHz;
	GPIO_InitStructOType = GPIO_OType_PP;
	GPIO_Init(LCD_DCX_PORT, &GPIO_InitStruct);
	GPIO_SetBits(LCD_DCX_PORT,LCD_DCX);

	//TFT CONTROLL LINES CONFIGURATIONS
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIOD, GPIOE, GPIOF, GPIOG and AFIO clocks */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE);

	/*-- GPIO Configuration ------------------------------------------------------*/
	/* SRAM Data lines,  NOE (/RD) and NWE (/WR) configuration */
	GPIO_InitStructure.Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.Mode = GPIO_Mode_AF;
	GPIO_InitStructure.Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

	GPIO_InitStructure.Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);

//	/* SRAM Address lines configuration (/RS)*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
//
//	/* NE3 configuration (/CS)*/
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
//
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_FSMC);

	/*/RESET */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD, GPIO_Pin_12);

	// INIT LCD BACKLIGHT PWM
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Clock for GPIOA */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Alternating functions for pins */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_TIM1);

	/* Set pins */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);


	TIM_TimeBaseInitTypeDef TIM_BaseStruct;

	/* Enable clock for TIM4 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
/*
	TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock
	But, timer has internal PLL, which double this frequency for timer, up to 84MHz
	Remember: Not each timer is connected to APB1, there are also timers connected
	on APB2, which works at 84MHz by default, and internal PLL increase
	this to up to 168MHz

	Set timer prescaller
	Timer count frequency is set with

	timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)

	In our case, we want a max frequency for timer, so we set prescaller to 0
	And our timer will have tick frequency

	timer_tick_frequency = 84000000 / (0 + 1) = 84000000
*/
	TIM_BaseStruct.TIM_Prescaler = 0;
	/* Count up */
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
/*
	Set timer period when it have reset
	First you have to know max value for timer
	In our case it is 16bit = 65535
	To get your frequency for PWM, equation is simple

	PWM_frequency = timer_tick_frequency / (TIM_Period + 1)

	If you know your PWM frequency you want to have timer period set correct

	TIM_Period = timer_tick_frequency / PWM_frequency - 1

	In our case, for 10Khz PWM_frequency, set Period to

	TIM_Period = 84000000 / 10000 - 1 = 8399

	If you get TIM_Period larger than max timer value (in our case 65535),
	you have to choose larger prescaler and slow down timer tick frequency
*/
    TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM1 */
    TIM_TimeBaseInit(TIM1, &TIM_BaseStruct);
	/* Start count on TIM1 */
    TIM_Cmd(TIM1, ENABLE);
//**************************************************************************
	TIM_OCInitTypeDef TIM_OCStruct;

	/* Common settings */

	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OCStruct.TIM_Pulse = 500; /* 100% duty cycle */
	TIM_OC4Init(TIM1, &TIM_OCStruct);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;
	TIM_BDTRStructInit(&TIM_BDTRInitStruct);
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStruct);
	TIM_CCPreloadControl(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

static void LCD_spi_init(void)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	// connect SPI1 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3);


	// enable peripheral clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);

	/* configure SPI1 in Mode 0
	 * CPOL = 1 --> clock is high when idle
	 * CPHA = 1 --> data is sampled at the second edge
	 */
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; // set to full duplex mode, seperate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     // transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; // one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        // clock is low
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      // data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; // set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;// data is transmitted MSB first
	SPI_Init(SPI3, &SPI_InitStruct);

	SPI_Cmd(SPI3, ENABLE); // enable SPI1
}
static void LCD_i2c_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6; // we are going to use PB8 and PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// init GPIOB

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_I2C1);	// SCL

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7; // we are going to use PB8 and PB9
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;			// set pins to alternate function
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// set GPIO speed
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;			// set output to open drain --> the line has to be only pulled low, not driven high
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;			// enable pull up resistors
	GPIO_Init(GPIOB, &GPIO_InitStruct);					// init GPIOB

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_I2C1); // SDA

	I2C_InitTypeDef I2C_InitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	I2C_InitStruct.I2C_ClockSpeed = 400000; 		// 100kHz
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;			// I2C mode
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	// 50% duty cycle --> standard
	I2C_InitStruct.I2C_OwnAddress1 = 0x00;			// own address, not relevant in master mode
	I2C_InitStruct.I2C_Ack = I2C_Ack_Disable;		// disable acknowledge when reading (can be changed later on)
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // set address length to 7 bit addresses
	I2C_Init(I2C1, &I2C_InitStruct);				// init I2C1

	// enable I2C1
	I2C_Cmd(I2C1, ENABLE);
}

void LCD_FSMC_config_write(void)
{

	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  p;
	FSMC_NORSRAMTimingInitTypeDef  p1;

	/* Enable FMC clock */
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM1 configuration */
	p.FSMC_AddressSetupTime = 2;//2;
	p.FSMC_AddressHoldTime = 1;
	p.FSMC_DataSetupTime = 4;//4;
	p.FSMC_BusTurnAroundDuration = 0;
	p.FSMC_CLKDivision = 1;
	p.FSMC_DataLatency = 0;
	p.FSMC_AccessMode = FSMC_AccessMode_A;
	/* Color LCD configuration ------------------------------------
	LCD configured as follow:
	- Data/Address MUX = Disable
	- Memory Type = SRAM
	- Data Width = 16bit
	- Write Operation = Enable
	- Extended Mode = Enable
	- Asynchronous Wait = Disable */

	/*-- FSMC Configuration ------------------------------------------------------*/
	/*----------------------- SRAM Bank 1 ----------------------------------------*/
	/* FSMC_Bank1_NORSRAM1 configuration */
	p1.FSMC_AddressSetupTime = 255;//2;
	p1.FSMC_AddressHoldTime = 1;
	p1.FSMC_DataSetupTime = 40;//4;
	p1.FSMC_BusTurnAroundDuration = 0;
	p1.FSMC_CLKDivision = 1;
	p1.FSMC_DataLatency = 0;
	p1.FSMC_AccessMode = FSMC_AccessMode_A;
	/* Color LCD configuration ------------------------------------
	LCD configured as follow:
	- Data/Address MUX = Disable
	- Memory Type = SRAM
	- Data Width = 16bit
	- Write Operation = Enable
	- Extended Mode = Enable
	- Asynchronous Wait = Disable */

	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p1;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

	/* Enable FSMC NOR/SRAM Bank1 */
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}
static void LCD_touch_ini(void )
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(LCD_PIN_PORT_RCC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = LCD_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LCD_PIN_PORT, &GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource4);

	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set EXTI5 Interrupt to the lowest priority */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_SetPriority(EXTI4_IRQn,2);
	touch_write_register(0x0E,0xC000);
	touch_write_control(0x80);
}
static void LCD_touch_write_register(unsigned char adr, unsigned int val)
{
	I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C1, adr<<3); // write one byte to the slave

	I2C_write(I2C1, val>>8); // write one byte to the slave
	I2C_write(I2C1, val&0xFF); // write one byte to the slave
	I2C_stop(I2C1); // stop the transmission
}
static void LCD_touch_write_control(unsigned char ctrl)
{
	I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	I2C_write(I2C1, ctrl); // write one byte to the slave
	I2C_stop(I2C1); // stop the transmission

}
static void LCD_ili9488_init(void)
{
	GPIO_ResetBits(LCD_DCX_PORT, LCD_DCX);
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);
	TFT_REG=0x0011;
	GPIO_SetBits(LCD_CS_PORT, LCD_CS);

	GPIO_ResetBits(LCD_DCX_PORT, LCD_DCX);
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);
	TFT_REG=0x0029;
	GPIO_SetBits(LCD_CS_PORT, LCD_CS);


	GPIO_ResetBits(LCD_DCX_PORT, LCD_DCX);
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);
	TFT_REG=0x003A;
	GPIO_SetBits(LCD_DCX_PORT, LCD_DCX);
	TFT_REG=0x0055;
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);

	GPIO_ResetBits(LCD_DCX_PORT, LCD_DCX);
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);
	TFT_REG=0x0036;
	GPIO_SetBits(LCD_DCX_PORT, LCD_DCX);
	TFT_REG=0x00E8;
	GPIO_ResetBits(LCD_CS_PORT, LCD_CS);

	delay_ms(100);
	ili9488_fillRect(0,0,LCD_getWidth(),LCD_getHeight(),BLACK);
}
/*--------------Public function definition----------------------------------------*/

void LCD_pwm_backlight_set(unsigned int pwm)
{
	if(pwm>=100)
	{
		pwm=100;
	}

	pwm=pwm*8399;
	pwm=pwm/100;

	TIM_OCInitTypeDef TIM_OCStruct;

	/* Common settings */

	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;

	TIM_OCStruct.TIM_Pulse = pwm; /* 100% duty cycle */
	TIM_OC4Init(TIM1, &TIM_OCStruct);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

}

void LCD_touch_reset()
{
	GPIO_ResetBits(TOUCH_RESET_PORT,TOUCH_RESET);
	delay_ms(10);
	GPIO_SetBits(TOUCH_RESET_PORT,TOUCH_RESET);
}

inline void LCD_fillRect(u16 x1, u16 y1, u16 w, u16 h, u16 color) {
    ili9488_fillRect(x1, y1, w ,h ,color);

//	u32 count = w * h;
//    LCD_setAddressWindowToWrite(x1, y1, (u16) (x1 + w - 1), (u16) (y1 + h - 1));
//    LCD_setSpi16();
//    dmaFill16(color, count);
//    LCD_setSpi8();
}


