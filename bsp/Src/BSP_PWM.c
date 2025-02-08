#include "BSP_PWM.h"

#ifndef PWM_SEND_QUEUE_SIZE
#define PWM_SEND_QUEUE_SIZE (10)
#endif

static QueueHandle_t pwm1_send_queue = NULL;  // need to add queue for each PWM timer
static StaticQueue_t pwm1_send_queue_buffer;
static uint8_t pwm1_send_queue_storage[PWM_SEND_QUEUE_SIZE*sizeof(PWM_Info)];
        
static QueueHandle_t pwm2_send_queue = NULL;  // need to add queue for each PWM timer
static StaticQueue_t pwm2_send_queue_buffer;
static uint8_t pwm2_send_queue_storage[PWM_SEND_QUEUE_SIZE*sizeof(PWM_Info)];

TIM_OC_InitTypeDef sConfigOC = {0};

void MX_GPIO_Init(){
    GPIO_InitTypeDef pwm_tim1_ch1 = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_8,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF1_TIM1
    };

    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
    // GPIO_InitTypeDef pwm_tim1_ch2 = {
    //     .Mode = GPIO_MODE_AF_PP,
    //     .Pull = GPIO_NOPULL,
    //     .Pin = GPIO_PIN_9,
    //     .Speed = GPIO_SPEED_FREQ_LOW,
    //     .Alternate = GPIO_AF1_TIM1
    // };

    GPIO_InitTypeDef pwm_tim2_ch2 = {
        .Mode = GPIO_MODE_AF_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_1,
        .Speed = GPIO_SPEED_FREQ_LOW,
        .Alternate = GPIO_AF1_TIM2
    };

    HAL_GPIO_Init(GPIOA, &pwm_tim1_ch1);
    HAL_GPIO_Init(GPIOA, &led_config);
    HAL_GPIO_Init(GPIOA, &pwm_tim2_ch2);
}

HAL_StatusTypeDef BSP_PWM_TIM_Init(TIM_HandleTypeDef* timHandle) {
    HAL_StatusTypeDef stat = HAL_OK;
    
    if(__HAL_RCC_GPIOA_IS_CLK_DISABLED())
         __HAL_RCC_GPIOA_CLK_ENABLE();
    
    if (timHandle->Instance == TIM1 && pwm1_send_queue == NULL) {// assign queue for specific timer

        pwm1_send_queue = xQueueCreateStatic(PWM_SEND_QUEUE_SIZE, sizeof(PWM_Info),
        pwm1_send_queue_storage, &pwm1_send_queue_buffer);
    
        if (pwm1_send_queue == NULL) 
            return HAL_ERROR; 
        }
    else if (timHandle->Instance == TIM2 && pwm2_send_queue == NULL) {
        
        pwm2_send_queue = xQueueCreateStatic(PWM_SEND_QUEUE_SIZE, sizeof(PWM_Info),
        pwm2_send_queue_storage, &pwm2_send_queue_buffer); 

        if (pwm2_send_queue == NULL) 
            return HAL_ERROR; 
        }

    stat = HAL_TIM_PWM_Init(timHandle);
    if(stat == HAL_ERROR) return stat;

    return stat;
}

HAL_StatusTypeDef BSP_PWM_Channel_Init(TIM_HandleTypeDef* timHandle, uint8_t channel) {    
    HAL_StatusTypeDef stat = HAL_OK;
    
    sConfigOC.OCMode = TIM_OCMODE_PWM1;  
    sConfigOC.Pulse = 5000;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

    
    stat = HAL_TIM_PWM_ConfigChannel(timHandle, &sConfigOC, channel);
    if(stat == HAL_ERROR) return stat;
    // while(timHandle->Channel[])

    stat = HAL_TIM_PWM_Start_IT(timHandle, channel); 
    return stat;
}

HAL_StatusTypeDef BSP_PWM_Set(TIM_HandleTypeDef* timHandle, uint8_t channel, uint8_t dutyCycle, uint64_t speed) {
    if(dutyCycle <= 100 && dutyCycle >= 0) {

        QueueHandle_t tx_Queue = NULL;
        if (timHandle->Instance == TIM1) // get queue for specific timer
            tx_Queue = pwm1_send_queue;
        else if (timHandle->Instance == TIM2)
            tx_Queue = pwm2_send_queue;
    
        PWM_Info pwmSend = { // for storing PWM info into queue
            .timHandle = timHandle,
            .channel = channel,
            .dutyCycle = dutyCycle,
            .speed = speed
        };
        
        if(!xQueueSend(tx_Queue, &pwmSend, 0)) {
            return HAL_ERROR;
        }

        return HAL_OK;
    }
    return HAL_ERROR;
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *timHandle) {

    // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // testing interrupt callback
    QueueHandle_t tx_Queue = NULL;
    
    if (timHandle->Instance == TIM1)
        tx_Queue = pwm1_send_queue;
    else if (timHandle->Instance == TIM2)
        tx_Queue = pwm2_send_queue;
 

    if(!xQueueIsQueueEmptyFromISR(tx_Queue)) {
        PWM_Info pwmReceive; 
        xQueueReceiveFromISR(tx_Queue, &pwmReceive, 0);
        HAL_TIM_PWM_Stop_IT(pwmReceive.timHandle, pwmReceive.channel);
        pwmReceive.timHandle->Init.Period = pwmReceive.speed;
        sConfigOC.Pulse = pwmReceive.dutyCycle*(pwmReceive.timHandle->Init.Period)/100;
        HAL_TIM_PWM_ConfigChannel(pwmReceive.timHandle, &sConfigOC, pwmReceive.channel);
        HAL_TIM_PWM_Start_IT(pwmReceive.timHandle, pwmReceive.channel);
    }
}

