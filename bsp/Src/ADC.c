#include "ADC.h"

// Define Queue Handles
#ifdef ADC1
static ADC_HandleTypeDef hadc1_ = {.Instance = ADC1};
ADC_HandleTypeDef* hadc1 = &hadc1_;
QueueHandle_t* adc1_q;
#endif

#ifdef ADC2
static ADC_HandleTypeDef hadc2_ = {.Instance = ADC2};
ADC_HandleTypeDef* hadc2 = &hadc2_;
QueueHandle_t* adc2_q;
#endif

#ifdef ADC3
static ADC_HandleTypeDef hadc3_ = {.Instance = ADC3};
ADC_HandleTypeDef* hadc3 = &hadc3_;
QueueHandle_t* adc3_q;
#endif

// Hardware ADC error code
uint32_t ADC_ERR_CODE = 0;

adc_status_t ADC_Init(ADC_InitTypeDef init, ADC_HandleTypeDef* hadc) {
    // Power ADC clocks and set configs
    #ifdef ADC3
    __HAL_RCC_ADC1_CLK_ENABLE();
    hadc1->Init = init;
    if (HAL_ADC_Init(hadc1) != HAL_OK) return ADC_INIT_FAIL;
    #endif
    #ifdef ADC2
    __HAL_RCC_ADC2_CLK_ENABLE();
    hadc2->Init = init;
    if (HAL_ADC_Init(hadc2) != HAL_OK) return ADC_INIT_FAIL;
    #endif
    #ifdef ADC3
    __HAL_RCC_ADC3_CLK_ENABLE();
    hadc3->Init = init;
    if (HAL_ADC_Init(hadc3) != HAL_OK) return ADC_INIT_FAIL;
    #endif

    // Configure NVIC
    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    
    return ADC_OK;
}


adc_status_t ADC_OneShotRead(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t *q) {
    
    // Reset channel config for only one channel
    ADC_ChannelConfTypeDef sConfig = {
        .Channel = channel,
        .Rank = 1,
        .SamplingTime = samplingTime
    }; 
    
    // --- Config channel
    // --- Start ADC Interrupt
    #ifdef ADC1
    if (h == hadc1) {
        adc1_q = q;
        if (uxQueueSpacesAvailable(*adc1_q) == 0) {
            return ADC_QUEUE_FULL;
        }
        if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK) {
            return ADC_CHANNEL_CONFIG_FAIL;
        }

        if (HAL_ADC_Start_IT(hadc1) != HAL_OK) {
            return ADC_INTERRUPT_FAIL;
        }
    }
    #endif
    #ifdef ADC2
    if (h == hadc2) {
        adc2_q = q;
        if (uxQueueSpacesAvailable(*adc2_q) == 0) {
            return ADC_QUEUE_FULL;
        }
        if (HAL_ADC_ConfigChannel(hadc2, &sConfig) != HAL_OK) {
            return ADC_CHANNEL_CONFIG_FAIL;
        }

        if (HAL_ADC_Start_IT(hadc2) != HAL_OK) {
            return ADC_INTERRUPT_FAIL;
        }
    }
    #endif
    #ifdef ADC3
    if (h == hadc3) {
        adc3_q = q;
        if (uxQueueSpacesAvailable(*adc3_q) == 0) {
            return ADC_QUEUE_FULL;
        }
        if (HAL_ADC_ConfigChannel(hadc3, &sConfig) != HAL_OK) {
            return ADC_CHANNEL_CONFIG_FAIL;
        }

        if (HAL_ADC_Start_IT(hadc3) != HAL_OK) {
            return ADC_INTERRUPT_FAIL;
        }
    }
    #endif
    
    return ADC_OK; 
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *h) {
    /*
    Interrupt triggers this callback when the conversion is complete
    */ 
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    int rawVal;

    #ifdef ADC1
    if (h == hadc1) {
        rawVal = HAL_ADC_GetValue(hadc1);
        xQueueSendFromISR(*adc1_q, &rawVal, &higherPriorityTaskWoken);

    } 
    #endif
    #ifdef ADC2
    if (h == hadc2) {
        rawVal = HAL_ADC_GetValue(hadc2);
        xQueueSendFromISR(*adc2_q, &rawVal, &higherPriorityTaskWoken);

    } 
    #endif
    #ifdef ADC3
    if (h == hadc3) {
        rawVal = HAL_ADC_GetValue(hadc3);
        xQueueSendFromISR(*adc3_q, &rawVal, &higherPriorityTaskWoken);
        
    }
    #endif
}

void ADC_IRQHandler() {
    // Arbitrate between ADCs
    #ifdef ADC1
    if (ADC_FLAG_EOC & ADC1->SR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #endif

    #ifdef ADC2
    if (ADC_FLAG_EOC & ADC2->SR) {
        HAL_ADC_IRQHandler(hadc2);
    }
    #endif

    #ifdef ADC3
    if (ADC_FLAG_EOC & ADC3->SR) {
        HAL_ADC_IRQHandler(hadc3);
    }
    #endif
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *h) {
    // in case of error
    ADC_ERR_CODE = h->ErrorCode;

    // try again
    HAL_ADC_Stop_IT(h);
    HAL_ADC_Start_IT(h);
}