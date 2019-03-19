PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)ld
OBJDUMP = $(PREFIX)objdump
OBJCOPY = $(PREFIX)objcopy
CPPFILT = $(PREFIX)c++filt

RPI = ../rpi

CFLAGS  = -fsigned-char -ffreestanding -nostdinc \
          -std=c99 -Wall -pedantic -Werror \
          -I. -I$(RPI) -I../include/common -Ilua-5.3.5/src

DEFINES = -DBOARD_STRINGS
LDFLAGS = -T link.T

ifeq ($(strip $(v7)),1)
  CFLAGS  += -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 -mfloat-abi=hard
  CFLAGS  += -I../include/v7
  LDFLAGS += -L../lib/v7
  TARGET   = kernel7
else ifeq ($(strip $(v8)),1)
  CFLAGS  += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard
  CFLAGS  += -I../include/v8-32
  LDFLAGS += -L../lib/v8-32
  TARGET   = kernel8-32
else
  CFLAGS  += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
  CFLAGS  += -I../include/v6
  LDFLAGS += -L../lib/v6
  TARGET   = kernel
endif

ifneq ($(DEBUG),)
  CFLAGS += -O0 -g
else
  CFLAGS += -O3 -DNDEBUG
endif

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

OBJS = kmain.o glod.o isr.o syscalls.o tty.o main.o \
       $(RPI)/aux.o $(RPI)/board.o $(RPI)/gpio.o $(RPI)/led.o \
       $(RPI)/mbox.o $(RPI)/memio.o $(RPI)/prop.o $(RPI)/timer.o

all: $(TARGET).img

$(TARGET).img: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS) -lrt -lc -lm
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O binary $@
	wc -c $@

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).img $(OBJS)

.PHONY: clean
