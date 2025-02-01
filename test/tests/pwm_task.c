// openocd -f openocd-stm32f4x.cfg
// gdb-multiarch build/stm32f401re.elf
// target extended-remote :3333

#include "stm32xx_hal.h"
#include "BSP_PWM.h"
#include 

StaticTask_t task_init_buffer;
StackType_t task_init_stack[configMINIMAL_STACK_SIZE];

static void error_handler(void) {
    while(1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }
}

int main(void) {
    // if (HAL_Init() != HAL_OK) error_handler();
    HAL_Init();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    xTaskCreateStatic(
        Task_Init
    );
    vTaskStartScheduler();

    error_handler();

}