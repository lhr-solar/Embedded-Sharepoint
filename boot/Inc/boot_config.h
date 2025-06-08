#ifndef __BOOT_CONFIG_H
#define __BOOT_CONFIG_H

#define BOOT_UART_TX_PIN GPIO_PIN_2
#define BOOT_UART_TX_PORT GPIOA
#define BOOT_UART_RX_PIN GPIO_PIN_3
#define BOOT_UART_RX_PORT GPIOA
#define BOOT_UART_AF GPIO_AF7_USART2
#define BOOT_UART_INST USART2
#define BOOT_UART_CLOCK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()
#define BOOT_UART_CLOCK_DISABLE() __HAL_RCC_USART2_CLK_DISABLE()
#define BOOT_UART_IRQN USART2_IRQn

void boot_led_init();
void boot_uart_init();
void boot_led_deinit();
void boot_uart_deinit();

#endif
