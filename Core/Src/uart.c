/*
 * uart.c
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#include "uart.h"


#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


PUTCHAR_PROTOTYPE{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xffff);
	return ch;
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	//after completed reception, receive again
	HAL_UART_Receive_IT(huart, (uint8_t *)huart->pRxBuffPtr, huart->RxXferSize);
}


