/* Linker script for 1Bitsquared 1Bitsy Dev Board (STM32F415RGT6, 1024K flash, 128K RAM). */

/* Define memory regions. */
MEMORY
{
        rom (rx) : ORIGIN = 0x08000000, LENGTH = 1024K
        ram (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

/* Include the common ld script. */
INCLUDE libopencm3_stm32f4.ld
