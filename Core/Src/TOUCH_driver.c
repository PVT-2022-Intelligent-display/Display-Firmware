
#include "TOUCH_driver.h"
#include "uart.h"

#include <stm32f4xx_hal_gpio.h>
#include "LCD_driver.h"
I2C_HandleTypeDef hi2c1_touch;


void Init_TOUCH(I2C_HandleTypeDef def)
{
	hi2c1_touch = def;
}
void touch_write_control(unsigned char ctrl)
{
	HAL_StatusTypeDef status =0;
	status = HAL_I2C_Master_Transmit (&hi2c1_touch, SLAVE_ADDRESS_TOUCH, &ctrl, sizeof(ctrl),I2C_DELAY);



	//I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	//I2C_write(I2C1, ctrl); // write one byte to the slave
	//I2C_stop(I2C1); // stop the transmission

}
void touch_write_register(uint8_t adr, uint16_t val)
{

	HAL_StatusTypeDef status =0;
	status = HAL_I2C_Master_Transmit(&hi2c1_touch, SLAVE_ADDRESS_TOUCH, &val, sizeof(val) , I2C_DELAY);


	//I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	//I2C_write(I2C1, adr<<3); // write one byte to the slave
	//I2C_write(I2C1, val>>8); // write one byte to the slave
	//I2C_write(I2C1, val&0xFF); // write one byte to the slave
	//I2C_stop(I2C1); // stop the transmission
}

unsigned short touch_read(unsigned char adr)
{
	uint16_t data;// was unsigned byte
	unsigned char adr_i2c=0x01;
	HAL_StatusTypeDef status =0;

	// ??? WHAT IS ADDR HERE?ADDDR OF A SLAVE ?
	status = HAL_I2C_Master_Receive(&hi2c1_touch, SLAVE_ADDRESS_TOUCH | 0x01, &data, sizeof(data), I2C_DELAY);

	// ??? Why <<3 and not just 1?
	// ??? Probabbly need to add as interupt?


	//adr_i2c+=adr<<3;
	//i2c_failure=0;
	//I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
	//I2C_write(I2C1, adr_i2c); // write one byte to the slave
	//I2C_Restart(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Receiver);
	//data=I2C_read_ack(I2C1);
	//data<<=8;
	//data|=I2C_read_nack(I2C1);
	//I2C_stop(I2C1); // stop the transmission

	return data;
}

void touch_reset()
{
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_RESET);
 	delay_ms(10);
 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_SET);
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

		touch.y=touch.x1;
		touch.x=1024-touch.y1;

		if(validate_touch_coordinates()==1)
		{
			//if(touch_ignore<rtc)
			//{
				printf("TOUCH=%d,%d\n",touch.x,touch.y);
				//USART_puts(DBG_UART,serial_data,strlen(serial_data));

				//touch_ignore=rtc+20;
			//}
		}

		//make_beep();

		touch_write_control(0x80);
	}
}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	touch.pressed=1;
}
