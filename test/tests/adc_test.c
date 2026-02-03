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

    volatile uint8_t ISR = hadc1->Instance->ISR;(void) ISR;

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
    
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef input =  {
        .Pin = GPIO_PIN_0,
        .Mode = GPIO_MODE_ANALOG,
        .Pull = GPIO_NOPULL,
    };

    HAL_GPIO_Init(GPIOA, &input);

    xReadings = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, qStorage, &xStaticQueue);

    // init ADC
    ADC_InitTypeDef adc_init_1 = {0};

    adc_init_1.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init_1.Resolution = ADC_RESOLUTION_12B;
    adc_init_1.ScanConvMode = DISABLE;
    adc_init_1.ContinuousConvMode = DISABLE;
    adc_init_1.DiscontinuousConvMode = DISABLE;
    adc_init_1.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init_1.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init_1.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init_1.NbrOfConversion = 1;
    adc_init_1.DMAContinuousRequests = DISABLE;
    adc_init_1.EOCSelection = ADC_EOC_SINGLE_CONV;

    volatile adc_status_t s = adc_init(&adc_init_1, hadc1);
    s+=0;
    if (s != ADC_OK) error_handler(ADC_INIT_FAIL);
    HAL_ADCEx_Calibration_Start(hadc1, ADC_SINGLE_ENDED);

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

    // Task Creation

    xTaskCreateStatic(TestADC1,
                    "ADC Test",
                    configMINIMAL_STACK_SIZE,
                    (void*) 1,
                    tskIDLE_PRIORITY+4,
                    xStack,
                    &xTaskBuffer);

    vTaskStartScheduler();
    
    // should never reach

    return 0;
}