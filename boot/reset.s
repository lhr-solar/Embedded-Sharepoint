.syntax unified
.cpu cortex-m4
.fpu softvfp
.thumb

.global  g_pfnVectors
.global  Default_Handler

.extern  _estack
.extern _app_start
.extern _app_len
.extern _boot_start
.extern _boot_end

// Pointer to the start address of the initialized data section in flash.
.word _sidata
// Pointer to the start address of the initialized data section in SRAM.
.word _sdata
// Pointer to the end address of the initialized data section in SRAM.
.word _edata
// Pointer to the start address of the uninitialized data section (BSS) in SRAM.
.word _sbss
// Pointer to the end address of the uninitialized data section (BSS) in SRAM.
.word _ebss
.word _estack
.word _app_start
.word _app_main
.word _app_len
.word _boot_start
.word _boot_end

.section  .text.Reset_Handler
.global Reset_Handler
.type  Reset_Handler, %function
Reset_Handler:
    /* Load _estack into sp */
    ldr sp, =_estack

    /* Call the clock system initialization function.*/
    @ bl  SystemInit

    /* Copy the data segment initializers from flash to SRAM */  
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b LoopCopyDataInit

    CopyDataInit:
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    adds r3, r3, #4

    LoopCopyDataInit:
    adds r4, r0, r3
    cmp r4, r1
    bcc CopyDataInit

    /* Zero fill the bss segment. */
    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b LoopFillZerobss

    FillZerobss:
    str  r3, [r2]
    adds r2, r2, #4

    LoopFillZerobss:
    cmp r2, r4
    bcc FillZerobss

    /* Call static constructors */
    bl __libc_init_array
    
    bl boot
    bx lr
.size  Reset_Handler, .-Reset_Handler

.section .text.startapp
.global startapp
.type startapp, %function
startapp:
    /* Set the stack pointer to _estack */
    ldr sp, =_estack

    @ /* Jump to the application's entry point at _app_main */
    @ ldr r0, =_app_main
    @ blx r0
    @ bx lr

    bl SystemInit
    
    /* Trigger a software reset */
    ldr r0, =_app_start
    ldr r0, [r0, #4]
    bx r0
.size startapp, .-startapp