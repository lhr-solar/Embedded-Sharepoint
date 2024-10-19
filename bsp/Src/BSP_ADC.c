#include "stm32xx.h"
#include "queue.h" // use FreeRTOS queue

static QueueHandle_t myRxQueue;
static ADC_HandleTypeDef myADC;
static u_int32_t val; // ADC value

/**
 * @param periph:   ADC_HandleTypeDef w/ channel, clock, and GPIO configs
 */
HAL_StatusTypeDef BSP_ADC_Init(ADC_HandleTypeDef *periph) {
    HAL_Init();

    // store local variables of user pointers
    myADC = periph;

    if (HAL_ADC_Init(&myADC) != HAL_OK) {
        return HAL_ERROR;
    }
    
    HAL_ADC_RegisterCallback(&myADC, HAL_ADC_CONVERSION_COMPLETE_CB_ID, HAL_ADC_ConvCpltCallback); // Register Complete Callback
    HAL_ADC_RegisterCallback(&myADC, HAL_ADC_ERROR_CB_ID, HAL_ADC_ErrorCallback); // Register Error Callback
    
    
    // Create NVIC Interrupt Handler and ISR stuff

}

/**
 * @param periph:   ADC_HandleTypeDef containing unique indentifiers for ADC peripheral
 * @param q         Queue structure to read ADC conversion to
 * @return          Returns HAL_StatusTypeDef of peripheral (can check states, errors, etc.)
 */
HAL_StatusTypeDef BSP_ADC_Read(ADC_HandleTypeDef *periph,  QueueHandle_t *RxQueue) {
    myRxQueue = RxQueue;

    // Check if Queue is full


    // Do ADC Conversion
    HAL_ADC_Start_IT(&myADC);

    // Store ADC Conversion in Queue
    xQueueSend(myRxQueue, &val, 100); 

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *periph) {
    // Callback when ADC conversion is finished
    val = HAL_ADC_GetValue(&periph);

}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *periph) {
    // Callback when ADC conversion results in error

}

