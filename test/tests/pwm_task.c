// openocd -f openocd-stm32f4x.cfg
// gdb-multiarch build/stm32f401re.elf
// target extended-remote :3333

#include "stm32xx_hal.h"
#include "BSP_PWM.h"
// #include "PWM_Tasks.h"

#define TASK_INIT_PRIORITY      tskIDLE_PRIORITY + 2
#define TASK_TOGGLE_PRIORITY    tskIDLE_PRIORITY + 2
#define TASK_TIM1_PRIORITY      tskIDLE_PRIORITY + 2
#define TASK_TIM2_PRIORITY      tskIDLE_PRIORITY + 2

StaticTask_t Task_Init_Buffer;
StackType_t Task_Init_Stack[configMINIMAL_STACK_SIZE];

StaticTask_t Task_Toggle_Buffer;
StackType_t Task_Toggle_Stack[configMINIMAL_STACK_SIZE];

StaticTask_t Task_TIM1_Buffer;
StackType_t Task_TIM1_Stack[configMINIMAL_STACK_SIZE];

StaticTask_t Task_TIM2_Buffer;
StackType_t Task_TIM2_Stack[configMINIMAL_STACK_SIZE];

static TIM_HandleTypeDef tim1;
static TIM_HandleTypeDef tim2;

static void error_handler(void) {
    while(1) {
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        // HAL_Delay(1000);
    }
}

void Task_Toggle(void * pvParameters) {
    while(1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        vTaskDelay(200);
    }
}

void Task_TIM1(void * pvParameters) {
    while(1) {
        if(BSP_PWM_Set(&tim1, TIM_CHANNEL_1, 75, 100000 - 1) != HAL_OK)
            error_handler();
        vTaskDelay(500);
    }
}

void Task_TIM2(void * pvParameters) {
    while(1) {
        if(BSP_PWM_Set(&tim2, TIM_CHANNEL_2, 50, 100000 - 1) != HAL_OK)
            error_handler();
        vTaskDelay(500);
    }
}

void Task_Init() {
    MX_GPIO_Init();

    tim1.Instance = TIM1;
    tim1.Init.Prescaler = 8-1;
    tim1.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim1.Init.Period = 10000 - 1;
    tim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    tim2.Instance = TIM2;
    tim2.Init.Prescaler = 8-1;
    tim2.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim2.Init.Period = 10000 - 1;
    tim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    
    if (BSP_PWM_TIM_Init(&tim1) != HAL_OK) error_handler();
    if (BSP_PWM_TIM_Init(&tim2) != HAL_OK) error_handler();

    // BSP_PWM_Channel_Init(&tim1, TIM_CHANNEL_1);
    // BSP_PWM_Channel_Init(&tim2, TIM_CHANNEL_2);
    if (BSP_PWM_Channel_Init(&tim1, TIM_CHANNEL_1) != HAL_OK) error_handler();
    if (BSP_PWM_Channel_Init(&tim2, TIM_CHANNEL_2) != HAL_OK) error_handler();

    xTaskCreateStatic(
        Task_Toggle,
        "Toggle Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TASK_TOGGLE_PRIORITY, //temporary
        Task_Toggle_Stack,
        &Task_Toggle_Buffer
        );

    xTaskCreateStatic(
        Task_TIM1,
        "PWM TIM1 Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TASK_TIM1_PRIORITY, //temporary
        Task_TIM1_Stack,
        &Task_TIM1_Buffer
        );
    
    xTaskCreateStatic(
        Task_TIM2,
        "PWM TIM2 Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TASK_TIM2_PRIORITY, //temporary
        Task_TIM2_Stack,
        &Task_TIM2_Buffer
        );

    vTaskDelete(NULL);
}

int main(void) {
    // if (HAL_Init() != HAL_OK) error_handler();
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    xTaskCreateStatic(
        Task_Init,
        "Initialization Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        TASK_INIT_PRIORITY,
        Task_Init_Stack,
        &Task_Init_Buffer
    );

    vTaskStartScheduler();

    while(1) {}

}