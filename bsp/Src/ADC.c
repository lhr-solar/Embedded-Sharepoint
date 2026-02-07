#include "ADC.h"

// Define Queue Handles
#ifdef ADC1
static ADC_HandleTypeDef hadc1_ = {.Instance = ADC1};
ADC_HandleTypeDef* hadc1 = &hadc1_;
QueueHandle_t adc1_q;
SemaphoreHandle_t adc1_semphr;
StaticSemaphore_t adc1_semphr_buff;
#endif

#ifdef ADC2
static ADC_HandleTypeDef hadc2_ = {.Instance = ADC2};
ADC_HandleTypeDef* hadc2 = &hadc2_;
QueueHandle_t adc2_q;
SemaphoreHandle_t adc2_semphr;
StaticSemaphore_t adc2_semphr_buff;
#endif

#ifdef ADC3
static ADC_HandleTypeDef hadc3_ = {.Instance = ADC3};
ADC_HandleTypeDef* hadc3 = &hadc3_;
QueueHandle_t adc3_q;
SemaphoreHandle_t adc3_semphr;
StaticSemaphore_t adc3_semphr_buff;
#endif

#ifdef ADC4
static ADC_HandleTypeDef hadc4_ = {.Instance = ADC4};
ADC_HandleTypeDef* hadc4 = &hadc4_;
QueueHandle_t adc4_q;
SemaphoreHandle_t adc4_semphr;
StaticSemaphore_t adc4_semphr_buff;
#endif

#ifdef ADC5
static ADC_HandleTypeDef hadc5_ = {.Instance = ADC5};
ADC_HandleTypeDef* hadc5 = &hadc5_;
QueueHandle_t adc5_q;
SemaphoreHandle_t adc5_semphr;
StaticSemaphore_t adc5_semphr_buff;
#endif

// Hardware ADC error code
uint32_t adc_err_code = 0;

adc_status_t adc_init(ADC_InitTypeDef* init, ADC_HandleTypeDef* h) {
    // Initalize ADC
    h->Init = *init;
    if (HAL_ADC_Init(h) != HAL_OK) return ADC_INIT_FAIL;

    // Instanstiate Semaphores for each ADC
    #ifdef ADC1
    adc1_semphr=xSemaphoreCreateBinaryStatic( &adc1_semphr_buff );
    xSemaphoreGive( adc1_semphr );
    #endif
    #ifdef ADC2
    adc2_semphr=xSemaphoreCreateBinaryStatic( &adc2_semphr_buff );
    xSemaphoreGive( adc2_semphr );
    #endif
    #ifdef ADC3
    adc3_semphr=xSemaphoreCreateBinaryStatic( &adc3_semphr_buff );
    xSemaphoreGive( adc3_semphr );
    #endif
    #ifdef ADC4
    adc4_semphr=xSemaphoreCreateBinaryStatic( &adc4_semphr_buff );
    xSemaphoreGive( adc4_semphr );
    #endif
    #ifdef ADC5
    adc5_semphr=xSemaphoreCreateBinaryStatic( &adc5_semphr_buff );
    xSemaphoreGive( adc5_semphr );
    #endif

    return ADC_OK;
}

adc_status_t adc_deinit(ADC_HandleTypeDef *h) {
    // Deinit ADC at specific handle
    if (HAL_ADC_DeInit(h) != HAL_OK) return ADC_DEINIT_FAIL;

    return ADC_OK;
} 


adc_status_t adc_read(uint32_t channel, uint32_t samplingTime, ADC_HandleTypeDef *h, QueueHandle_t q) {
    // Semaphore to content for ADC configuration and read
    #ifdef ADC1
    if (h->Instance == ADC1) xSemaphoreTake( adc1_semphr , portMAX_DELAY );
    #endif
    #ifdef ADC2
    if (h->Instance == ADC2) xSemaphoreTake( adc2_semphr , portMAX_DELAY );
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) xSemaphoreTake( adc3_semphr , portMAX_DELAY );
    #endif
    #ifdef ADC4
    if (h->Instance == ADC4) xSemaphoreTake( adc4_semphr , portMAX_DELAY );
    #endif
    #ifdef ADC5
    if (h->Instance == ADC5) xSemaphoreTake( adc5_semphr , portMAX_DELAY );
    #endif

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

    // debugging :)
    // volatile uint8_t isr = h->Instance->ISR;(void)isr;
    
    // Trigger Interrupt
    HAL_StatusTypeDef adc_it_stat = HAL_ADC_Start_IT(h);

    // isr=h->Instance->ISR;

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

    // grabbed reading so gib
    if (h->Instance == ADC1) xSemaphoreGiveFromISR( adc1_semphr, &higherPriorityTaskWoken );
    #ifdef ADC2
    if (h->Instance == ADC2) xSemaphoreGiveFromISR( adc2_semphr, &higherPriorityTaskWoken );
    #endif
    #ifdef ADC3
    if (h->Instance == ADC3) xSemaphoreGiveFromISR( adc3_semphr, &higherPriorityTaskWoken );
    #endif
    #ifdef ADC4
    if (h->Instance == ADC4) xSemaphoreGiveFromISR( adc4_semphr, &higherPriorityTaskWoken );
    #endif
    #ifdef ADC5
    if (h->Instance == ADC5) xSemaphoreGiveFromISR( adc5_semphr, &higherPriorityTaskWoken );
    #endif

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