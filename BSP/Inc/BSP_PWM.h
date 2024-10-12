#ifndef BSP_PWM_H
#define BSP_PWM_H

//figure out #includes
#include <stdint.h>
#include "stm32xx_hal.h"

typedef struct{
    TIM_HandleTypeDef* timHandle; 
    uint32_t channel;
    uint32_t speed;
    uint32_t dutyCycle; 
    uint8_t stopFlag;
} PWM_Info;

/**
 * @brief Initialize TIM_PWM
 * 
 * @param config
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_PWM_Init(TIM_HandleTypeDef* timHandle, uint32_t channel, QueueHandle_t* TxPtr);

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
uint32_t BSP_PWM_GetSpeed(TIM_HandleTypeDef* timHandle, uint32_t channel);

#endif