
#include "TOUCH_driver.h"
#include "uart.h"
#include <stdio.h>
#include <stm32f4xx_hal_gpio.h>
#include "LCD_driver.h"
I2C_HandleTypeDef hi2c1_touch;
touch_t touch;
static unsigned short touch_read(unsigned char adr);
static void touch_write_control(unsigned char ctrl);
static void touch_write_register(uint8_t adr, uint16_t val);
static void delay_ms(__IO uint32_t nCount1);

void Init_TOUCH(I2C_HandleTypeDef def)
{
	hi2c1_touch = def;
	touch_write_register(0x0E,0xC000);
	touch_write_control(0x80);
}
static void touch_write_control(unsigned char ctrl)
{
	HAL_StatusTypeDef status =0;
	status = HAL_I2C_Master_Transmit(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH, &ctrl, 1 ,I2C_DELAY);
	//I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	//I2C_write(I2C1, ctrl); // write one byte to the slave
	//I2C_stop(I2C1); // stop the transmissio;
	printf("Write control I2C\n\r");
	printf ("%d",status);
}
static void touch_write_register(uint8_t adr, uint16_t val)
{

	HAL_StatusTypeDef status =0;
	uint8_t data[2];
	data[0] = val>>8;
	data[1] = (uint8_t)val;
	status = HAL_I2C_Mem_Write(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH,(adr<<3),I2C_MEMADD_SIZE_8BIT,data,2,I2C_DELAY);
	printf ("I2C write register\n\r");
	printf ("status %d \n\r",status);
}
static unsigned short touch_read(unsigned char adr)
{
	unsigned char adr_i2c= 0x01;
	adr_i2c+=adr<<3;
	printf("%d",adr_i2c);
	uint8_t data[2];
	HAL_StatusTypeDef status =0;
	//status = HAL_I2C_Master_Transmit(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH + 0x01, &adr_i2c, 1 ,I2C_DELAY);
	status = HAL_I2C_Mem_Read(&hi2c1_touch, (uint16_t)SLAVE_ADDRESS_TOUCH ,adr_i2c,I2C_MEMADD_SIZE_8BIT,data, 2, I2C_DELAY);
	uint16_t result = (data[0] << 8) | (data[1] & 0xFF);
	return result;
}
void touch_reset()
{
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_RESET);
 	delay_ms(10);
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_SET);
 	delay_ms(50);
}
//**************************************************************************
void touch_periodic_process()
{

	if(touch.pressed==1)
	{
		touch.pressed=0;

		touch.x=touch_read(0x00);
		touch.x1=touch_read(0x01);
		touch.y=touch_read(0x02);
		touch.y1=touch_read(0x03);
		printf("TOUCH x %d\n\r", touch.x);
		printf("TOUCH x1 %d\n\r", touch.x1);
		printf("TOUCH y %d\n\r", touch.y);
		printf("TOUCH y1 %d\n\r", touch.y1);
		touch.y=touch.x1;
		touch.x=1024-touch.y1;

		//if(validate_touch_coordinates()==1)
		//{
			//if(touch_ignore<rtc)
			//{
				printf("TOUCH = %d,%d\n\r",touch.x,touch.y);
				//USART_puts(DBG_UART,serial_data,strlen(serial_data));

				//touch_ignore=rtc+20;
			//}
		//}

		//make_beep();

		touch_write_control(0x80);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	touch.pressed=1;
	printf("Interrupt touch");
}


static void delay_ms(__IO uint32_t nCount1) {
	__IO uint32_t nCount=nCount1*23666;
	while(nCount--) {
  }
}
