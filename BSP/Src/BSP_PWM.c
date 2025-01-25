#include "../Inc/BSP_PWM.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#ifndef PWM_SEND_QUEUE_SIZE
#define PWM_SEND_QUEUE_SIZE (10)
#endif

static QueueHandle_t pwm1_send_queue;  // need to add queue for each PWM timer
static StaticQueue_t pwm1_send_queue_buffer;
static uint8_t pwm1_send_queue_storage[PWM_SEND_QUEUE_SIZE*sizeof(PWM_Info)];

static QueueHandle_t pwm2_send_queue;  // need to add queue for each PWM timer
static StaticQueue_t pwm2_send_queue_buffer;
static uint8_t pwm2_send_queue_storage[PWM_SEND_QUEUE_SIZE*sizeof(PWM_Info)];

TIM_OC_InitTypeDef sConfigOC = {0};

HAL_StatusTypeDef BSP_PWM_TIM_Init(TIM_HandleTypeDef* timHandle) {
    HAL_StatusTypeDef stat;

    if( __HAL_RCC_GPIOA_IS_CLK_DISABLED())
         __HAL_RCC_GPIOA_CLK_ENABLE();

    if (timHandle->Instance == TIM1) // assign queue for specific timer
        pwm1_send_queue = xQueueCreateStatic(PWM_SEND_QUEUE_SIZE, sizeof(PWM_Info),
        pwm1_send_queue_storage, &pwm1_send_queue_buffer);
    else if (timHandle->Instance == TIM2)
        pwm2_send_queue = xQueueCreateStatic(PWM_SEND_QUEUE_SIZE, sizeof(PWM_Info),
        pwm2_send_queue_storage, &pwm2_send_queue_buffer);
}

HAL_StatusTypeDef BSP_PWM_Channel_Init(TIM_HandleTypeDef* timHandle, uint32_t channel) {    
    HAL_StatusTypeDef stat;
    
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
        if (timHandle->Instance == TIM1) // get queue for specific timer
            tx_Queue = &pwm1_send_queue;
        else if (timHandle->Instance == TIM2)
            tx_Queue = &pwm2_send_queue;
    
        PWM_Info pwmSend = { // for storing PWM info into queue
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

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *timHandle) {
    
    QueueHandle_t* tx_Queue;
    
    if (timHandle->Instance == TIM1)
        tx_Queue = &pwm1_send_queue;
    else if (timHandle->Instance == TIM2)
        tx_Queue = &pwm2_send_queue;
 

    if(!xQueueIsQueueEmptyFromISR(*tx_Queue)) {
        PWM_Info pwmReceive; 
        xQueueReceiveFromISR(*tx_Queue, &pwmReceive, 0);
        HAL_TIM_PWM_Stop_IT(pwmReceive.timHandle, pwmReceive.channel);
        pwmReceive.timHandle->Init.Period = pwmReceive.speed;
        sConfigOC.Pulse = pwmReceive.dutyCycle*(pwmReceive.timHandle->Init.Period)/100;
        HAL_TIM_PWM_ConfigChannel(pwmReceive.timHandle, &sConfigOC, pwmReceive.channel);
        HAL_TIM_PWM_Start_IT(pwmReceive.timHandle, pwmReceive.channel);
    }
}