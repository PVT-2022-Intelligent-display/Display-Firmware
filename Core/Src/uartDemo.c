/*
 * uartDemo.c
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 *
 *      Simple uart usage example that echoes the message sent by user via debug uart.
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

	int count = read_usart_message(msg, &huart1, sizeof(msg), delimiter);

	if(count == 0){
		//While reading buffer of received chars, delimiter char was not encountered.
		printf("[UART demo] message not yet ready.\r\n");
		return;
	}

	printf("[UART demo] message ready!\r\n");

	msg[count-1] = 0; //null terminate received string

	char reply[256];

	sprintf(reply, " Sending this as a buffer instead of printf just for demonstration. \n\r The message I got was \"%s\". \n\r", msg);

	int ret = send_usart_message(reply, &huart1, strlen(reply));

	if(ret == 0){
		printf("[UART demo] Fail: Cannot transmit now, another transmission in progress.\n\r");
	}

	HAL_Delay(500); //sleep for a bit so that next printf does not interfere with send_uart_message();

	printf("[UART demo] Success.\n\r");

}



