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

CFLAGS += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard
TARGET  = kernel

ifneq ($(DEBUG),)
  CFLAGS += -O0 -g
else
  CFLAGS += -Os -DNDEBUG
endif

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

OBJS = kmain.o

all: $(TARGET).img

$(TARGET).img: $(OBJS)
	$(LD) -o $(TARGET).elf -Map $(TARGET).map $(LDFLAGS) $(OBJS)
	$(OBJDUMP) -d $(TARGET).elf | $(CPPFILT) > $(TARGET).lst
	$(OBJCOPY) $(TARGET).elf -O binary $(TARGET).img
	wc -c $(TARGET).img

clean:
	rm -f $(TARGET).elf $(TARGET).map $(TARGET).lst $(TARGET).img $(OBJS)

.PHONY: clean
