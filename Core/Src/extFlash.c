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
#include <string.h>

#define FLASHWRITE_DEBUG_PRINT 0


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

void ext_flash_erase_full(){
	unsigned int i;

	ext_flash_wren();

	flashCSReset();
	SPI1_Transfer(0x60);
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

void ext_flash_erase_4kB(unsigned int address)
{
	unsigned int i;

	printf("erase at %d \n\r", address);

	ext_flash_wren();

	flashCSReset();
	SPI1_Transfer(0x20);
	SPI1_Transfer((address>>16)&0xFF);
	SPI1_Transfer((address>>8)&0xFF);
	SPI1_Transfer(address&0xFF);
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




void ext_flash_write(unsigned int address, unsigned char *buff, unsigned int len)
{
	ext_flash_continuous_write_begin(address);
	ext_flash_continuous_write_write(buff, len);
	ext_flash_continuous_write_finish();
}
void ext_flash_read(unsigned int address, unsigned char *buff, unsigned int len)
{
	ext_flash_continuous_read_begin(address);
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
void ext_flash_continuous_read_begin(unsigned int address){
	/*unsigned char command[4];
	command[0]=0x03;
	command[1]=((char *)&address)[3];
	command[2]=((char *)&address)[2];
	command[3]=((char *)&address)[1];*/

	flashCSReset();

	SPI1_Transfer(0x03);
	SPI1_Transfer((address>>16)&0xFF);
	SPI1_Transfer((address>>8)&0xFF);
	SPI1_Transfer(address&0xFF);
}

void ext_flash_continuous_read_read(unsigned char *buff, unsigned int len){
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

void ext_flash_continuous_write_begin(unsigned int address){
	ext_flash_wren();
	flashCSReset();
	SPI1_Transfer(0x02);
	SPI1_Transfer((address>>16)&0xFF);
	SPI1_Transfer((address>>8)&0xFF);
	SPI1_Transfer(address&0xFF);
	if(FLASHWRITE_DEBUG_PRINT){printf("FLASHWRITE @ %x :", address);};
}
void ext_flash_continuous_write_write(unsigned char *buff, unsigned int len){
	unsigned int i;

	for(i=0;i<len;i++)
		{
			SPI1_Transfer(buff[i]);
			if(FLASHWRITE_DEBUG_PRINT){printf(" %x ", buff[i]);};
		}
	if(FLASHWRITE_DEBUG_PRINT){printf("\n\r");};
}
void ext_flash_continuous_write_finish(){
	flashCSSet();
	unsigned int i;
	for(i=0;i<1000;i++)
	{
		HAL_Delay(1);
		if(ext_flash_read_status_register_1()==0x00)
		{
			break;
		}
	}
}

void ext_flash_print_sector(unsigned int address){
	flashCSReset();

	SPI1_Transfer(0x03);
	SPI1_Transfer((address>>16)&0xFF);
	SPI1_Transfer((address>>8)&0xFF);
	SPI1_Transfer(address&0xFF);

	printf("Printing sector @ %d: ", address);
	int i;
	for(i=0;i<SECTOR_SIZE;i++){
		printf("%02x ", SPI1_Transfer(0x00));
	}
	printf("\n\r");

}



void ext_flash_write_multipage(unsigned int address, unsigned char *buff, unsigned int len){
	printf("Multipage write [%d]b@%d \n\r", len, address);
	unsigned char pageBuff[PAGE_SIZE];
	unsigned int bytesLeft = len;
	unsigned int buffIndex = 0;
	unsigned int addrIncrement = 0;

	while(bytesLeft > 0){
		if(bytesLeft <= PAGE_SIZE){
			memcpy(pageBuff, buff + buffIndex, bytesLeft);
			bytesLeft = 0;
		}
		else{
			memcpy(pageBuff, buff + buffIndex, PAGE_SIZE);
			bytesLeft -= PAGE_SIZE;
		}
		ext_flash_write(address + addrIncrement, pageBuff, PAGE_SIZE);
		addrIncrement += PAGE_SIZE;
	}

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

