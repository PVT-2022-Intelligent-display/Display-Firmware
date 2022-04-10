/*
 * extFlashDemo.c
 *
 *  Created on: Apr 10, 2022
 *      Author: Nouth
 */

#include "uart.h"
#include <stdio.h>
#include <stm32f4xx_hal.h>
#include <string.h>
#include "extFlash.h"

#define demoFlashAddr 0x00

void flashDemoPrintLast(){
	char flashMsg[64];
	ext_flash_read(demoFlashAddr, flashMsg, 64);
	flashMsg[63] = 0; //just to be safe
	printf("[FLASH demo] Read message from flash: \"%s\".\r\n", flashMsg);
}

int flashDemoLoop(){

		char delimiter = 13;
		char msg[64];

		int count = read_usart_message(msg, &huart1, sizeof(msg), delimiter);

		if(count == 0){
			//While reading buffer of received chars, delimiter char was not encountered.
			printf("[FLASH demo] message not yet ready.\r\n");
			return 0;
		}

		msg[count-1] = 0; //null terminate received string

		printf("[FLASH demo] Saving message to flash: \"%s\".\r\n", msg);

		ext_flash_erase_4kB(demoFlashAddr);
		ext_flash_write(demoFlashAddr, msg, count);

		printf("[FLASH demo] Done.\n\r");
		return 1;

}






