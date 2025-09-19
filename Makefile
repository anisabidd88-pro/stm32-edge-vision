# Template Makefile (edit paths for your system)
TOOLCHAIN_PREFIX = arm-none-eabi-
CC = $(TOOLCHAIN_PREFIX)gcc
CFLAGS = -mcpu=cortex-m7 -mthumb -O2 -ffunction-sections -fdata-sections -DUSE_HAL_DRIVER
LDFLAGS = -TSTM32H743_FLASH.ld -Wl,--gc-sections
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
TARGET = stm32_edge_vision.elf

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
