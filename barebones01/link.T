OUTPUT_ARCH(arm)
ENTRY(kmain)

SECTIONS {
  .text 0x00008000 : {
    KEEP(kmain.o(.kmain))
    *(.text .text.*)
  }
  
  .rodata : {
    . = ALIGN(4);
    rodata_start = .;
    *(.rodata .rodata.*)
    rodata_end = .;
  }

  .data : {
    . = ALIGN(4);
     data_start = .;
    *(.data .data.*)
     data_end = .;
  }

  .bss : {
    . = ALIGN(4);
    *(.bss .bss.*)
    *(COMMON)
  }

  /DISCARD/ : {
    *(*)
  }
}
