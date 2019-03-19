PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
CXX = $(PREFIX)g++
LD = $(PREFIX)ld
OBJDUMP = $(PREFIX)objdump
OBJCOPY = $(PREFIX)objcopy
CPPFILT = $(PREFIX)c++filt

RPI = ../rpi
LUA = lua-5.3.5/src

CFLAGS  = -fsigned-char -ffreestanding -nostdinc \
          -std=c99 -Wall -pedantic -Werror \
          -I. -I$(RPI) -I../include/common -Ilua-5.3.5/src

DEFINES = -DBOARD_STRINGS -DLUA_COMPAT_5_2 -DLUA_32BITS
LDFLAGS = -T link.T

ifeq ($(strip $(v7)),1)
  CFLAGS  += -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 -mfloat-abi=hard
  CFLAGS  += -I../include/v7
  LDFLAGS += -L../lib/v7
  TARGET   = lua7
else ifeq ($(strip $(v8)),1)
  CFLAGS  += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard
  CFLAGS  += -I../include/v8-32
  LDFLAGS += -L../lib/v8-32
  TARGET   = lua8-32
else
  CFLAGS  += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
  CFLAGS  += -I../include/v6
  LDFLAGS += -L../lib/v6
  TARGET   = lua
endif

ifneq ($(DEBUG),)
  CFLAGS += -O0 -g
else
  CFLAGS += -O3 -DNDEBUG
endif

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

OBJS = syscalls.o crt0.o \
       $(RPI)/memio.o \
       $(LUA)/lapi.o $(LUA)/lcode.o $(LUA)/lctype.o $(LUA)/ldebug.o \
       $(LUA)/ldo.o $(LUA)/ldump.o $(LUA)/lfunc.o $(LUA)/lgc.o $(LUA)/llex.o \
       $(LUA)/lmem.o $(LUA)/lobject.o $(LUA)/lopcodes.o $(LUA)/lparser.o \
       $(LUA)/lstate.o $(LUA)/lstring.o $(LUA)/ltable.o $(LUA)/ltm.o \
       $(LUA)/lundump.o $(LUA)/lvm.o $(LUA)/lzio.o $(LUA)/lauxlib.o \
       $(LUA)/lbaselib.o $(LUA)/lbitlib.o $(LUA)/lcorolib.o $(LUA)/ldblib.o \
       $(LUA)/liolib.o $(LUA)/lmathlib.o $(LUA)/loslib.o $(LUA)/lstrlib.o \
       $(LUA)/ltablib.o $(LUA)/lutf8lib.o $(LUA)/loadlib.o $(LUA)/linit.o \
       $(LUA)/lua.o

all: $(TARGET).ihex

$(TARGET).ihex: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS) -lc -lm -lrt
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O ihex $@
	wc -c $@

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).ihex $(OBJS)

.PHONY: clean
