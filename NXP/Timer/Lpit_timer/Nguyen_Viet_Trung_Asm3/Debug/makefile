################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include source/inc/subdir.mk
-include source/app/subdir.mk
-include source/subdir.mk
-include device/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Nguyen_Viet_Trung_Asm3
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: Nguyen_Viet_Trung_Asm3.axf

# Tool invocations
Nguyen_Viet_Trung_Asm3.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -Xlinker -Map="Nguyen_Viet_Trung_Asm3.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -Xlinker --sort-section=alignment -Xlinker --cref -mcpu=cortex-m0plus -mthumb -T Nguyen_Viet_Trung_Asm3_Debug.ld -o "Nguyen_Viet_Trung_Asm3.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Nguyen_Viet_Trung_Asm3.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Nguyen_Viet_Trung_Asm3.axf"; # arm-none-eabi-objcopy -v -O binary "Nguyen_Viet_Trung_Asm3.axf" "Nguyen_Viet_Trung_Asm3.bin" ; # checksum -p MKE16Z64xxx4 -d "Nguyen_Viet_Trung_Asm3.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
