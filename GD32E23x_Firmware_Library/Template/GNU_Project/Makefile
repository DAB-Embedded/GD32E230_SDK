# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# Author: DAB-Embedded
# ------------------------------------------------

######################################
# target
######################################
TARGET = Template


######################################
# building variables
######################################
# debug build?
ifdef DEBUG_BUILD
DEBUG = 1
endif

ifeq ($(DEBUG), 1)
# no optimization
OPT = -Og
else
# optimization
OPT = -O2
endif


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
../gd32e23x_it.c \
../main.c \
../systick.c \
../../Utilities/gd32e230c_eval.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_rtc.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_dbg.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_gpio.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_dma.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_rcu.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_adc.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_cmp.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_crc.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_exti.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_fmc.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_fwdgt.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_i2c.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_misc.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_pmu.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_spi.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_syscfg.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_timer.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_usart.c \
../../Firmware/GD32E23x_standard_peripheral/Source/gd32e23x_wwdgt.c \
../../Firmware/CMSIS/GD/GD32E23x/Source/system_gd32e23x.c

# ASM sources
ASM_SOURCES =  \
../../Firmware/CMSIS/GD/GD32E23x/Source/GNU/startup_gd32e23x.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m23

# float-abi
FLOAT-ABI = -mfloat-abi=soft

# mcu
MCU = $(CPU) -mthumb $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS =

# C defines
C_DEFS =  \
-DGD32E230

# AS includes
AS_INCLUDES =

# C includes
C_INCLUDES =  \
-I../../Firmware/CMSIS/GD/GD32E23x/Include \
-I../../Utilities \
-I../../Firmware/CMSIS \
-I../../Template \
-I../../Firmware/CMSIS/GD/GD32E23x/Include \
-I../../Firmware/CMSIS/GD/GD32E23x/Source/GNU \
-I../../Firmware/GD32E23x_standard_peripheral/Include


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = GD32E230K6U6_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LIBDIR =
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
