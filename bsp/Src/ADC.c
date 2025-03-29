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

adc_status_t ADC_Init(ADC_InitTypeDef init, ADC_HandleTypeDef* h) {
    // Power ADC clocks and set configs
    // L4 
    #ifdef STM32L4xx
    __HAL_RCC_ADC_CLK_ENABLE();
    #endif

    #ifdef ADC1
    if (h->Instance == ADC1) {
        #ifdef STM32F4xx
        __HAL_RCC_ADC1_CLK_ENABLE();
        #endif
        hadc1->Init = init;
        if (HAL_ADC_Init(hadc1) != HAL_OK) return ADC_INIT_FAIL;
    }
    #endif
    #ifdef ADC2
    if (h->Instance == ADC2) {
        #ifdef STM32F4xx
        __HAL_RCC_ADC2_CLK_ENABLE();
        #endif
        hadc2->Init = init;
        if (HAL_ADC_Init(hadc2) != HAL_OK) return ADC_INIT_FAIL;
    }
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) {
        #ifdef STM32F4xx
        __HAL_RCC_ADC3_CLK_ENABLE();
        #endif
        hadc3->Init = init;
        if (HAL_ADC_Init(hadc3) != HAL_OK) return ADC_INIT_FAIL;
    }
    #endif

    // Configure NVIC
    // if l4 with only 1 adc
    #if defined(STM32L4xx) && (!defined(ADC2) && !defined(ADC3)) 
    HAL_NVIC_SetPriority(ADC1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);
    #elif defined(STM32L4xx) && defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    #elif defined(STM32L4xx) && defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);
    #else
    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
    #endif
    
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

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void ADC_IRQHandler() {
    // Arbitrate between ADCs
    #ifdef ADC1
    #if defined(STM32L4xx) 
    if (ADC_FLAG_EOC & ADC1->ISR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #else
    if (ADC_FLAG_EOC & ADC1->SR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #endif
    #endif

    #ifdef ADC2
    #if defined(STM32L4xx) 
    if (ADC_FLAG_EOC & ADC2->ISR) {
        HAL_ADC_IRQHandler(hadc2);
    }
    #else
    if (ADC_FLAG_EOC & ADC2->SR) {
        HAL_ADC_IRQHandler(hadc2);
    }
    #endif
    #endif

    #ifdef ADC3
    #if defined(STM32L4xx) 
    if (ADC_FLAG_EOC & ADC3->ISR) {
        HAL_ADC_IRQHandler(hadc3);
    }
    #else
    if (ADC_FLAG_EOC & ADC3->SR) {
        HAL_ADC_IRQHandler(hadc3);
    }
    #endif
    #endif
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *h) {
    // in case of error
    ADC_ERR_CODE = h->ErrorCode;

    // try again
    HAL_ADC_Stop_IT(h);
    HAL_ADC_Start_IT(h);
}