#include "LCD_touch_driver.h"

touch_t touch;
//static I2C_HandleTypeDef hi2c1;
unsigned long touch_ignore=0;
//**************************************************************************
//	Touch initialization
//**************************************************************************
//void touch_ini(I2C_HandleTypeDef i2c_per)
// {
// 	hi2c1 = i2c_per;
// 	__GPIOB_CLK_ENABLE();
// 	touch_reset();
// }
// //**************************************************************************
// void register_touch_button(unsigned char index, unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1, unsigned char *p)
// {
// 	if(index>=MAX_TP_BUTTONS)
// 	{
// 		return;
// 	}

// 	sprintf(touch.key[index],"%s",p);

// 	touch.coordinates[index][0]=x0;
// 	touch.coordinates[index][1]=y0;
// 	touch.coordinates[index][2]=x1;
// 	touch.coordinates[index][3]=y1;
// }
// //**************************************************************************
// char validate_touch_coordinates()
// {
// 	if(touch.x<MIN_X) return 0;
// 	if(touch.x>MAX_X) return 0;
// 	if(touch.y<MIN_Y) return 0;
// 	if(touch.y>MAX_Y) return 0;

// 	touch.x=touch.x-MIN_X;
// 	touch.y=touch.y-MIN_Y;

// 	touch.x=(unsigned short) ((float) touch.x * X_SCALE);
// 	touch.y=(unsigned short) ((float) touch.y * Y_SCALE);

// 	return 1;
// }
// //**************************************************************************
// void get_touched_button()
// {
// 	unsigned char i;

// 	touch.key_pressed[0]=0x00;

// 	for(i=0;i<MAX_TP_BUTTONS;i++)
// 	{
// 		if( (touch.x >= touch.coordinates[i][0] &&  touch.x <= touch.coordinates[i][2]) && (touch.y >= touch.coordinates[i][1] &&  touch.y <= touch.coordinates[i][3]) )
// 		{
// 			memcpy(touch.key_pressed,touch.key[i],8);
// 			return;
// 		}
// 	}
// }
// //**************************************************************************
// void clear_touched_button()
// {
// 	touch.key_pressed[0]=0x00;
// }
// //**************************************************************************
// /* This function transmits one byte to the slave device
//  * Parameters:
//  *		I2Cx --> the I2C peripheral e.g. I2C1
//  *		data --> the data byte to be transmitted
//  */
// void I2C_write(I2C_TypeDef* I2Cx, uint8_t data){
// 	unsigned int del=0;

// 	I2C_SendData(I2Cx, data);
// 	// wait for I2C1 EV8_2 --> byte has been transmitted
// 	del=0;
// 	while(!I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
// 	{
// 		del++;
// 		DelayI2C(10);
// 		if(del==1000)
// 		{
// 			i2c_failure=1;
// 			return;
// 		}
// 	}
// }

// /* This function reads one byte from the slave device
//  * and acknowledges the byte (requests another byte)
//  */
// uint8_t I2C_read_ack(I2C_TypeDef* I2Cx){
// 	unsigned int del=0;
// 	// enable acknowledge of recieved data
// 	I2C_AcknowledgeConfig(I2Cx, ENABLE);
// 	// wait until one byte has been received
// 	del=0;
// 	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) )
// 	{
// 		del++;
// 		DelayI2C(10);
// 		if(del==1000)
// 		{
// 			i2c_failure=1;
// 			return 0;
// 		}
// 	}
// 	// read data from I2C data register and return data byte
// 	uint8_t data = I2C_ReceiveData(I2Cx);
// 	return data;
// }

// /* This function reads one byte from the slave device
//  * and doesn't acknowledge the recieved data
//  */
// uint8_t I2C_read_nack(I2C_TypeDef* I2Cx){
// 	unsigned int del=0;
// 	// disabe acknowledge of received data
// 	I2C_AcknowledgeConfig(I2Cx, DISABLE);
// 	// wait until one byte has been received
// 	del=0;
// 	while( !I2C_CheckEvent(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED) )
// 	{
// 		del++;
// 		DelayI2C(10);
// 		if(del==1000)
// 		{
// 			i2c_failure=1;
// 			return 0;
// 		}
// 	}
// 	// read data from I2C data register and return data byte
// 	uint8_t data = I2C_ReceiveData(I2Cx);
// 	return data;
// }

// /* This funtion issues a stop condition and therefore
//  * releases the bus
//  */
// void I2C_stop(I2C_TypeDef* I2Cx){
// 	// Send I2C1 STOP Condition
// 	I2C_GenerateSTOP(I2Cx, ENABLE);
// }


// void touch_write_control(unsigned char ctrl)
// {
// 	HAL_I2C_Master_Recive(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
// 	I2C_write(I2C1, ctrl); // write one byte to the slave
// 	I2C_stop(I2C1); // stop the transmission

// }
// void touch_write_register(uint8_t adr, uint16_t val)
// {
// 	HAL_StatusTypeDef status;
// 	uint8_t buf[3];
// 	buf[0] = adr;
// 	buf[1] = (uint8_t)(val>>8);
// 	buf[2] = (uint8_t)(val);

// 	status = HAL_I2C_Master_Transmit(hi2c1, SLAVE_ADDRESS_TOUCH, val, 2, HAL_MAX_DELAY); // start a transmission in Master transmitter mode

// 	I2C_write(I2C1, adr<<3); // write one byte to the slave

// 	I2C_write(I2C1, val>>8); // write one byte to the slave
// 	I2C_write(I2C1, val&0xFF); // write one byte to the slave
// 	I2C_stop(I2C1); // stop the transmission
// }

// unsigned short touch_read(unsigned char adr)
// {
// 	unsigned short data;
// 	unsigned char adr_i2c=0x01;

// 	adr_i2c+=adr<<	3;

// 	i2c_failure=0;

// 	I2C_start(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Transmitter); // start a transmission in Master transmitter mode
// 	I2C_write(I2C1, adr_i2c); // write one byte to the slave
// 	I2C_Restart(I2C1, SLAVE_ADDRESS_TOUCH, I2C_Direction_Receiver);

// 	data=I2C_read_ack(I2C1);
// 	data<<=8;
// 	data|=I2C_read_nack(I2C1);
// 	I2C_stop(I2C1); // stop the transmission

// 	return data;
// }

// void touch_reset()
// {
// 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_RESET);
// 	delay_ms(10);
// 	HAL_GPIO_WritePin(TOUCH_RESET_PORT,TOUCH_RESET,GPIO_PIN_SET);
// }

