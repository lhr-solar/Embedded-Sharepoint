#include <stdint.h>
#include "stm32xx_hal.h"

int main(){

    HAL_Init();

    GPIO_InitTypeDef pwm_config = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_8,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF1_TIM1
    };

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5,
    };

    GPIO_InitTypeDef pin_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_6,
    };

    
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &pwm_config); // initialize GPIOA with pwm_config
    HAL_GPIO_Init(GPIOA, &led_config);
    HAL_GPIO_Init(GPIOA, &pin_config);

    TIM_HandleTypeDef htim1;
    htim1.Instance = TIM1;
    htim1.Init.Prescaler = 8-1;
    htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim1.Init.Period = 10000 - 1;
    // htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_PWM_Init(&htim1);

    TIM_OC_InitTypeDef sConfigOC = {0};
    sConfigOC.OCMode = TIM_OCMODE_PWM1;  // Set PWM mode 1
    sConfigOC.Pulse = 5000;               // Set the pulse to 50% of the period (500 out of 1000)
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);

    HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);
    // TIM1->CCR1 = 500;
    // __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, 500);

    while(1) {
        HAL_Delay(500);
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

        HAL_TIM_PWM_Stop_IT(&htim1, TIM_CHANNEL_1); // testing changing duty cycle
        sConfigOC.Pulse = 2500;
        HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start_IT(&htim1, TIM_CHANNEL_1);

        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

// void TIM1_IRQHandler(TIM_HandleTypeDef *htim) {
//     // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
//     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
// }

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6); // testing interrupt callback
}