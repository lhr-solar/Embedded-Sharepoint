#include "ADC.h"

ADC_HandleTypeDef hadc;
static QueueHandle_t* adcReadings;

adc_status_t ADC_Init(ADC_InitTypeDef init, QueueHandle_t* rxQueue) {
    ADC_ChannelConfTypeDef sConfig = {0};

    adcReadings = rxQueue;

    hadc.Init = init; // set the init structure to InitTypeDef
    if (HAL_ADC_Init(&hadc) != HAL_OK) Error_Handler();
}


adc_status_t ADC_OneShotRead(uint32_t channel, uint32_t samplingTime, bool blocking) {
    if (xQueueIsQueueFullFromISR(adcReadings)) {
        return ADC_QUEUE_FULL;
    }

    ADC_ChannelConfTypeDef sConfig = {0}; // resets the channel config

    // configure only one channel
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = samplingTime;

    if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK) {
        return ADC_CHANNEL_CONFIG_FAIL;
    }

    // trigger interrupt to read 
    HAL_ADC_Start_IT(&hadc);   

    return ADC_OK; // not sure if i should actually send an "OK" interrupt b/c the callback is still pending
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *h) {
    /*
    Interrupt triggers this callback when the conversion is complete
    */
   BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (h == &hadc) {
        int rawVal = HAL_ADC_GetValue(&hadc);

        // push value to q
        xQueueSendFromISR(adcReadings, &rawVal, &higherPriorityTaskWoken);

    } else {
        // define a flag in ISR  
        #define READ_FAILED
    }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc) {
     _HAL_RCC_ADC1_CLK_ENABLE(); 

    /**
     * GPIO Inits for ADC Reading [using ADC1]
     * PA_3     ADC1_3
     * PC_0     ADC1_10
     * PC_3     ADC1_13
     * PA_7     ADC1_7
     * PA_4     ADC1_4
     * PB_1     ADC1_9
     * PC_2     ADC1_12
     * PA_0     ADC1_0
     * PB_0     ADC1_8
     * PA_5     ADC1_5
     * PA_6     ADC1_6
     * PA_7     ADC1_7
     */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;    
    GPIO_InitStruct.Pull = GPIO_NOPULL;          
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    __HAL_RCC_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_1;
    
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;    
    GPIO_InitStruct.Pull = GPIO_NOPULL;          
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_2;

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;    
    GPIO_InitStruct.Pull = GPIO_NOPULL;          
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    __HAL_RCC_GPIOF_CLK_ENABLE();
    GPIO_InitStruct.Pin = GPIO_PIN_4;

    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;    
    GPIO_InitStruct.Pull = GPIO_NOPULL;          
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}


void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc) {
    __HAL_RCC_ADC1_CLK_DISABLE();

    // gpio deinits
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_7|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_2);
    HAL_GPIO_DeInit(GPIOF, GPIO_PIN_4);

    HAL_NVIC_DisableIRQ(ADC_IRQn);
}