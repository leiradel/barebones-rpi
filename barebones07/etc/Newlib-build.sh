#! /bin/sh

mkdir v6
make distclean

./configure --target=arm-none-eabi --disable-multilib --disable-shared --disable-newlib-supplied-syscalls --enable-newlib-io-long-long --prefix=`pwd`/v6

make CFLAGS_FOR_TARGET="-O2 -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp -mfloat-abi=hard" -j
make install


mkdir v7
make distclean

./configure --target=arm-none-eabi --disable-multilib --disable-shared --disable-newlib-supplied-syscalls --enable-newlib-io-long-long --prefix=`pwd`/v7

make CFLAGS_FOR_TARGET="-O2 -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 -mfloat-abi=hard" -j
make install


mkdir v8-32
make distclean

./configure --target=arm-none-eabi --disable-multilib --disable-shared --disable-newlib-supplied-syscalls --enable-newlib-io-long-long --prefix=`pwd`/v8-32

make CFLAGS_FOR_TARGET="-O2 -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 -mfloat-abi=hard" -j
make install
