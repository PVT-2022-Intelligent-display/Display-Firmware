/*
 * uartDemo.c
 *
 *  Created on: Apr 3, 2022
 *      Author: Nouth
 *
 *      Simple uart usage example.
 *
 */

#include "uart.h"
#include <stdio.h>
#include <stm32f4xx_hal.h>
#include <string.h>



void uartDemoLoop(){

	static int firstFlag = 0;

	if(firstFlag == 0){
		firstFlag = 1;
		printf("[UART demo] Send a string over debug uart and terminate it by ENTER (ascii 13).\r\n");
	}

	char delimiter = 13;
	char msg[64];

	int count = read_usart_message(msg, DBG_UART, sizeof(msg), delimiter);

	if(count == 0){
		printf("[UART demo] message not yet ready.\r\n");
		return;
	}

	printf("[UART demo] message ready!\r\n");

	msg[count-1] = 0; //null terminate received string

	char reply[256];

	sprintf(reply, "I'm sending this as a buffer instead of printf just for demonstration. The message I got was \" %s \". ", msg);







}



