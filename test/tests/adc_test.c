/**
 * @file adc_test.c
 * @brief ADC test file for verifying ADC read behavior with FreeRTOS queues.
 * 
 * ---------------------
 * CONFIGURATION NOTE:
 * ---------------------
 * To change the GPIO pin used for a given ADC channel, override the function:
 *
 *      void HAL_ADC_MspGPIOInit(ADC_HandleTypeDef* hadc)
 *
 * This function is weakly defined in `ADC.c`. You can redefine it in this file 
 * or another user file to change pin configurations (e.g., port, pin, alternate function).
 *
 * Refer to the `ADC.c` file for the default implementation.
 */

#include "ADC.h"

#define STACK_SIZE 200

StaticTask_t xTaskBuffer;
StackType_t xStack[ STACK_SIZE ];

#define QUEUE_LENGTH    10
#define ITEM_SIZE       sizeof( uint32_t )

uint8_t qStorage[QUEUE_LENGTH * ITEM_SIZE];
static StaticQueue_t xStaticQueue;

QueueHandle_t xReadings;

static void error_handler(adc_status_t err) {
    while(1) {
        // set bkpt here
    }
}
  
static void success_handler(void) {
    // blinky

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
     // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &led_config); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(500);
    }
  }

#ifdef ADC_SAMPLETIME_3CYCLES
void TestQueueFull(void *pvParameters) {
    // Set bkpt in error_handler();
    uint32_t reading = 0;

    // read once
    for (int i = 0; i < 11; i++) {
        adc_status_t stat = adc_read(ADC_CHANNEL_0,  ADC_SAMPLETIME_3CYCLES, hadc1, xReadings);
        
        if (stat != ADC_OK) {
            error_handler(stat);
        }
    }

    // should not reach here
    xQueueReceive(xReadings, &reading, 0);
    success_handler();
}
#endif


void TestADC1(void *pvParameters) {
    // Set bkpt in error_handler();
    uint32_t reading = 0;

    // read once
    for (int i = 0; i < 10; i++) {
        #ifdef ADC_SAMPLETIME_3CYCLES
        adc_status_t stat = adc_read(ADC_CHANNEL_1,  ADC_SAMPLETIME_3CYCLES, hadc1, xReadings);
        #else
        adc_status_t stat = adc_read(ADC_CHANNEL_1,  ADC_SAMPLETIME_2CYCLES_5, hadc1, xReadings);
        #endif
        
        if (stat != ADC_OK) {
            error_handler(stat);
        }
    }

    for (int i = 0; i < 10; i++) {
        xQueueReceive(xReadings, &reading, 0);
    }
    
    success_handler();
}

#ifdef ADC2
void TestADC2(void *pvParameters) {
    // Set bkpt in error_handler();
    uint32_t reading = 0;

    // read once
    for (int i = 0; i < 10; i++) {
        #ifdef ADC_SAMPLETIME_3CYCLES
        adc_status_t stat = adc_read(ADC_CHANNEL_0,  ADC_SAMPLETIME_3CYCLES, hadc2, xReadings);
        #else
        adc_status_t stat = adc_read(ADC_CHANNEL_0,  ADC_SAMPLETIME_2CYCLES_5, hadc2, xReadings);
        #endif
        
        if (stat != ADC_OK) {
            error_handler(stat);
        }
    }
    
    for (int i = 0; i < 10; i++) {
        xQueueReceive(xReadings, &reading, 0);
    }
    
    success_handler();
}
#endif

#ifdef ADC3
void TestADC3(void *pvParameters) {
    // Set bkpt in error_handler();
    uint32_t reading = 0;

    // read once
    for (int i = 0; i < 10; i++) {
        #ifdef ADC_SAMPLETIME_3CYCLES
        adc_status_t stat = adc_read(ADC_CHANNEL_0,  ADC_SAMPLETIME_3CYCLES, hadc3, xReadings);
        #else
        adc_status_t stat = adc_read(ADC_CHANNEL_0,  ADC_SAMPLETIME_2CYCLES_5, hadc3, xReadings);
        #endif
        
        if (stat != ADC_OK) {
            error_handler(stat);
        }
    }

    for (int i = 0; i < 10; i++) {
        xQueueReceive(xReadings, &reading, 0);
    }
    
    success_handler();
}
#endif

int main() {
    // GPIO Init
    HAL_Init();
    SystemClock_Config();
    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef input =  {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(GPIOA, &input);

    xReadings = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, qStorage, &xStaticQueue);

    HAL_ADC_MspInit(hadc1);

    // init ADC
    ADC_InitTypeDef adc_init_1 = {0};

    // Testing F4 Init
    adc_init_1.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init_1.Resolution = ADC_RESOLUTION_12B;
    adc_init_1.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init_1.EOCSelection = ADC_EOC_SINGLE_CONV;
    adc_init_1.ContinuousConvMode = DISABLE;
    adc_init_1.NbrOfConversion = 1;
    adc_init_1.DiscontinuousConvMode = DISABLE;
    adc_init_1.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init_1.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init_1.DMAContinuousRequests = DISABLE;

    // Testing G4 Init
    // adc_init_1.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    // adc_init_1.Resolution = ADC_RESOLUTION_12B;
    // adc_init_1.DataAlign = ADC_DATAALIGN_RIGHT;
    // adc_init_1.ScanConvMode = ADC_SCAN_DISABLE;
    // adc_init_1.EOCSelection = ADC_EOC_SINGLE_CONV;
    // adc_init_1.LowPowerAutoWait = DISABLE;
    // adc_init_1.ContinuousConvMode = DISABLE;
    // adc_init_1.NbrOfConversion = 1;
    // adc_init_1.DiscontinuousConvMode = DISABLE;
    // adc_init_1.ExternalTrigConv = ADC_SOFTWARE_START;
    // adc_init_1.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    // adc_init_1.DMAContinuousRequests = DISABLE;
    // adc_init_1.Overrun = ADC_OVR_DATA_PRESERVED;
    // adc_init_1.OversamplingMode = DISABLE;

    volatile adc_status_t s = adc_init(&adc_init_1, hadc1);
    s+=0;
    if (s != ADC_OK) error_handler(ADC_INIT_FAIL);

    #if !defined(STM32L4xx)
    ADC_MultiModeTypeDef multimode = {0};
    multimode.Mode = ADC_MODE_INDEPENDENT;
    if (HAL_ADCEx_MultiModeConfigChannel(hadc1, &multimode) != HAL_OK)
    {
    Error_Handler();
    }
    #endif

    
    #ifdef ADC2
    ADC_InitTypeDef adc_init_2 = {0};

    adc_init_2.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init_2.Resolution = ADC_RESOLUTION_12B;
    adc_init_2.ScanConvMode = DISABLE;
    adc_init_2.ContinuousConvMode = DISABLE;
    adc_init_2.DiscontinuousConvMode = DISABLE;
    adc_init_2.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init_2.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init_2.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init_2.NbrOfConversion = 1;
    adc_init_2.DMAContinuousRequests = DISABLE;
    adc_init_2.EOCSelection = ADC_EOC_SINGLE_CONV;

    if (adc_init(&adc_init_2, hadc2) != ADC_OK) error_handler(ADC_INIT_FAIL);
    #endif
    #ifdef ADC3
    ADC_InitTypeDef adc_init_3 = {0};

    adc_init_3.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init_3.Resolution = ADC_RESOLUTION_12B;
    adc_init_3.ScanConvMode = DISABLE;
    adc_init_3.ContinuousConvMode = DISABLE;
    adc_init_3.DiscontinuousConvMode = DISABLE;
    adc_init_3.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init_3.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init_3.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init_3.NbrOfConversion = 1;
    adc_init_3.DMAContinuousRequests = DISABLE;
    adc_init_3.EOCSelection = ADC_EOC_SINGLE_CONV;

    if (adc_init(&adc_init_3, hadc3) != ADC_OK) error_handler(ADC_INIT_FAIL);
    #endif

    xTaskCreateStatic(TestADC1,
                    "ADC Test",
                    configMINIMAL_STACK_SIZE,
                    (void*) 1,
                    tskIDLE_PRIORITY+4,
                    xStack,
                    &xTaskBuffer);

    vTaskStartScheduler();
    
    // should never reach
    HAL_ADC_MspDeInit(hadc1);

    return 0;
}

#define ADC_PRIO 5 /** can't make this 0 because FreeRTOS priorities */

#if defined(STM32G4xx)
static inline void HAL_ADC_MspG4Init(ADC_HandleTypeDef *h) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
    
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if (h->Instance == ADC1 || h->Instance == ADC2) { 
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC12;
        PeriphClkInit.Adc12ClockSelection = RCC_ADC12CLKSOURCE_SYSCLK;
        __HAL_RCC_ADC12_CLK_ENABLE(); 
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) Error_Handler();
    }   
    else if (h->Instance == ADC3 || h->Instance == ADC4 || h->Instance == ADC5) {
        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC345;
        PeriphClkInit.Adc345ClockSelection = RCC_ADC345CLKSOURCE_SYSCLK;
        __HAL_RCC_ADC345_CLK_ENABLE(); 
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) Error_Handler();
    }

    #if defined(ADC1) || defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
    #endif

    #if defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);
    #endif

    #if defined(ADC4)
    HAL_NVIC_SetPriority(ADC4_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC4_IRQn);
    #endif

    #if defined(ADC5)
    HAL_NVIC_SetPriority(ADC5_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC5_IRQn);
    #endif
}
#endif

#if defined(STM32L4xx)
static inline void HAL_ADC_MspL4Init(ADC_HandleTypeDef *h) {

    // L4 Clock
    __HAL_RCC_ADC_CLK_ENABLE();

    // L4 w/ one ADC
    #if !defined(ADC2) && !defined(ADC3)
    HAL_NVIC_SetPriority(ADC1_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC2)
    HAL_NVIC_SetPriority(ADC1_2_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC1_2_IRQn);

    // L4 w/ more than one ADC
    #elif defined(ADC3)
    HAL_NVIC_SetPriority(ADC3_IRQn, ADC_PRIO, 0);
    HAL_NVIC_EnableIRQ(ADC3_IRQn);

    #endif
}
#endif

#if defined(STM32F4xx)
static inline void HAL_ADC_MspF4Init(ADC_HandleTypeDef *h) {
    // GPIO Init

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

#if defined(STM32G4xx)
static inline void HAL_ADC_MspG4DeInit(ADC_HandleTypeDef *h) {
    if (h->Instance == ADC1 || h->Instance == ADC2) { 
        __HAL_RCC_ADC12_CLK_DISABLE(); 
    }   
    if (h->Instance == ADC3 || h->Instance == ADC4 || h->Instance == ADC5) {
        __HAL_RCC_ADC345_CLK_DISABLE(); 
    }

    #if defined(ADC1) || defined(ADC2)
    HAL_NVIC_DisableIRQ(ADC1_2_IRQn);
    #endif

    #if defined(ADC3)
    HAL_NVIC_DisableIRQ(ADC3_IRQn);
    #endif

    #if defined(ADC4)
    HAL_NVIC_DisableIRQ(ADC4_IRQn);
    #endif

    #if defined(ADC5)
    HAL_NVIC_DisableIRQ(ADC5_IRQn);
    #endif
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
    // G4
    #ifdef STM32G4xx
    HAL_ADC_MspG4Init(h);
    #endif
    
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
    // G4
    #ifdef STM32G4xx
    HAL_ADC_MspG4DeInit(h);
    #endif

    // L4
    #ifdef STM32L4xx
    HAL_ADC_MspL4DeInit(h);
    #endif

    // F4
    #ifdef STM32F4xx
    HAL_ADC_MspF4DeInit(h);
    #endif
}