/*
 * demoUart.c
 *
 *  Created on: Mar 28, 2022
 *      Author: Petr
 */

#include "demoUart.h"
#include "uart.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *inBuffer;
uint8_t *outBuffer;
uint8_t flagD; //read done
uint8_t flagR; //read requested
uint8_t flagT; //transmit done

#define CHARCOUNT 1
#define DEBUG_UART huart1

int assembleReply();

void initUartDemo(){
	flagD = 0;
	flagR = 0;
	flagT = 1;
	inBuffer = malloc(CHARCOUNT);
	outBuffer = malloc(100);
}

void uartDemoBlocking(){

	printf("Uart demo blocking entering.");
	uartReceiveBuffer(DEBUG_UART, inBuffer, CHARCOUNT, &flagD);
	while(flagD == 0){

	}
	int i = assembleReply();
	uartTransmitBuffer(DEBUG_UART, outBuffer, i, &flagT);
	while(flagT == 0){

	}
	printf("Uart demo blocking done.");
}

void uartDemoLoop(){
	if(flagD == 0){
		if(flagR == 0){
			uartReceiveBuffer(DEBUG_UART, inBuffer, CHARCOUNT, &flagD);
			flagR = 1;
		}
	}
	else if(flagT == 1){
		//it would be easier to just printf this, I'm using this just to demonstrate uart lib usage.
		int i = assembleReply();
		uartTransmitBuffer(DEBUG_UART, outBuffer, i, &flagT);
		flagR = 0;
		flagD = 0;
	}
}


int assembleReply(){
	char replyStart[29] = "I received following chars: {";
	char replyEnd[3] = "}\n\r";
	int i = 0;

	memcpy(outBuffer, replyStart, 29);
	i = i + 29;
	memcpy(outBuffer + i, inBuffer, 10);
	i = i + 10;
	memcpy(outBuffer + i, replyEnd, 3);
	i = i + 3;
	return i;
}

