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

unsigned long get_tx_bytes(USART_TypeDef* USARTx)
{
	if(USARTx == RS485_UART)
	{
		return SIO_TBUFLEN_RS485;
	}
	else if(USARTx == DBG_UART)
	{
		return SIO_TBUFLEN_DBG;
	}
	return 0;
}
//**************************************************************************************
unsigned long get_rx_bytes(USART_TypeDef* USARTx)
{
	if(USARTx == DBG_UART)
	{
		return SIO_RBUFLEN_DBG;
	}

	else if(USARTx == RS485_UART)
	{
		return SIO_RBUFLEN_RS485;
	}

	return 0;
}
//**************************************************************************************
char usart_message_ready(USART_TypeDef* USARTx, char delimiter)
{
	if (USARTx == DBG_UART)
	{
		unsigned long tail = rd_pointer_rx_dbg;

		while ((wr_pointer_rx_dbg - tail) != 0)
		{
			if (rx_buffer_dbg[tail & (LEN_RX_BUFFER_DBG - 1)] == delimiter)
				return 1;
			++tail;
		}
	}
	else if (USARTx == RS485_UART)
	{
		unsigned long tail = rd_pointer_rx_rs485;

		while ((wr_pointer_rx_rs485 - tail) != 0)
		{
			if (rx_buffer_rs485[tail & (LEN_RX_BUFFER_RS485 - 1)] == delimiter)
				return 1;
			++tail;
		}
	}
	return 0;
}
//**************************************************************************************
/*
 * Reads message from specified uart rx buffer into @dst until @delimiter character is encountered or @max_len exceeded.
 * Returns: number of characters read if successful, zero when there is no message ready to be read.
 */
unsigned int read_usart_message(char* dst, UART_HandleTypeDef* huart, int max_len, char delimiter)
{
	USART_TypeDef* USARTx = huart->Instance;
	if (usart_message_ready(USARTx,delimiter))
	{
		int nr = 0;
		do
		{
			*dst = get_from_rx_buffer(USARTx);
			++nr;
		} while (*dst++ != delimiter && nr < max_len);

		return nr;
	}
	return 0;
}

/*
 * Sends @size bytes from @src via specified uart. Nonblocking.
 * Returns: 0 when operation fails due to another message being transmitted, number of bytes written to uart buffer otherwise.
 */

unsigned int send_usart_message(char* src, UART_HandleTypeDef* huart, int size){
	USART_TypeDef* USARTx = huart->Instance;
	if (USARTx == DBG_UART){
		if(wr_pointer_dbg!=rd_pointer_dbg){
			return 0;//another message is in progress.
		}

	}
	else if (USARTx == RS485_UART){
		if(wr_pointer_rs485!=rd_pointer_rs485){
			return 0;//another message is in progress.
		}
	}

	int nr = 0;
	do{
		put_in_tx_buffer(*(src + nr), USARTx);
		nr++;
	} while (nr < size);

	if(nr>0){
		USARTx->DR = get_from_tx_buffer(USARTx);
		__HAL_UART_ENABLE_IT(huart, UART_IT_TXE); //turn on tx interrupt forever (until turned off in IRQ handler)
	}
	return nr;
}







/*
//This function is called after each completed reception.
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	uint8_t *flagToRaise = NULL;
}

//This function is called after each completed transmission
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
		uint8_t *flagToRaise = NULL;
}
*/





