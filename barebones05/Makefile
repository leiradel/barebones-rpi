PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)ld
OBJDUMP = $(PREFIX)objdump
OBJCOPY = $(PREFIX)objcopy
CPPFILT = $(PREFIX)c++filt

CFLAGS   = -fsigned-char -ffreestanding -nostdinc -std=c99
INCLUDES = -I.
DEFINES  =
LDFLAGS  = -T link.T

ifeq ($(strip $(v7)),1)
  CFLAGS += -march=armv7-a -marm -mfpu=neon-vfpv4 -mfloat-abi=hard
  TARGET  = kernel7
else ifeq ($(strip $(v8)),1)
  CFLAGS += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard
  TARGET  = kernel8-32
else
  CFLAGS += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
  TARGET  = kernel
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

OBJS = kmain.o memio.o gpio.o mbox.o prop.o board.o led.o timer.o

all: $(TARGET).img

$(TARGET).img: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS)
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).img
	wc -c $(TARGET).img

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).img $(OBJS)

.PHONY: clean
