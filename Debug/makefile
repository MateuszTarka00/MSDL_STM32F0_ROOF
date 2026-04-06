################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM0/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/CANopenNode_STM32/subdir.mk
-include Core/CANopenNode/storage/subdir.mk
-include Core/CANopenNode/extra/subdir.mk
-include Core/CANopenNode/309/subdir.mk
-include Core/CANopenNode/305/subdir.mk
-include Core/CANopenNode/304/subdir.mk
-include Core/CANopenNode/303/subdir.mk
-include Core/CANopenNode/301/subdir.mk
-include Core/CANopenNode/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := MSDL_STM32F0_ROOF
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
MSDL_STM32F0_ROOF.elf \

MAP_FILES += \
MSDL_STM32F0_ROOF.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
MSDL_STM32F0_ROOF.list \


# All Target
all: main-build

# Main-build Target
main-build: MSDL_STM32F0_ROOF.elf secondary-outputs

# Tool invocations
MSDL_STM32F0_ROOF.elf MSDL_STM32F0_ROOF.map: $(OBJS) $(USER_OBJS) C:\Users\mateo\STM32CubeIDE\workspace_1.6.1\MSDL_STM32F0_ROOF\STM32F091CCTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "MSDL_STM32F0_ROOF.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 -T"C:\Users\mateo\STM32CubeIDE\workspace_1.6.1\MSDL_STM32F0_ROOF\STM32F091CCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="MSDL_STM32F0_ROOF.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

MSDL_STM32F0_ROOF.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "MSDL_STM32F0_ROOF.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) MSDL_STM32F0_ROOF.elf MSDL_STM32F0_ROOF.list MSDL_STM32F0_ROOF.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
