OUTPUT_ARCH(arm)
ENTRY(kmain)

SECTIONS {
  .text 0x00008000 : {
    KEEP(kmain.o(.kmain))
    *(.text .text.*)
  }
  
  .rodata : {
    . = ALIGN(4);
    *(.rodata .rodata.*)
  }

  .data : {
    . = ALIGN(4);
    *(.data .data.*)
  }

  .bss : {
    . = ALIGN(4);
    __bss_start = .;
    *(.bss .bss.*)
    *(COMMON)
    . = ALIGN(4);
    __bss_end = .;
  }

  . = ALIGN(16);
  __heap_start = .;

  /DISCARD/ : {
    *(*)
  }
}
