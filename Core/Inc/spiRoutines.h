/*
 * spiRoutines.h
 *
 *  Created on: Apr 3, 2022
 *      Author: Petr
 */

#ifndef INC_SPIROUTINES_H_
#define INC_SPIROUTINES_H_

#include <stm32f4xx_hal.h>


extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

#define SPI_MASTER hspi1

uint8_t SPI1_Transfer(uint8_t data);
uint8_t SPI2_Transfer(uint8_t data);
uint8_t SPI3_Transfer(uint8_t data);

#endif /* INC_SPIROUTINES_H_ */
