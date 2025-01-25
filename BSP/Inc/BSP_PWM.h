#ifndef BSP_PWM_H
#define BSP_PWM_H

//figure out #includes
#include <stdint.h>
// #include "stm32xx_hal.h"
#include "stm32f4xx_hal.h"
// #include "stm32f4xx.h"
// #include "stm32f4xx_hal_tim.h"
// #include "stm32f4xx_hal_gpio.h"

typedef struct{
    TIM_HandleTypeDef* timHandle; 
    uint32_t channel;
    uint32_t speed;
    uint32_t dutyCycle; 
    uint8_t stopFlag;
} PWM_Info;



HAL_StatusTypeDef BSP_PWM_TIM_Init(TIM_HandleTypeDef* timHandle);
/**
 * @brief Initialize TIM_PWM
 * 
 * @param config
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_PWM_Channel_Init(TIM_HandleTypeDef* timHandle, uint32_t channel);

/**
 * @brief Set PWM frequency and duty cycle, or adds to transmit queue
 * 
 * @param timer handle that being configured
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_PWM_Set(TIM_HandleTypeDef* timHandle, uint32_t channel, uint32_t dutyCycle, uint32_t speed);

/**
 * @brief Get PWM speed
 * 
 * @param timer handle to get speed from
 * @return HAL_StatusTypeDef
 */
// uint32_t BSP_PWM_GetSpeed(TIM_HandleTypeDef* timHandle, uint32_t channel);

#endif