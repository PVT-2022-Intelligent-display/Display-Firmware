/*
 * extFlash.c
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 *      Copied from original firmware
 */

#include "extFlash.h"
#include "spiRoutines.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
#include <stdio.h>

void flashCSSet(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 1);
}

void flashCSReset(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, 0);
}


unsigned int ext_flash_readid()
{
	unsigned int id_data;

	flashCSReset();
	SPI1_Transfer(0x9F);
	id_data=SPI1_Transfer(0x00);
	id_data<<=8;
	id_data|=SPI1_Transfer(0x00);
	id_data<<=8;
	id_data|=SPI1_Transfer(0x00);
	flashCSSet();
	return id_data;
}

void ext_flash_erase_4kB(unsigned int sector_adress)
{
	unsigned int i;

	ext_flash_wren();

	flashCSReset();
	SPI1_Transfer(0x20);
	SPI1_Transfer((sector_adress>>16)&0xFF);
	SPI1_Transfer((sector_adress>>8)&0xFF);
	SPI1_Transfer(sector_adress&0xFF);
	flashCSSet();

	for(i=0;i<1000;i++)
	{
		HAL_Delay(1);
		if(ext_flash_read_status_register_1()==0x00)
		{
			break;
		}
	}
}

void ext_flash_write(unsigned int sector_adress, char *buff, unsigned int len)
{
	unsigned int i;

	ext_flash_wren();

	flashCSReset();
	SPI1_Transfer(0x02);
	SPI1_Transfer((sector_adress>>16)&0xFF);
	SPI1_Transfer((sector_adress>>8)&0xFF);
	SPI1_Transfer(sector_adress&0xFF);

	for(i=0;i<len;i++)
	{
		SPI1_Transfer(buff[i]);
	}
	flashCSSet();

	for(i=0;i<1000;i++)
	{
		HAL_Delay(1);
		if(ext_flash_read_status_register_1()==0x00)
		{
			break;
		}
	}
}
void ext_flash_read(unsigned int sector_adress, char *buff, unsigned int len)
{
	ext_flash_continuous_read_begin(sector_adress);
	ext_flash_continuous_read_read(buff, len);
	ext_flash_continuous_read_finish();
}
unsigned char ext_flash_read_CR2V()
{
	char data;
	flashCSReset();
	SPI1_Transfer(0x15);
	data=SPI1_Transfer(0x00);
	flashCSSet();

	return data;
}

unsigned char ext_flash_read_status_register_1()
{
	char data;
	flashCSReset();
	SPI1_Transfer(0x05);
	data=SPI1_Transfer(0x00);
	flashCSSet();

	return data;
}

void ext_flash_wren()
{
	flashCSReset();
	SPI1_Transfer(0x06);
	flashCSSet();
}


//Continuous functions below keep conection open while reading, allowing for access to data not aligned to 4kB sectors
void ext_flash_continuous_read_begin(unsigned int sector_adress){
	/*unsigned char command[4];
	command[0]=0x03;
	command[1]=((char *)&sector_adress)[3];
	command[2]=((char *)&sector_adress)[2];
	command[3]=((char *)&sector_adress)[1];*/

	flashCSReset();

	SPI1_Transfer(0x03);
	SPI1_Transfer((sector_adress>>16)&0xFF);
	SPI1_Transfer((sector_adress>>8)&0xFF);
	SPI1_Transfer(sector_adress&0xFF);
}

void ext_flash_continuous_read_read(char *buff, unsigned int len){
	for(unsigned int i = 0; i<len; i++){
		*(buff+i) = SPI1_Transfer(0x00);
	}
}

void ext_flash_continuous_read_skip(unsigned int len){
	for(unsigned int i = 0; i<len; i++){
			SPI1_Transfer(0x00);
	}
}
void ext_flash_continuous_read_finish(){
	flashCSSet();
}








/*
 *
void store_picture_header_information_ext_flash()
{
	unsigned int i;

	ext_flash_erase_4kB(0);

	for(i=0;i<16;i++)
	{
		ext_flash_write(0+(256*i),((char *)&picture)+(256*i),256);
	}
}
void restore_picture_header_information_ext_flash()
{
	ext_flash_read(0,((char *)&picture),sizeof(picture[0])*MAX_PICTURES);
}
*/

