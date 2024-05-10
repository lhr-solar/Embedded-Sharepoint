#ifndef BSP_PWM_H
#define BSP_PWM_H

#include <stdint.h>
#include "stm32f4xx_hal.h"

typedef struct {
    GPIO_TypeDef *GPIOx;        // GPIO port
    uint16_t GPIO_Pin;          // GPIO pin
    TIM_TypeDef *TIMx;          // Timer
    uint32_t PWM_DutyCycle;     // PWM duty cycle
    uint8_t Timer_Channel;      // Timer channel
} PWM_Channel;

/**
 * @brief Initialize PWM Ports
*/
HAL_StatusTypeDef BSP_PWM_Init(PWM_Channel *channels, uint8_t num_channels);



#endif