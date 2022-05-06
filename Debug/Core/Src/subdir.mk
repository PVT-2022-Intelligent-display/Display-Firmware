################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/LCD_driver.c \
../Core/Src/TOUCH_driver.c \
../Core/Src/configLib.c \
../Core/Src/extFlash.c \
../Core/Src/extFlashDemo.c \
../Core/Src/main.c \
../Core/Src/objectVisualization.c \
../Core/Src/spiRoutines.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uart.c \
../Core/Src/uartDemo.c 

OBJS += \
./Core/Src/LCD_driver.o \
./Core/Src/TOUCH_driver.o \
./Core/Src/configLib.o \
./Core/Src/extFlash.o \
./Core/Src/extFlashDemo.o \
./Core/Src/main.o \
./Core/Src/objectVisualization.o \
./Core/Src/spiRoutines.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uart.o \
./Core/Src/uartDemo.o 

C_DEPS += \
./Core/Src/LCD_driver.d \
./Core/Src/TOUCH_driver.d \
./Core/Src/configLib.d \
./Core/Src/extFlash.d \
./Core/Src/extFlashDemo.d \
./Core/Src/main.d \
./Core/Src/objectVisualization.d \
./Core/Src/spiRoutines.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uart.d \
./Core/Src/uartDemo.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/LCD_driver.d ./Core/Src/LCD_driver.o ./Core/Src/LCD_driver.su ./Core/Src/TOUCH_driver.d ./Core/Src/TOUCH_driver.o ./Core/Src/TOUCH_driver.su ./Core/Src/configLib.d ./Core/Src/configLib.o ./Core/Src/configLib.su ./Core/Src/extFlash.d ./Core/Src/extFlash.o ./Core/Src/extFlash.su ./Core/Src/extFlashDemo.d ./Core/Src/extFlashDemo.o ./Core/Src/extFlashDemo.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/objectVisualization.d ./Core/Src/objectVisualization.o ./Core/Src/objectVisualization.su ./Core/Src/spiRoutines.d ./Core/Src/spiRoutines.o ./Core/Src/spiRoutines.su ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su ./Core/Src/uartDemo.d ./Core/Src/uartDemo.o ./Core/Src/uartDemo.su

.PHONY: clean-Core-2f-Src

