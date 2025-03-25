#include "ADC.h"

static ADC_HandleTypeDef hadc_ = {.Instance = ADC1};
ADC_HandleTypeDef* hadc = &hadc_;

static QueueHandle_t* adcReadings;

volatile bool read_failed = 0;

void hi() {
    int a = 2;
    a += 0;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *h) {
    /*
    Interrupt triggers this callback when the conversion is complete
    */
   BaseType_t higherPriorityTaskWoken = pdFALSE;

    if (h == hadc) {
        int rawVal = HAL_ADC_GetValue(hadc);

        // push value to q
        xQueueSendFromISR(*adcReadings, &rawVal, &higherPriorityTaskWoken);

    } else {
        read_failed = 1; 
    }
}

adc_status_t ADC_Init(ADC_InitTypeDef init, QueueHandle_t* rxQueue) {
    adcReadings = rxQueue;

    hadc->Init = init; // set the init structure to InitTypeDef
    if (HAL_ADC_Init(hadc) != HAL_OK) return ADC_INIT_FAIL;
    
    return ADC_OK;
}


adc_status_t ADC_OneShotRead(uint32_t channel, uint32_t samplingTime, bool blocking) {
    if (xQueueIsQueueFullFromISR(*adcReadings)) {
        return ADC_QUEUE_FULL;
    }

    ADC_ChannelConfTypeDef sConfig = {0}; // resets the channel config

    // configure only one channel
    sConfig.Channel = channel;
    sConfig.Rank = 1;
    sConfig.SamplingTime = samplingTime;

    if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK) {
        return ADC_CHANNEL_CONFIG_FAIL;
    }

    // trigger interrupt to read 
    HAL_ADC_Start_IT(hadc);   

    return ADC_OK; // not sure if i should actually send an "OK" interrupt b/c the callback is still pending
}

void ADC_IRQHandler() {
    HAL_ADC_IRQHandler(hadc);
}