ifeq ($(strip $(v7)),1)
  TARGET = kernel7
else ifeq ($(strip $(v8)),1)
  TARGET = kernel8-32
else
  TARGET = kernel
endif

all: kernel/$(TARGET).img

kernel/$(TARGET).img: $(OBJS)
	cd kernel && make
	cd info && make

clean:
	cd kernel && make clean
	cd info && make clean

.PHONY: clean
