/*
 * uart.h
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#ifndef __UART_H__
#define __UART_H__

#include <stm32f4xx_hal.h>

extern UART_HandleTypeDef huart1; //usb connector / debug pins uart
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart4;

int __io_putchar(int ch);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

int uartTransmitBuffer(UART_HandleTypeDef huart, uint8_t *buffer, uint16_t size, uint8_t *doneFlag);
int uartReceiveBuffer(UART_HandleTypeDef huart, uint8_t *buffer, uint16_t size, uint8_t *doneFlag);


#endif /* __UART_H__ */
