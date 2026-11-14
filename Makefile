# ARM Cross-Compiler Flags
CC := arm-none-eabi-gcc
AFLAGS := -mcpu=cortex-m4 -mthumb -nostdlib
CPPFLAGS := -DSTM32F401xE -Ivendor/CMSIS/Core/Include -Ivendor/CMSIS/Device/ST/STM32F4/Include 

LINKER := stm32f401re_linker.ld
LFLAGS := -T $(LINKER)
TARGET := astro-mcu-stm32f401.elf
COMMON_SRCS := startup_stm32f401re.c main.c vendor/CMSIS/Device/ST/STM32F4/Source/Templates/system_stm32f4xx.c
COMMON_OBJS := $(COMMON_SRCS:%.c=%.o)

all: $(TARGET)

# Compile and Build Elf File
$(TARGET): $(COMMON_OBJS)
	$(CC) $(AFLAGS) $(CPPFLAGS) $(LFLAGS) $^ -o $@

# Pattern Rule To Compile All Source Files
%.o: %.c
	$(CC) $(AFLAGS) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(COMMON_OBJS)