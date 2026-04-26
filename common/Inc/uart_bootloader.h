#ifndef UART_BOOTLOADER_H_
#define UART_BOOTLOADER_H_

#include <stdbool.h>
#include <stdint.h>

#define UART_BOOTLOADER_COMMAND "ESBLT_BOOT\n"
#define UART_BOOTLOADER_MAGIC_WORD (0x4553424CUL)

bool uart_bootloader_feed_command_byte(uint8_t byte);
void uart_bootloader_command_reset(void);
void uart_bootloader_request_reset(void);
bool uart_bootloader_consume_request(void);

#endif /* UART_BOOTLOADER_H_ */
