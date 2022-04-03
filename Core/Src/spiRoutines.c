/*
 * spiRoutines.c
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 *
 */

#include "spiRoutines.h"
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
#include <stdio.h>


uint8_t SPI1_Transfer(uint8_t data)
{
	SPI_TypeDef *SPI1inst = hspi1.Instance;
    SPI1inst->DR = data;
    while (!(SPI1inst->SR & (SPI_FLAG_TXE)));
    while (!(SPI1inst->SR & (SPI_FLAG_RXNE)));
    while (SPI1inst->SR & (SPI_FLAG_BSY));
    return SPI1inst->DR;
}
//**************************************************************************************
uint8_t SPI2_Transfer(uint8_t data)
{
	SPI_TypeDef *SPI2inst = hspi2.Instance;
    SPI2inst->DR = data;
    while (!(SPI2inst->SR & (SPI_FLAG_TXE)));
    while (!(SPI2inst->SR & (SPI_FLAG_RXNE)));
    while (SPI2inst->SR & (SPI_FLAG_BSY));
    return SPI2inst->DR;
}
//**************************************************************************************
uint8_t SPI3_Transfer(uint8_t data)
{
	SPI_TypeDef *SPI3inst = hspi3.Instance;
    SPI3inst->DR = data;
    while (!(SPI3inst->SR & (SPI_FLAG_TXE)));
    while (!(SPI3inst->SR & (SPI_FLAG_RXNE)));
    while (SPI3inst->SR & (SPI_FLAG_BSY));
    return SPI3inst->DR;
}
