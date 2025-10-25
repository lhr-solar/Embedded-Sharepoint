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

#ifdef STM32F4xx
#ifndef ADC_PRIO
#define ADC_PRIO 5
#endif
#endif

#ifdef STM32L4xx
#ifndef ADC1_PRIO
#define ADC1_PRIO 5
#endif

#ifndef ADC2_PRIO
#define ADC2_PRIO 5
#endif

#ifndef ADC3_PRIO
#define ADC3_PRIO 5
#endif
#endif

// Hardware ADC error code
uint32_t adc_err_code = 0;

adc_status_t adc_init(const ADC_InitTypeDef* init, ADC_HandleTypeDef* h) {
    // Initalize ADC
    h->Init = *init;
    if (HAL_ADC_Init(h) != HAL_OK) return ADC_INIT_FAIL;

    return ADC_OK;
}

adc_status_t adc_deinit(ADC_HandleTypeDef *h) {
    // Deinit ADC at specific handle
    if (HAL_ADC_DeInit(h) != HAL_OK) return ADC_DEINIT_FAIL;

    return ADC_OK;
} 


adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t *q) {
    ADC_ChannelConfTypeDef sConfig = {
        .Channel = channel,
        .Rank = 1,
        .SamplingTime = samplingTime
    }; 
    
    // Queue Arbitration for later
    #ifdef ADC1
    if (h->Instance == ADC1) adc1_q = q;
    #endif
    #ifdef ADC2
    if (h->Instance == ADC2) adc2_q = q;
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) adc3_q = q;
    #endif
    
    // Check Queue Full
    if (uxQueueSpacesAvailable(*q) == 0) {
        return ADC_QUEUE_FULL;
    }
    // Configure Channel
    if (HAL_ADC_ConfigChannel(h, &sConfig) != HAL_OK) {
        return ADC_CHANNEL_CONFIG_FAIL;
    }
    // Trigger Interrupt
    HAL_StatusTypeDef adc_it_stat = HAL_ADC_Start_IT(h);

    // Handling
    switch (adc_it_stat) {
        case HAL_BUSY:
            return ADC_INTERRUPT_BUSY;
            break;
        case HAL_TIMEOUT:
            return ADC_INTERRUPT_TIMEOUT;
            break;
        case HAL_ERROR:
            return ADC_INTERRUPT_ERROR;
            break;
        default: break;
    }
    
    return ADC_OK; 
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *h) {
    /*
    Interrupt triggers this callback when the conversion is complete
    */ 
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    QueueHandle_t* q = NULL; // Queue will never be null by the call [placeholder]
    int rawVal;

    if (h->Instance == ADC1) q = adc1_q;
    #ifdef ADC2
    if (h->Instance == ADC2) q = adc2_q;
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) q = adc3_q;
    #endif

    rawVal = HAL_ADC_GetValue(h);
    xQueueSendFromISR(*q, &rawVal, &higherPriorityTaskWoken);

    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

__weak void HAL_ADC_MspGPIOInit() {
    // GPIO --- Instanstiate PA3
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef input =  {
        .Pin = GPIO_PIN_3,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(GPIOA, &input);
}

#if defined(STM32L4xx)
static inline void HAL_ADC_MspL4Init(ADC_HandleTypeDef *h) {
    HAL_ADC_MspGPIOInit();

    // L4 Clock
    __HAL_RCC_ADC_CLK_ENABLE();

    // L4 w/ one ADC
    #if !defined(ADC2) && !defined(ADC3)
    HAL_NVIC_SetPriority(ADC1_IRQn, ADC1_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC2_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, ADC3_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);

    #endif
}
#endif

#if defined(STM32F4xx)
static inline void HAL_ADC_MspF4Init(ADC_HandleTypeDef *h) {
    // GPIO Init
    HAL_ADC_MspGPIOInit();

    // F4 Clock
    if (h->Instance == ADC1) __HAL_RCC_ADC1_CLK_ENABLE();

    #ifdef ADC2
    if (h->Instance == ADC2) __HAL_RCC_ADC2_CLK_ENABLE();
    #endif

    #ifdef ADC3
    if (h->Instance == ADC3)__HAL_RCC_ADC3_CLK_ENABLE();
    #endif

    HAL_NVIC_SetPriority(ADC_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
}
#endif

#if defined(STM32L4xx)
static inline void HAL_ADC_MspL4DeInit(ADC_HandleTypeDef *h) {
    // GPIO Init
    

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

#if defined(STM32F4xx)
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

#if defined(STM32L4xx)
#ifdef ADC1_2_IRQHandler
void ADC1_2_IRQHandler() {
    if (ADC_FLAG_EOC & ADC1->ISR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #ifdef ADC2
    else if (ADC_FLAG_EOC & ADC2->ISR) {
        HAL_ADC_IRQHandler(hadc2);
    }
    #endif
}
#endif

#ifdef ADC1_IRQHandler
void ADC1_IRQHandler() {
    // L4 IRQ Handler

    if (ADC_FLAG_EOC & ADC1->ISR) {
        HAL_ADC_IRQHandler(hadc1);
    }
}
#endif

#ifdef ADC3
void ADC3_IRQHandler() {
    if (ADC_FLAG_EOC & ADC3->ISR) {
        HAL_ADC_IRQHandler(hadc3);
    }
}
#endif
#endif

#if defined(STM32F4xx)
void ADC_IRQHandler() {
    // F4 IRQ Handler 

    if (ADC_FLAG_EOC & ADC1->SR) {
        HAL_ADC_IRQHandler(hadc1);
    }
    #ifdef ADC2
    else if (ADC_FLAG_EOC & ADC2->SR) {
        HAL_ADC_IRQHandler(hadc2);
    }
    #endif
    #ifdef ADC3
    if (ADC_FLAG_EOC & ADC3->SR) {
        HAL_ADC_IRQHandler(hadc3);
    }
    #endif
}
#endif

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *h) {
    adc_err_code = HAL_ADC_GetError(h);
    
    // retry 
    HAL_ADC_Stop_IT(h);
    HAL_ADC_Start_IT(h);
}