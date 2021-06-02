################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../software/UART_handler/UART_handler.c 

C_DEPS += \
./software/UART_handler/UART_handler.d 

OBJS += \
./software/UART_handler/UART_handler.o 


# Each subdirectory must supply rules for building sources it contributes
software/UART_handler/UART_handler.o: ../software/UART_handler/UART_handler.c software/UART_handler/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../software/UART_handler -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"software/UART_handler/UART_handler.d" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

