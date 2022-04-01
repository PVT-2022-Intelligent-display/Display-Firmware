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

//redirect printf to uart1.
PUTCHAR_PROTOTYPE{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 0xffff);
	return ch;
}

//This function is called after each completed reception.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t *flagToRaise = NULL;
}

//This function is called after each completed transmission
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
		uint8_t *flagToRaise = NULL;
}



