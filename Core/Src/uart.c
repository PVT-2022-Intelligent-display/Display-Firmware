/*
 * uart.c
 *
 *  Created on: Mar 21, 2022
 *      Author: Petr
 */

#include "uart.h"

uint8_t *pHuart1RxDoneFlag;
uint8_t *pHuart2RxDoneFlag;
uint8_t *pHuart4RxDoneFlag;

uint8_t *pHuart1TxDoneFlag;
uint8_t *pHuart2TxDoneFlag;
uint8_t *pHuart4TxDoneFlag;

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
	if(huart->Instance == USART1){
				flagToRaise = pHuart1RxDoneFlag;
			}
			else if(huart->Instance == USART2){
				flagToRaise = pHuart2RxDoneFlag;
			}
			else if(huart->Instance == UART4){
				flagToRaise = pHuart4RxDoneFlag;
			}
	if(flagToRaise!=NULL){
		*flagToRaise = 1;
	}
}

//This function is called after each completed transmission
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
		uint8_t *flagToRaise = NULL;
		if(huart->Instance == USART1){
			flagToRaise = pHuart1TxDoneFlag;
		}
		else if(huart->Instance == USART2){
			flagToRaise = pHuart2TxDoneFlag;
		}
		else if(huart->Instance == UART4){
			flagToRaise = pHuart4TxDoneFlag;
		}

		if(flagToRaise!=NULL){
			*flagToRaise = 1;
		}
}



/**Nonblocking. Sends @size bytes of data from @buffer over specified uart.
 * doneFlag is set to 0 when called and changes to 1 once transmission is complete.
 * Returns: 0 when ok, nonzero otherwise.
 */
//WIP! Does not yet work!
int uartTransmitBuffer(UART_HandleTypeDef huart, uint8_t *buffer, uint16_t size, uint8_t *doneFlag){
	if(huart.Instance == USART1){
		pHuart1TxDoneFlag = doneFlag;
	}
	else if(huart.Instance == USART2){
		pHuart2TxDoneFlag = doneFlag;
	}
	else if(huart.Instance == UART4){
		pHuart4TxDoneFlag = doneFlag;
	}
	else{ //unhandled uart
		return 100;
	}

	*doneFlag = 0;
	return HAL_UART_Transmit_IT(&huart, buffer, size);
}

/**Nonblocking. Reads @size bytes of data from @buffer over specified uart.
 * doneFlag is set to 0 when called and changes to 1 once reception is complete.
 * Returns: 0 when ok, nonzero otherwise.
 */
//WIP! Does not yet work!
int uartReceiveBuffer(UART_HandleTypeDef huart, uint8_t *buffer, uint16_t size, uint8_t *doneFlag){
	if(huart.Instance == USART1){
			pHuart1RxDoneFlag = doneFlag;
		}
		else if(huart.Instance == USART2){
			pHuart2RxDoneFlag = doneFlag;
		}
		else if(huart.Instance == UART4){
			pHuart4RxDoneFlag = doneFlag;
		}
		else{ //unhandled uart
			return 100;
		}
	*doneFlag = 0;
	return HAL_UART_Receive_IT(&huart, buffer, size);
}

