#include "../Inc/BSP_PWM.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

static QueueHandle_t*  pwmTx_queuePtr [5];  // need to add queue for each PWM timer
TIM_OC_InitTypeDef sConfigOC = {0};

HAL_StatusTypeDef BSP_PWM_Init(TIM_HandleTypeDef* timHandle, uint32_t channel, QueueHandle_t* txPtr) {
    
    HAL_StatusTypeDef stat;
    if( __HAL_RCC_GPIOA_IS_CLK_DISABLED())
         __HAL_RCC_GPIOA_CLK_ENABLE();

    if (timHandle->Instance == TIM1)
        pwmTx_queuePtr[0] = txPtr;
    else if (timHandle->Instance == TIM2)
        pwmTx_queuePtr[1] = txPtr;
    else if (timHandle->Instance == TIM3)
        pwmTx_queuePtr[2] = txPtr;g
    else if (timHandle->Instance == TIM4)
        pwmTx_queuePtr[3] = txPtr;
    else if (timHandle->Instance == TIM5)
        pwmTx_queuePtr[4] = txPtr;
    else
        return HAL_ERROR;

    stat = HAL_TIM_PWM_Init(timHandle);
    if(stat == HAL_ERROR) return stat;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;  
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    stat = HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, channel);
    if(stat == HAL_ERROR) return stat;

    stat = HAL_TIM_PWM_Start_IT(timHandle, channel);
    return stat;
}

HAL_StatusTypeDef BSP_PWM_Set(TIM_HandleTypeDef* timHandle, uint32_t channel, uint32_t dutyCycle, uint32_t speed) {
    if(dutyCycle <= 100 && dutyCycle >= 0) {
        QueueHandle_t* tx_Queue;
        if (timHandle->Instance == TIM1)
            tx_Queue = pwmTx_queuePtr[0];
        else if (timHandle->Instance == TIM2)
            tx_Queue = pwmTx_queuePtr[1];
        else if (timHandle->Instance == TIM3)
            tx_Queue = pwmTx_queuePtr[2];
        else if (timHandle->Instance == TIM4)
            tx_Queue = pwmTx_queuePtr[3];
        else if (timHandle->Instance == TIM5)
            tx_Queue = pwmTx_queuePtr[4];
        else
            return HAL_ERROR;

        PWM_Info pwmSend = {
            .timHandle = timHandle,
            .channel = channel,
            .dutyCycle = dutyCycle,
            .speed = speed
        };
        
        if(!xQueueSend(*tx_Queue, &pwmSend, 0)) {
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    return HAL_ERROR;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {  //need to fix, multiple timers one callback
    QueueHandle_t* tx_Queue;
    
    if (timHandle->Instance == TIM1)
        tx_Queue = pwmTx_queuePtr[0];
    else if (timHandle->Instance == TIM2)
        tx_Queue = pwmTx_queuePtr[1];
    else if (timHandle->Instance == TIM3)
        tx_Queue = pwmTx_queuePtr[2];
    else if (timHandle->Instance == TIM4)
        tx_Queue = pwmTx_queuePtr[3];
    else if (timHandle->Instance == TIM5)
        tx_Queue = pwmTx_queuePtr[4];
    else return;

    if(!xQueueIsQueueEmptyFromISR(*tx_Queue)) {
        PWM_Info pwmReceive; 
        xQueueReceiveFromISR(*tx_Queue, &pwmReceive, 0);
        HAL_TIM_PWM_Stop_IT(pwmReceive.timHandle, pwmReceive.channel);
        sConfigOC.Pulse = pwmReceive.dutyCycle*(pwmReceive.timHandle->Init.Period)/100;
        HAL_TIM_PWM_ConfigChannel(pwmReceive.timHandle, &sConfigOC, pwmReceive.channel);
        HAL_TIM_PWM_Start_IT(pwmReceive.timHandle, pwmReceive.channel);
    }
}