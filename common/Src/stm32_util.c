#include "stm32xx_hal.h"

void GPIO_CLK_ENABLE(GPIO_TypeDef *gpio_inst){
    switch((uint32_t)gpio_inst){
	case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_ENABLE(); break;
	case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_ENABLE(); break;
	case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_ENABLE(); break;
	case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_ENABLE(); break;
	case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_ENABLE(); break;
    }
}

void GPIO_CLK_DISABLE(GPIO_TypeDef *gpio_inst){
    switch((uint32_t)gpio_inst){
	case (uint32_t)GPIOA: __HAL_RCC_GPIOA_CLK_DISABLE(); break;
	case (uint32_t)GPIOB: __HAL_RCC_GPIOB_CLK_DISABLE(); break;
	case (uint32_t)GPIOC: __HAL_RCC_GPIOC_CLK_DISABLE(); break;
	case (uint32_t)GPIOD: __HAL_RCC_GPIOD_CLK_DISABLE(); break;
	case (uint32_t)GPIOE: __HAL_RCC_GPIOE_CLK_DISABLE(); break;
    }
}
