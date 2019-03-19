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
	cd echo && make
	cd info && make
	cd lua && make

clean:
	cd kernel && make clean
	cd echo && make clean
	cd info && make clean
	cd lua && make clean

.PHONY: clean
