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
uint32_t adc_err_code = 0;

adc_status_t ADC_Init(ADC_InitTypeDef init, ADC_HandleTypeDef* h) {
    // Initalize ADC
    if (h->Instance == ADC1) {
        hadc1->Init = init;
        if (HAL_ADC_Init(hadc1) != HAL_OK) return ADC_INIT_FAIL;
    }
    #ifdef ADC2
    if (h->Instance == ADC2) {
        hadc2->Init = init;
        if (HAL_ADC_Init(hadc2) != HAL_OK) return ADC_INIT_FAIL;
    }
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) {
        hadc3->Init = init;
        if (HAL_ADC_Init(hadc3) != HAL_OK) return ADC_INIT_FAIL;
    }
    #endif

    return ADC_OK;
}

adc_status_t ADC_DeInit(ADC_HandleTypeDef *h) {
    // Deinit ADC at specific handle
    if (HAL_ADC_DeInit(h) != HAL_OK) {
        return ADC_DEINIT_FAIL;
    }

    return ADC_OK;
} 


adc_status_t ADC_Read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t *q) {
    ADC_ChannelConfTypeDef sConfig = {
        .Channel = channel,
        .Rank = 1,
        .SamplingTime = samplingTime
    }; 
    
    #ifdef ADC1
    if (h == hadc1) {
        adc1_q = q;
        if (uxQueueSpacesAvailable(*adc1_q) == 0) {
            return ADC_QUEUE_FULL;
        }
        if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK) {
            return ADC_CHANNEL_CONFIG_FAIL;
        }

        HAL_StatusTypeDef adc1_it_stat = HAL_ADC_Start_IT(hadc1);
        if (adc1_it_stat == HAL_BUSY) {
            return ADC_INTERRUPT_BUSY;
        }
        if (adc1_it_stat == HAL_TIMEOUT) {
            return ADC_INTERRUPT_TIMEOUT;
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

        HAL_StatusTypeDef adc2_it_stat = HAL_ADC_Start_IT(hadc2);
        if (adc2_it_stat == HAL_BUSY) {
            return ADC_INTERRUPT_BUSY;
        }
        if (adc2_it_stat == HAL_TIMEOUT) {
            return ADC_INTERRUPT_TIMEOUT;
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

        HAL_StatusTypeDef adc3_it_stat = HAL_ADC_Start_IT(hadc3);
        if (adc3_it_stat == HAL_BUSY) {
            return ADC_INTERRUPT_BUSY;
        }
        if (adc3_it_stat == HAL_TIMEOUT) {
            return ADC_INTERRUPT_TIMEOUT;
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

#ifdef STM32L4xx
static inline void HAL_ADC_MspL4Init(ADC_HandleTypeDef *h) {
    // L4 Clock
    __HAL_RCC_ADC_CLK_ENABLE();

    // L4 w/ one ADC
    #if !defined(ADC2) && !defined(ADC3)
    HAL_NVIC_SetPriority(ADC1_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);

    #endif
}
#endif

#ifdef STM32F4xx
static inline void HAL_ADC_MspF4Init(ADC_HandleTypeDef *h) {
    // F4 Clock
    if (h->Instance == ADC1) __HAL_RCC_ADC1_CLK_ENABLE();

    #ifdef ADC2
    if (h->Instance == ADC2) __HAL_RCC_ADC2_CLK_ENABLE();
    #endif

    #ifdef ADC3
    if (h->Instance == ADC3)__HAL_RCC_ADC3_CLK_ENABLE();
    #endif

    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}
#endif

#ifdef STM32L4xx
static inline void HAL_ADC_MspL4DeInit(ADC_HandleTypeDef *h) {
    // L4 Clock
    __HAL_RCC_ADC_CLK_DISABLE();

    // L4 w/ one ADC
    #if !defined(ADC2) && !defined(ADC3)
    HAL_NVIC_DisableIRQ(ADC1_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC2)
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC3)
    HAL_NVIC_DisableIRQ(ADC3_IRQn);

    #endif
}
#endif

#ifdef STM32F4xx
static inline void HAL_ADC_MspF4DeInit(ADC_HandleTypeDef *h) {
    if (h->Instance == ADC1) __HAL_RCC_ADC1_CLK_DISABLE();

    #ifdef ADC2
    if (h->Instance == ADC2) __HAL_RCC_ADC2_CLK_DISABLE();
    #endif

    #ifdef ADC3
    if (h->Instance == ADC3)__HAL_RCC_ADC3_CLK_DISABLE();
    #endif

    HAL_NVIC_DisableIRQ(ADC_IRQn);
}
#endif

void HAL_ADC_MspInit(ADC_HandleTypeDef *h) {
    // L4
    #ifdef STM32L4xx
    HAL_ADC_MspL4Init(h);
    #endif

    // F4
    #ifdef STM32F4xx
    HAL_ADC_MspF4Init(h);
    #endif
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *h) {
    // L4
    #ifdef STM32L4xx
    HAL_ADC_MspL4DeInit(h);
    #endif

    // F4
    #ifdef STM32F4xx
    HAL_ADC_MspF4DeInit(h);
    #endif
}

void ADC_IRQHandler() {
    #if defined(STM32L4xx) 
    if (ADC_FLAG_EOC & ADC1->ISR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #else
    if (ADC_FLAG_EOC & ADC1->SR) {
        HAL_ADC_IRQHandler(hadc1);
    }
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
    adc_err_code = HAL_ADC_GetError(h);
    
    // retry 
    HAL_ADC_Stop_IT(h);
    HAL_ADC_Start_IT(h);
}