OUTPUT_ARCH(riscv)
OUTPUT_FORMAT(elf32-littleriscv)

ENTRY (_start)

MEMORY
{
  OCRAM (rwx) : ORIGIN = 0x22020000, LENGTH = 64K
  WRAM (rwx) : ORIGIN = 0x22030000, LENGTH = 160K
  DRAM (rwx) : ORIGIN = 0x3ef80000, LENGTH = 16K
  VRAM (rwx) : ORIGIN = 0x3f000000, LENGTH = 16K
  XRAM (rwx) : ORIGIN = 0x40000000, LENGTH = 16K

  PSRAM (rwx) : ORIGIN = 0x50000000, LENGTH = 64M
  FLASH (rx) : ORIGIN = 0x58000000, LENGTH = 64M

  ROM (rx) : ORIGIN = 0x90000000, LENGTH = 128K
}

SECTIONS {
  . = ORIGIN(OCRAM);
  __stack_size = 2K;

  .text : {
    __code_start__ = .;
    KEEP(*(.text.boot))
    *(.text*)
    . = ALIGN(4);
    __code_end__ = .;
  }

  .rodata . : {
    . = ALIGN(4);
    *(.rodata*)
  }

  .sdata : {
    . = ALIGN(4);
    __global_pointer$ = . + 0x800;
    *(.sdata*)
    *(.srodata*)
  }

  .data : {
    __data_start__ = .;
    *(.data*)
    __data_end__ = .;
  }

  .bss : {
    __bss_start__ = .;
    *(.bss*)
    *(.sbss*)
    *(COMMON)
    __bss_end__ = .;
    . = ALIGN(8);
    __heap_start__ = .;
  }

  /DISCARD/ : {
    *(.comment)
    *(.riscv.attributes)
  }

  .stack (NOLOAD) : {
    __stack_bottom__ = .;
    . += __stack_size;
    __stack_top__ = .;
  }
}
