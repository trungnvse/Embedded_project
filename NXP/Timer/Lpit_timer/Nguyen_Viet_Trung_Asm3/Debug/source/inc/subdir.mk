################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/inc/asm3.c 

C_DEPS += \
./source/inc/asm3.d 

OBJS += \
./source/inc/asm3.o 


# Each subdirectory must supply rules for building sources it contributes
source/inc/%.o: ../source/inc/%.c source/inc/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm3\source" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm3\source\inc" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm3\device" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm3\CMSIS" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-inc

clean-source-2f-inc:
	-$(RM) ./source/inc/asm3.d ./source/inc/asm3.o

.PHONY: clean-source-2f-inc

