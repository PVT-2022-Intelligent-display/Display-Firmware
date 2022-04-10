/*
 * dma.h
 *
 *  Created on: Apr 10, 2022
 *      Author: Nouth
 *
 *      Work in progress, to be implemented later... I don't really know how to adapt the original to HAL.
 *
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "spiRoutines.h"
#include <stm32f4xx_hal.h>

extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;

#define dmaWait() while(__HAL_SPI_GET_FLAG(SPI_MASTER , SPI_FLAG_BSY));

#define dmaStartRx HAL_DMA_Init(&hdma_spi1_rx); \
	__HAL_DMA_ENABLE(&hdma_spi1_rx);

#define dmaStartRx HAL_DMA_Init(&hdma_spi1_tx); \
	__HAL_DMA_ENABLE(&hdma_spi1_tx);



#endif /* INC_DMA_H_ */
