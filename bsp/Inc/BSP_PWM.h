#ifndef BSP_PWM_H
#define BSP_PWM_H

//figure out #includes
#include <stdint.h>
#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

typedef struct{
    TIM_HandleTypeDef* timHandle; 
    uint8_t channel;
    uint64_t speed;
    uint8_t dutyCycle; 
} PWM_Info;

void MX_GPIO_Init();

HAL_StatusTypeDef BSP_PWM_TIM_Init(TIM_HandleTypeDef* timHandle);
/**
 * @brief Initialize TIM_PWM
 * 
 * @param config
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_PWM_Channel_Init(TIM_HandleTypeDef* timHandle, uint8_t channel);

/**
 * @brief Set PWM frequency and duty cycle, or adds to transmit queue
 * 
 * @param timer handle that being configured
 * @return HAL_StatusTypeDef
 */
HAL_StatusTypeDef BSP_PWM_Set(TIM_HandleTypeDef* timHandle, uint8_t channel, uint8_t dutyCycle, uint64_t speed);

/**
 * @brief Get PWM speed
 * 
 * @param timer handle to get speed from
 * @return HAL_StatusTypeDef
 */
// uint32_t BSP_PWM_GetSpeed(TIM_HandleTypeDef* timHandle, uint32_t channel);

#endif