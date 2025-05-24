#ifndef STM32xx_HAL_H
#define STM32xx_HAL_H

// STM32
#if defined(STM32F4xx)
    #include "stm32f4xx.h"
    #include "stm32f4xx_hal.h"
    #include "stm32f4xx_hal_conf.h"

    // Supported ports
    #if defined(STM32F401xE)
        #include "stm32f401xe.h"
    #elif defined(STM32F413xx)
        #include "stm32f413xx.h"

        #define LED_PIN GPIO_PIN_7
        #define LED_PORT GPIOB
    #elif defined(STM32F446xx)
        #include "stm32f446xx.h"
        
        #define DBG_UART_TX_PIN GPIO_PIN_2
        #define DBG_UART_TX_PORT GPIOA
        #define DBG_UART_RX_PIN GPIO_PIN_3
        #define DBG_UART_RX_PORT GPIOA
        #define DBG_UART_AF GPIO_AF7_USART2
        #define DBG_UART_INST USART2
        #define DBG_UART_GPIO_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE();
        #define DBG_UART_CLOCK_ENABLE() __HAL_RCC_USART2_CLK_ENABLE()
        #define DBG_UART_CLOCK_DISABLE() __HAL_RCC_USART2_CLK_DISABLE()
	#define DBG_UART_IRQN USART2_IRQn

	#define LED_PIN GPIO_PIN_5
        #define LED_PORT GPIOA
    #elif defined(STM32F429xx)
        #include "stm32f429xx.h"
	
	#define DBG_UART_TX_PIN GPIO_PIN_8
	#define DBG_UART_TX_PORT GPIOD
	#define DBG_UART_RX_PIN GPIO_PIN_9
	#define DBG_UART_RX_PORT GPIOD
	#define DBG_UART_AF GPIO_AF7_USART3
	#define DBG_UART_INST USART3
	#define DBG_UART_GPIO_ENABLE() __HAL_RCC_GPIOD_CLK_ENABLE();
	#define DBG_UART_CLOCK_ENABLE() __HAL_RCC_USART3_CLK_ENABLE()
	#define DBG_UART_CLOCK_DISABLE() __HAL_RCC_USART3_CLK_DISABLE()
	#define DBG_UART_IRQN USART3_IRQn
	
	#define LED_PIN GPIO_PIN_0
	#define LED_PORT GPIOB
    #endif
#elif defined(STM32L4xx)
    #include "stm32l4xx.h"
    #include "stm32l4xx_hal.h"
#else
    #error "No valid STM32 series defined. Please define either STM32F4xx or STM32L4xx."
#endif

// FreeRTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

// Standard Library
#include <stdint.h>
#include <stdbool.h>

// Init function prototypes
void Error_Handler(void);
void HAL_MspInit(void);
void SystemClock_Config(void);

#endif /* STM32xs_HAL_H */
