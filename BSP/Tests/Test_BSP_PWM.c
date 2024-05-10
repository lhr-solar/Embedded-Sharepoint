#include "queue.h"
#include "BSP_PWM.h"

int main () {
    HAL_Init();

    TIM_HandleTypeDef htim1;
    TIM_HandleTypeDef htim2;

    htim1.Instance = TIM1;
    htim2.Instance = TIM2;

    PWM_Channel channels[] = {
        {GPIOA, GPIO_PIN_0, &htim1, 500, TIM_CHANNEL_1},
        {GPIOB, GPIO_PIN_0, &htim2, 700, TIM_CHANNEL_2} 
    };

    PWM_Init(channels, sizeof(channels) / sizeof(channels[0]));

    // Main loop
    while (1) {
        // Your application code here
    }
}
