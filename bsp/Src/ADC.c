#include "ADC.h"

// Define Queue Handles
#ifdef ADC1
static ADC_HandleTypeDef hadc1_ = {.Instance = ADC1};
ADC_HandleTypeDef* hadc1 = &hadc1_;
QueueHandle_t adc1_q;
#endif

#ifdef ADC2
static ADC_HandleTypeDef hadc2_ = {.Instance = ADC2};
ADC_HandleTypeDef* hadc2 = &hadc2_;
QueueHandle_t adc2_q;
#endif

#ifdef ADC3
static ADC_HandleTypeDef hadc3_ = {.Instance = ADC3};
ADC_HandleTypeDef* hadc3 = &hadc3_;
QueueHandle_t adc3_q;
#endif

#ifdef ADC4
static ADC_HandleTypeDef hadc4_ = {.Instance = ADC4};
ADC_HandleTypeDef* hadc4 = &hadc4_;
QueueHandle_t adc4_q;
#endif

#ifdef ADC5
static ADC_HandleTypeDef hadc5_ = {.Instance = ADC5};
ADC_HandleTypeDef* hadc5 = &hadc5_;
QueueHandle_t adc5_q;
#endif

// Hardware ADC error code
uint32_t adc_err_code = 0;

adc_status_t adc_init(ADC_InitTypeDef* init, ADC_HandleTypeDef* h) {
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


adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q) {
    ADC_ChannelConfTypeDef sConfig = {};
    sConfig.Channel = channel;
    #if defined(STM32F4xx)
    sConfig.Rank = 1; 
    #endif
    #if defined(STM32G4xx) || defined(STM32L4xx)
    sConfig.Rank = ADC_REGULAR_RANK_1;
    #endif
    sConfig.SamplingTime = samplingTime;

    if (HAL_ADC_ConfigChannel(hadc1, &sConfig) != HAL_OK)
    {
      Error_Handler();
    } 
    
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
    #ifdef ADC4
    if (h->Instance == ADC4) adc4_q = q;
    #endif
    #ifdef ADC5
    if (h->Instance == ADC5) adc5_q = q;
    #endif
    
    // Check Queue Full
    if (uxQueueSpacesAvailable(q) == 0) {
        return ADC_QUEUE_FULL;
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

__weak adc_status_t adc_hook(ADC_HandleTypeDef *h) {
  /*** Callback hook if you're feeling creative ***/
  return ADC_OK;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *h) {
    /*
    Interrupt triggers this callback when the conversion is complete
    */ 
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    QueueHandle_t q = NULL; // Queue will never be null by the call [placeholder]
    int rawVal;

    if (h->Instance == ADC1) q = adc1_q;
    #ifdef ADC2
    if (h->Instance == ADC2) q = adc2_q;
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) q = adc3_q;
    #endif
    #ifdef ADC4
    if (h->Instance == ADC4) q = adc4_q;
    #endif
    #ifdef ADC5
    if (h->Instance == ADC5) q = adc5_q;
    #endif

    rawVal = HAL_ADC_GetValue(h); // reads DR reg
    
    xQueueSendFromISR(q, &rawVal, &higherPriorityTaskWoken);

    // isr done
    adc_hook(h); // lakman
    portYIELD_FROM_ISR(higherPriorityTaskWoken);
}



#if defined(STM32L4xx) || defined(STM32G4xx)
void ADC1_2_IRQHandler() {
    HAL_ADC_IRQHandler(hadc1);
    #ifdef ADC2
        HAL_ADC_IRQHandler(hadc2);

    #endif
}

#ifdef ADC3
void ADC3_IRQHandler() {
    HAL_ADC_IRQHandler(hadc3);

}
#endif
#endif

#if defined(STM32G4xx)
#ifdef ADC4_IRQHandler
void ADC4_IRQHandler() {
    HAL_ADC_IRQHandler(hadc4);
}
#endif

#ifdef ADC5_IRQHandler
void ADC5_IRQHandler() {
    HAL_ADC_IRQHandler(hadc5);
}
#endif
#endif

#if defined(STM32F4xx)
void ADC_IRQHandler() {
    // w simplicity
    // F4 IRQ Handler 
    HAL_ADC_IRQHandler(hadc1);
    #ifdef ADC2
    HAL_ADC_IRQHandler(hadc2);
    #endif
    #ifdef ADC3
    HAL_ADC_IRQHandler(hadc3);
    #endif
}
#endif

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *h) {
    adc_err_code = h->ErrorCode;
    
    // todo 
}