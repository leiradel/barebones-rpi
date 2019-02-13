OBJS = \
	fixsfsi.o \
	floatuntisf.o \
	divsc3.o \
	muldc3.o \
	trunctfdf2.o \
	fixtfti.o \
	fixdfsi.o \
	udivdi3.o \
	fixunssfsi.o \
	truncdfhf2.o \
	ffssi2.o \
	udivmoddi4.o \
	clzti2.o \
	ucmpti2.o \
	apple_versioning.o \
	floatundisf.o \
	fixunsxfsi.o \
	subdf3.o \
	fixunssfti.o \
	comparedf2.o \
	floatsisf.o \
	ctzsi2.o \
	trampoline_setup.o \
	bswapsi2.o \
	popcountdi2.o \
	floattixf.o \
	clzdi2.o \
	divdf3.o \
	absvsi2.o \
	paritysi2.o \
	floatundixf.o \
	fixxfdi.o \
	udivti3.o \
	addsf3.o \
	divsf3.o \
	powidf2.o \
	lshrdi3.o \
	umodti3.o \
	muldf3.o \
	fixdfdi.o \
	mulodi4.o \
	floatsitf.o \
	paritydi2.o \
	moddi3.o \
	extenddftf2.o \
	fixunsxfti.o \
	subvti3.o \
	mulxc3.o \
	floatundidf.o \
	divtf3.o \
	divmodsi4.o \
	fixunsdfdi.o \
	negdi2.o \
	bswapdi2.o \
	fixtfsi.o \
	negdf2.o \
	divtc3.o \
	fixunssfdi.o \
	mulsc3.o \
	addvdi3.o \
	fixtfdi.o \
	subvdi3.o \
	mulvsi3.o \
	powitf2.o \
	subvsi3.o \
	negvdi2.o \
	ucmpdi2.o \
	umoddi3.o \
	absvdi2.o \
	divdc3.o \
	floatdixf.o \
	parityti2.o \
	gcc_personality_v0.o \
	lshrti3.o \
	mulosi4.o \
	multi3.o \
	negti2.o \
	clear_cache.o \
	modsi3.o \
	floatsidf.o \
	divxc3.o \
	addvsi3.o \
	clzsi2.o \
	fixunsxfdi.o \
	arm/aeabi_div0.o \
	arm/aeabi_drsub.o \
	arm/aeabi_cfcmpeq_check_nan.o \
	arm/aeabi_cdcmpeq_check_nan.o \
	arm/aeabi_frsub.o \
	floatditf.o \
	negvti2.o \
	floattisf.o \
	floatunditf.o \
	popcountti2.o \
	udivmodsi4.o \
	ffsti2.o \
	subtf3.o \
	floattitf.o \
	negvsi2.o \
	divdi3.o \
	ctzdi2.o \
	eprintf.o \
	ashrdi3.o \
	ffsdi2.o \
	floatdidf.o \
	addtf3.o \
	divmoddi4.o \
	extendsftf2.o \
	umodsi3.o \
	floatuntixf.o \
	powisf2.o \
	muldi3.o \
	fixunstfti.o \
	adddf3.o \
	os_version_check.o \
	fixxfti.o \
	trunctfsf2.o \
	extendsfdf2.o \
	divsi3.o \
	mulvdi3.o \
	fixunstfdi.o \
	udivmodti4.o \
	negsf2.o \
	mulsf3.o \
	extendhfsf2.o \
	fixunstfsi.o \
	truncsfhf2.o \
	floatdisf.o \
	floatunsisf.o \
	floatunsidf.o \
	comparesf2.o \
	addvti3.o \
	fixdfti.o \
	mingw_fixfloat.o \
	fixsfdi.o \
	multf3.o \
	powixf2.o \
	floatuntitf.o \
	popcountsi2.o \
	cpu_model.o \
	modti3.o \
	truncdfsf2.o \
	subsf3.o \
	ashlti3.o \
	absvti2.o \
	floattidf.o \
	comparetf2.o \
	ashldi3.o \
	cmpti2.o \
	floatunsitf.o \
	fixunsdfti.o \
	ashrti3.o \
	muloti4.o \
	fixsfti.o \
	floatuntidf.o \
	udivsi3.o \
	int_util.o \
	multc3.o \
	fixunsdfsi.o \
	divti3.o \
	ctzti2.o \
	mulvti3.o \
	cmpdi2.o \
	arm/subdf3vfp.o \
	arm/save_vfp_d8_d15_regs.o \
	arm/subsf3vfp.o \
	arm/floatunssidfvfp.o \
	arm/floatsisfvfp.o \
	arm/comparesf2.o \
	arm/eqsf2vfp.o \
	arm/fixdfsivfp.o \
	arm/ledf2vfp.o \
	arm/muldf3vfp.o \
	arm/aeabi_memset.o \
	arm/restore_vfp_d8_d15_regs.o \
	arm/unordsf2vfp.o \
	arm/fixunssfsivfp.o \
	arm/sync_synchronize.o \
	arm/switch16.o \
	arm/truncdfsf2vfp.o \
	arm/addsf3.o \
	arm/clzdi2.o \
	arm/divsf3vfp.o \
	arm/lesf2vfp.o \
	arm/negsf2vfp.o \
	arm/aeabi_cfcmp.o \
	arm/fixunsdfsivfp.o \
	arm/fixsfsivfp.o \
	arm/floatsidfvfp.o \
	arm/aeabi_memmove.o \
	arm/aeabi_dcmp.o \
	arm/switch8.o \
	arm/clzsi2.o \
	arm/divmodsi4.o \
	arm/aeabi_ldivmod.o \
	arm/umodsi3.o \
	arm/negdf2vfp.o \
	arm/ltdf2vfp.o \
	arm/mulsf3vfp.o \
	arm/udivsi3.o \
	arm/nesf2vfp.o \
	arm/adddf3vfp.o \
	arm/aeabi_uidivmod.o \
	arm/gesf2vfp.o \
	arm/aeabi_cdcmp.o \
	arm/ltsf2vfp.o \
	arm/divdf3vfp.o \
	arm/unorddf2vfp.o \
	arm/bswapsi2.o \
	arm/gtdf2vfp.o \
	arm/extendsfdf2vfp.o \
	arm/divsi3.o \
	arm/eqdf2vfp.o \
	arm/aeabi_memcmp.o \
	arm/addsf3vfp.o \
	arm/gedf2vfp.o \
	arm/switch32.o \
	arm/floatunssisfvfp.o \
	arm/modsi3.o \
	arm/aeabi_idivmod.o \
	arm/bswapdi2.o \
	arm/gtsf2vfp.o \
	arm/chkstk.o \
	arm/aeabi_memcpy.o \
	arm/nedf2vfp.o \
	arm/aeabi_uldivmod.o \
	arm/switchu8.o \
	arm/aeabi_fcmp.o \
	arm/udivmodsi4.o

CC = arm-none-eabi-gcc
CFLAGS = -MMD -MP -O3 -Wall -Wstrict-prototypes -fno-builtin \
	       -fno-strict-aliasing -fno-common -ffunction-sections -fno-pic \
	       -fwrapv -mno-unaligned-access -ffreestanding -nostdlib \
	       -nostartfiles -std=c11 -D__ARM_EABI__

ifeq ($(strip $(v7)),1)
	CFLAGS += -march=armv7-a -mtune=cortex-a7 -marm -mfpu=neon-vfpv4 \
	          -mfloat-abi=hard
	TARGET  = librt_v7.a
else ifeq ($(strip $(v8)),1)
	CFLAGS += -march=armv8-a -mtune=cortex-a53 -marm -mfpu=neon-fp-armv8 \
	          -mfloat-abi=hard
	TARGET  = librt_v8-32.a
else
	CFLAGS += -march=armv6k -mtune=arm1176jzf-s -marm -mfpu=vfp \
	          -mfloat-abi=hard
	TARGET  = librt_v6.a
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJS)
	arm-none-eabi-ar rvs $@ $+

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
