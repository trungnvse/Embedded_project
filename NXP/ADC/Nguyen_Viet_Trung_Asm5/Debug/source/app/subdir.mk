################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/app/Nguyen_Viet_Trung_Asm5.c 

C_DEPS += \
./source/app/Nguyen_Viet_Trung_Asm5.d 

OBJS += \
./source/app/Nguyen_Viet_Trung_Asm5.o 


# Each subdirectory must supply rules for building sources it contributes
source/app/%.o: ../source/app/%.c source/app/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKE16Z64VLF4 -DCPU_MKE16Z64VLF4_cm0plus -DSDK_OS_BAREMETAL -DSERIAL_PORT_TYPE_UART=1 -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\board" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\source\inc" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\source" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\drivers" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\component\lists" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\component\uart" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\component\serial_manager" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\utilities" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\device" -I"E:\FPT ACADEMY\MCU\CODE\Nguyen_Viet_Trung_Asm5\CMSIS" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source-2f-app

clean-source-2f-app:
	-$(RM) ./source/app/Nguyen_Viet_Trung_Asm5.d ./source/app/Nguyen_Viet_Trung_Asm5.o

.PHONY: clean-source-2f-app

