#include "BSP_PWM.h"
#include "common.h"

HAL_StatusTypeDef BSP_PWM_Init(PWM_Channel *channels, uint8_t num_channels)
{
    HAL_StatusTypeDef status = HAL_OK;
    for (int i = 0; i < num_channels; i++) {
        // TODO: Init the GPIO properly
        HAL_GPIO_Init(channels[i].GPIOx, &channels[i].GPIO_Pin);
    }

    // Initialize and start PWM for each channel
    for (int i = 0; i < num_channels; i++) {
        
        // Initialize timer
        HAL_TIM_PWM_Init(channels[i].htim);

        // Configure PWM output
        // TODO: make the math done here in macros
        TIM_OC_InitTypeDef sConfigOC;
        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = channels[i].PWM_DutyCycle;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(channels[i].htim, &sConfigOC, channels[i].Timer_Channel);

        // Start PWM and send status
        if(HAL_TIM_PWM_Start_IT(channels[i].htim, channels[i].Timer_Channel) == HAL_ERROR) {
            status = HAL_ERROR;
        }
    }
    return status;
}