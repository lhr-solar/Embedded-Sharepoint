#include "stm32f4xx_hal.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "stm32xx_hal.h"
#include "stm32f4xx_hal_init.c" // this can be put in "stm32xx_hal.h" later
#include "semphr.h"

/*
NOTE ---
For our boards we use the F413 which only has 1 ADC so no need for specific ADC config
*/


HAL_StatusTypeDef BSP_ADC_Init(ADC_InitTypeDef init, uint8_t bitNum, double Vcc, QueueHandle_t *rxQueue);
/** 
 * init             ADC_InitTypeDef 
 * bitNum           uint8_t         number of bits in ADC
 * Vcc              double
 * rxQueue          QueueHandle_t   pointer to user-provided Queue handle
*/


HAL_StatusTypeDef BSP_ADC_OneShotRead(uint32_t channel, uint32_t samplingTime, bool blocking); 
/**
 *   channel        uint32_t        channel to read
 *   samplingTime   uint32_t        ADC sampling time
 *   blocking       bool            whether the process is blocking
*/