################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_inverse_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q31.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_f32.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q15.c \
../system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q31.c 

OBJS += \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_inverse_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q31.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_f32.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q15.o \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q31.o 

C_DEPS += \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_add_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_init_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_inverse_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_fast_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_mult_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_scale_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_sub_q31.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_f32.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q15.d \
./system/src/dsp_src_ref/MatrixFunctions/arm_mat_trans_q31.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/dsp_src_ref/MatrixFunctions/%.o: ../system/src/dsp_src_ref/MatrixFunctions/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=softfp -mfpu=fpv4-sp-d16 -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wpadded -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g3 -DDEBUG -DARM_MATH_CM4 -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F401xE -DUSE_HAL_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f4-hal" -I/tools/modulesystem/tools/subversion/subversion-1.7.0-dev/install/sled11.x86_64.gcc.release/include -I../include/myo -std=gnu11 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


