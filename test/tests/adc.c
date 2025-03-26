#include "ADC.h"

#define STACK_SIZE 200

StaticTask_t xTaskBuffer;
StackType_t xStack[ STACK_SIZE ];

#define QUEUE_LENGTH    10
#define ITEM_SIZE       sizeof( uint32_t )

uint8_t qStorage[QUEUE_LENGTH * ITEM_SIZE];
static StaticQueue_t xStaticQueue;

QueueHandle_t xReadings;

static void error_handler() {
    while(1) {}
}
  
static void success_handler(void) {
    // toggle output of LED

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
     // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &led_config); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }
  }

void vTask(void *pvParameters) {
    uint32_t reading;

    // read once
    if (ADC_OneShotRead(ADC_CHANNEL_0,  ADC_SAMPLETIME_3CYCLES, 0) != ADC_OK) error_handler();
    xQueueReceive(xReadings, &reading, 0);

    success_handler();
}

int main() {
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // 10 elem queue
    xReadings = xQueueCreateStatic(QUEUE_LENGTH, ITEM_SIZE, qStorage, &xStaticQueue);

    // init ADC
    ADC_InitTypeDef adc_init = {0};
    
    adc_init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
    adc_init.Resolution = ADC_RESOLUTION_12B;
    adc_init.ScanConvMode = DISABLE;
    adc_init.ContinuousConvMode = DISABLE;
    adc_init.DiscontinuousConvMode = DISABLE;
    adc_init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_init.NbrOfConversion = 1;
    adc_init.DMAContinuousRequests = DISABLE;
    adc_init.EOCSelection = ADC_EOC_SINGLE_CONV;
    
    if (ADC_Init(adc_init, &xReadings) != ADC_OK) error_handler();

    // Task Creation

    xTaskCreateStatic(vTask,
                    "ADC Read",
                    configMINIMAL_STACK_SIZE,
                    (void*) 1,
                    tskIDLE_PRIORITY+4,
                    xStack,
                    &xTaskBuffer);

    vTaskStartScheduler();
    
    // should never reach

    return 0;
}