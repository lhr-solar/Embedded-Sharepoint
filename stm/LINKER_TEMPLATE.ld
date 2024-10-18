/*
* Linker script template (heavily derived from STM32CubeMX linker scripts)
*/

/* Entry Point */
ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);    /* end of RAM */

_Min_Heap_Size = 0x0;      /* we don't use the heap  */
_Min_Stack_Size = 0x400;   /* default amount of stack (change if necessary) */

MEMORY{
    RAM (xrw) : ORIGIN = 0x20000000, LENGTH = ${RAM_SIZE}
    VECTOR (rx) : ORIGIN = 0x08000000, LENGTH = 0x200
    FLASH (rx) : ORIGIN = 0x08000200, LENGTH = ${FLASH_SIZE}
    BOOT (rx) : ORIGIN = ${BOOT_OFFSET}, LENGTH = ${BOOT_SIZE}
}

SECTIONS{
    .isr_vector :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Startup code */
        . = ALIGN(4);
    } >VECTOR

    .text :
    {
        . = ALIGN(4);
        *(.text)           /* .text sections (code) */
        *(.text*)          /* .text* sections (code) */
        *(.glue_7)         /* glue arm to thumb code */
        *(.glue_7t)        /* glue thumb to arm code */
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;        /* define a global symbols at end of code */
    } >${FLASH_NAME}

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)         /* .rodata sections (constants, strings, etc.) */
        *(.rodata*)        /* .rodata* sections (constants, strings, etc.) */
        . = ALIGN(4);
    } >${FLASH_NAME}

    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >${FLASH_NAME}

    .ARM : {
        __exidx_start = .;
        __exidx_end = .;
    } >${FLASH_NAME}

    _sidata = LOADADDR(.data);

    .data : 
    {
        . = ALIGN(4);
        _sdata = .;
        *(.data)
        *(.data*)

        . = ALIGN(4);
        _edata = .;
    } >RAM AT> ${FLASH_NAME}

    . = ALIGN(4);
    .bss :
    {
        _sbss = .;
        __bss_start__ = _sbss;
        *(.bss)
        *(.bss*)
        *(COMMON)

        . = ALIGN(4);
        _ebss = .;
        __bss_end__ = _ebss;
    } >RAM

    ._user_heap_stack :
    {
        . = ALIGN(8);
        PROVIDE ( end = . );
        PROVIDE ( _end = . );
        . = . + _Min_Heap_Size;
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } >RAM

    /DISCARD/ :
    {
        libc.a ( * )
        libm.a ( * )
        libgcc.a ( * )
    }

    .ARM.attributes 0 : { *(.ARM.attributes) }

}