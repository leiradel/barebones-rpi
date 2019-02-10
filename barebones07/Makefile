PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)ld
OBJDUMP = $(PREFIX)objdump
OBJCOPY = $(PREFIX)objcopy
CPPFILT = $(PREFIX)c++filt

CFLAGS  = -fsigned-char -ffreestanding -nostdinc -std=c99 -Iinclude/common
DEFINES = -DBOARD_STRINGS
LDFLAGS = -T link.T

ifeq ($(strip $(v7)),1)
  CFLAGS  += -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 -mfloat-abi=hard
  CFLAGS  += -Iinclude/v7
  LDFLAGS += -Llib/v7
  TARGET   = kernel7
else ifeq ($(strip $(v8)),1)
  CFLAGS  += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard
  CFLAGS  += -Iinclude/v8-32
  LDFLAGS += -Llib/v8-32
  TARGET   = kernel8-32
else
  CFLAGS  += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
  CFLAGS  += -Iinclude/v6
  LDFLAGS += -Llib/v6
  TARGET   = kernel
endif

ifneq ($(DEBUG),)
  CFLAGS += -O0 -g
else
  CFLAGS += -Os -DNDEBUG
endif

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

OBJS = kmain.o memio.o gpio.o mbox.o prop.o board.o led.o \
       timer.o aux.o glod.o irq.o syscalls.o \
       main.o

all: $(TARGET).img

$(TARGET).img: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS) -lc -lm -lrt
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).img
	wc -c $(TARGET).img

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).img $(OBJS)

.PHONY: clean
