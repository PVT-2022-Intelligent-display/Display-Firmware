/*
 * uart.h
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#ifndef __UART_H__
#define __UART_H__

#include <stm32f4xx_hal.h>

#define UART1_RX_BUFF_SIZE 1 //how many bytes will be received before firing HAL_UART_TxCpltCallback

extern UART_HandleTypeDef huart1; //debug TX RX pins
extern uint8_t huart1RxBuffer[UART1_RX_BUFF_SIZE];

int __io_putchar(int ch);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);


#endif /* __UART_H__ */
