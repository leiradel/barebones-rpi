PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)ld
OBJDUMP = $(PREFIX)objdump
OBJCOPY = $(PREFIX)objcopy
CPPFILT = $(PREFIX)c++filt

RPI = ../rpi
CRT0 = ../crt0

CFLAGS  = -fsigned-char -ffreestanding -nostdinc \
          -std=c99 -Wall -pedantic -Werror \
          -I. -I$(RPI) -I../include/common -Ilua-5.3.5/src

DEFINES = -DBOARD_STRINGS
LDFLAGS = -T link.T

ifeq ($(strip $(v7)),1)
  CFLAGS  += -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 -mfloat-abi=hard
  CFLAGS  += -I../include/v7
  LDFLAGS += -L../lib/v7
  TARGET   = echo7
else ifeq ($(strip $(v8)),1)
  CFLAGS  += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard
  CFLAGS  += -I../include/v8-32
  LDFLAGS += -L../lib/v8-32
  TARGET   = echo8-32
else
  CFLAGS  += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
  CFLAGS  += -I../include/v6
  LDFLAGS += -L../lib/v6
  TARGET   = echo
endif

ifneq ($(DEBUG),)
  CFLAGS += -O0 -g
else
  CFLAGS += -O3 -DNDEBUG
endif

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

OBJS = main.o \
       $(CRT0)/syscalls.o $(CRT0)/crt0.o \
       $(RPI)/board.o $(RPI)/gpio.o $(RPI)/led.o $(RPI)/mbox.o $(RPI)/memio.o \
       $(RPI)/prop.o $(RPI)/timer.o

all: $(TARGET).ihex

$(TARGET).ihex: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS) -lc -lm -lrt
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O ihex $@
	wc -c $@

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).ihex $(OBJS)

.PHONY: clean
