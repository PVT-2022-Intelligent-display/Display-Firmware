/*
 * uart.h
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#ifndef __UART_H__
#define __UART_H__

#include <stm32f4xx_hal.h>

#define MAX_STRLEN_RS485	 	255
#define MAX_STRLEN_DBG		 	4100

#define RS485UART USART2
#define DBGUART   USART1

extern UART_HandleTypeDef huart1; //usb connector / debug pins uart
extern UART_HandleTypeDef huart2;

//*************************************************************************************
#define SIO_TBUFLEN_RS485 	((unsigned long)(wr_pointer_rs485-rd_pointer_rs485))
#define SIO_TBUFLEN_DBG 	((unsigned long)(wr_pointer_dbg-rd_pointer_dbg))
//*************************************************************************************
#define SIO_RBUFLEN_RS485 	((unsigned long)(wr_pointer_rx_rs485-rd_pointer_rx_rs485))
#define SIO_RBUFLEN_DBG 	((unsigned long)(wr_pointer_rx_dbg-rd_pointer_rx_dbg))
//*************************************************************************************
extern char serial_data[128];
//*************************************************************************************
extern char received_string_rs485[MAX_STRLEN_RS485+1];
extern volatile char message_received_rs485;
extern volatile unsigned int message_len_rs485;
//*************************************************************************************
extern char received_string_dbg[MAX_STRLEN_DBG+1];
extern volatile char message_received_dbg;
extern volatile unsigned int message_len_dbg;
//*************************************************************************************
extern USART_TypeDef* RS485_UART;
extern USART_TypeDef* DBG_UART;
//*************************************************************************************
extern volatile unsigned long rd_pointer_rs485;
extern volatile unsigned long wr_pointer_rs485;
//**************************************************************************************
extern volatile unsigned long rd_pointer_dbg;
extern volatile unsigned long wr_pointer_dbg;
//**************************************************************************************
extern volatile unsigned long rd_pointer_rx_rs485;
extern volatile unsigned long wr_pointer_rx_rs485;
//**************************************************************************************
extern volatile unsigned long rd_pointer_rx_dbg;
extern volatile unsigned long wr_pointer_rx_dbg;


int __io_putchar(int ch);

void put_in_tx_buffer(char data, USART_TypeDef* USARTx);
char get_from_tx_buffer(USART_TypeDef* USARTx);
void put_in_rx_buffer(char data, USART_TypeDef* USARTx);
char get_from_rx_buffer(USART_TypeDef* USARTx);

char usart_message_ready(USART_TypeDef* USARTx, char delimiter);
unsigned int read_usart_message(char* dst, USART_TypeDef* USARTx, int max_len, char delimiter);


#endif /* __UART_H__ */
