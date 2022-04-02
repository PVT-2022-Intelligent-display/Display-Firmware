/*
 * uart.c
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#include "uart.h"

#define LEN_TX_BUFFER_RS485 1024
volatile char tx_buffer_rs485[LEN_TX_BUFFER_RS485];
volatile unsigned long rd_pointer_rs485=0;
volatile unsigned long wr_pointer_rs485=0;
//**************************************************************************************
#define LEN_TX_BUFFER_DBG 1024
volatile char tx_buffer_dbg[LEN_TX_BUFFER_DBG];
volatile unsigned long rd_pointer_dbg=0;
volatile unsigned long wr_pointer_dbg=0;
//**************************************************************************************
#define LEN_RX_BUFFER_RS485 1024
volatile char rx_buffer_rs485[LEN_RX_BUFFER_RS485];
volatile unsigned long rd_pointer_rx_rs485=0;
volatile unsigned long wr_pointer_rx_rs485=0;
//**************************************************************************************
#define LEN_RX_BUFFER_DBG 8192
volatile char rx_buffer_dbg[LEN_RX_BUFFER_DBG];
volatile unsigned long rd_pointer_rx_dbg=0;
volatile unsigned long wr_pointer_rx_dbg=0;
//**************************************************************************************
char serial_data[128];
//**************************************************************************************
char received_string_rs485[MAX_STRLEN_RS485+1];
volatile char message_received_rs485=0;
volatile int cnt_rs485 = 0;
volatile unsigned int message_len_rs485=0;
//**************************************************************************************
char received_string_dbg[MAX_STRLEN_DBG+1];
volatile char message_received_dbg=0;
volatile int cnt_dbg = 0;
volatile unsigned int message_len_dbg=0;
//**************************************************************************************
USART_TypeDef* RS485_UART=RS485UART; //PB: I have no idea what the point of this juggling is, but am keeping it just in case.
USART_TypeDef* DBG_UART=DBGUART;




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

void put_in_rx_buffer(char data, USART_TypeDef* USARTx)
{
	if(USARTx == DBG_UART)
	{
		if(SIO_RBUFLEN_DBG>=LEN_RX_BUFFER_DBG)
			{
				return;
			}
		rx_buffer_dbg[wr_pointer_rx_dbg & (LEN_RX_BUFFER_DBG - 1)]=data;
		wr_pointer_rx_dbg++;
	}
	else if(USARTx == RS485_UART)
	{
		if(SIO_RBUFLEN_RS485>=LEN_RX_BUFFER_RS485)
			{
				return;
			}
		rx_buffer_rs485[wr_pointer_rx_rs485 & (LEN_RX_BUFFER_RS485 - 1)]=data;
		wr_pointer_rx_rs485++;
	}
}

void put_in_tx_buffer(char data, USART_TypeDef* USARTx)
{
	if(USARTx == RS485_UART)
	{
		if(SIO_TBUFLEN_RS485>=LEN_TX_BUFFER_RS485)
			{
				return;
			}
		tx_buffer_rs485[wr_pointer_rs485 & (LEN_TX_BUFFER_RS485 - 1)]=data;
		wr_pointer_rs485++;
	}
	else if(USARTx == DBG_UART)
	{
		while(SIO_TBUFLEN_DBG>=LEN_TX_BUFFER_DBG)
			{
				//return;
			}
		tx_buffer_dbg[wr_pointer_dbg & (LEN_TX_BUFFER_DBG - 1)]=data;
		wr_pointer_dbg++;
	}
}
char get_from_tx_buffer(USART_TypeDef* USARTx)
{
	char data;

	if(USARTx == RS485_UART)
	{

		data=tx_buffer_rs485[rd_pointer_rs485 & (LEN_TX_BUFFER_RS485-1)];
		rd_pointer_rs485++;
	}
	else if(USARTx == DBG_UART)
	{

		data=tx_buffer_dbg[rd_pointer_dbg & (LEN_TX_BUFFER_DBG-1)];
		rd_pointer_dbg++;
	}
	return data;
}
char get_from_rx_buffer(USART_TypeDef* USARTx)
{
	char data;

	if(USARTx == RS485_UART)
	{

		data=rx_buffer_rs485[rd_pointer_rx_rs485 & (LEN_RX_BUFFER_RS485-1)];
		rd_pointer_rx_rs485++;
	}
	else if(USARTx == DBG_UART)
	{

		data=rx_buffer_dbg[rd_pointer_rx_dbg & (LEN_RX_BUFFER_DBG-1)];
		rd_pointer_rx_dbg++;
	}
	return data;
}







//This function is called after each completed reception.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t *flagToRaise = NULL;
}

//This function is called after each completed transmission
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
		uint8_t *flagToRaise = NULL;
}





