// openocd -f openocd-stm32f4x.cfg
// gdb-multiarch build/stm32f401re.elf
// target extended-remote :3333

#include "stm32xx_hal.h"
#include "BSP_PWM.h"
#include "PWM_Tasks.h"

StaticTask_t Task_Init_Buffer;
StackType_t Task_Init_Stack[configMINIMAL_STACK_SIZE];

static void error_handler(void) {
    while(1) {
        // HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        // HAL_Delay(1000);
    }
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

    error_handler();

}