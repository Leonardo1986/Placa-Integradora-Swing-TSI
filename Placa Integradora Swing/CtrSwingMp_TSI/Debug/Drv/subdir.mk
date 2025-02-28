################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drv/api_swing.c \
../Drv/encoder.c \
../Drv/inputs.c \
../Drv/outputs.c \
../Drv/position.c \
../Drv/protocolo_swing.c \
../Drv/sensor.c \
../Drv/stepmotor.c \
../Drv/task_manager.c 

OBJS += \
./Drv/api_swing.o \
./Drv/encoder.o \
./Drv/inputs.o \
./Drv/outputs.o \
./Drv/position.o \
./Drv/protocolo_swing.o \
./Drv/sensor.o \
./Drv/stepmotor.o \
./Drv/task_manager.o 

C_DEPS += \
./Drv/api_swing.d \
./Drv/encoder.d \
./Drv/inputs.d \
./Drv/outputs.d \
./Drv/position.d \
./Drv/protocolo_swing.d \
./Drv/sensor.d \
./Drv/stepmotor.d \
./Drv/task_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Drv/%.o Drv/%.su Drv/%.cyclo: ../Drv/%.c Drv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F030x8 -DDEBUG -c -I../Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../Drv -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drv

clean-Drv:
	-$(RM) ./Drv/api_swing.cyclo ./Drv/api_swing.d ./Drv/api_swing.o ./Drv/api_swing.su ./Drv/encoder.cyclo ./Drv/encoder.d ./Drv/encoder.o ./Drv/encoder.su ./Drv/inputs.cyclo ./Drv/inputs.d ./Drv/inputs.o ./Drv/inputs.su ./Drv/outputs.cyclo ./Drv/outputs.d ./Drv/outputs.o ./Drv/outputs.su ./Drv/position.cyclo ./Drv/position.d ./Drv/position.o ./Drv/position.su ./Drv/protocolo_swing.cyclo ./Drv/protocolo_swing.d ./Drv/protocolo_swing.o ./Drv/protocolo_swing.su ./Drv/sensor.cyclo ./Drv/sensor.d ./Drv/sensor.o ./Drv/sensor.su ./Drv/stepmotor.cyclo ./Drv/stepmotor.d ./Drv/stepmotor.o ./Drv/stepmotor.su ./Drv/task_manager.cyclo ./Drv/task_manager.d ./Drv/task_manager.o ./Drv/task_manager.su

.PHONY: clean-Drv

