################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/myo/driver/myo_adc.c \
../src/myo/driver/myo_dp.c \
../src/myo/driver/myo_fla.c \
../src/myo/driver/myo_mux.c \
../src/myo/driver/myo_rf.c 

OBJS += \
./src/myo/driver/myo_adc.o \
./src/myo/driver/myo_dp.o \
./src/myo/driver/myo_fla.o \
./src/myo/driver/myo_mux.o \
./src/myo/driver/myo_rf.o 

C_DEPS += \
./src/myo/driver/myo_adc.d \
./src/myo/driver/myo_dp.d \
./src/myo/driver/myo_fla.d \
./src/myo/driver/myo_mux.d \
./src/myo/driver/myo_rf.d 


# Each subdirectory must supply rules for building sources it contributes
src/myo/driver/%.o: ../src/myo/driver/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARM_MATH_CM4 -DARM_MATH_MATRIX_CHECK -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I/tools/modulesystem/tools/subversion/subversion-1.7.0-dev/install/sled11.x86_64.gcc.release/include -I../include/myo -I../system/include/stm32f4-hal -I../include/myo/driver -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


