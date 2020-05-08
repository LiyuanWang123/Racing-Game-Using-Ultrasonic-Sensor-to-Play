################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
LD_SRCS += \
../src/lscript.ld 

C_SRCS += \
../src/bsp.c \
../src/fonts.c \
../src/lab2a.c \
../src/lcd.c \
../src/main.c \
../src/platform.c \
../src/qepn.c \
../src/qfn.c \
../src/qfn_init.c \
../src/timeCount.c \
../src/xgpio_intr_tapp_example.c \
../src/xgpio_tapp_example.c \
../src/xintc_tapp_example.c 

OBJS += \
./src/bsp.o \
./src/fonts.o \
./src/lab2a.o \
./src/lcd.o \
./src/main.o \
./src/platform.o \
./src/qepn.o \
./src/qfn.o \
./src/qfn_init.o \
./src/timeCount.o \
./src/xgpio_intr_tapp_example.o \
./src/xgpio_tapp_example.o \
./src/xintc_tapp_example.o 

C_DEPS += \
./src/bsp.d \
./src/fonts.d \
./src/lab2a.d \
./src/lcd.d \
./src/main.d \
./src/platform.d \
./src/qepn.d \
./src/qfn.d \
./src/qfn_init.d \
./src/timeCount.d \
./src/xgpio_intr_tapp_example.d \
./src/xgpio_tapp_example.d \
./src/xintc_tapp_example.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MicroBlaze gcc compiler'
	mb-gcc -Wall -O0 -g3 -c -fmessage-length=0 -MT"$@" -I../../final_project_bsp/microblaze_0/include -mlittle-endian -mcpu=v10.0 -mxl-soft-mul -Wl,--no-relax -ffunction-sections -fdata-sections -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


