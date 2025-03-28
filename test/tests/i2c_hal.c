/**
 * Test to confirm the bsp_i2c is functional
 */
#include "FreeRTOS.h"
#include "stm32xx_hal.h"
#include "I2C.h"
#include "task.h"

// Functions used by main
void Clock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
void TestTask(void *argument);

// Variables required for scheduling i2c_hal
I2C_HandleTypeDef hi2c1;
StaticTask_t task_buffer;
StackType_t taskStack[configMINIMAL_STACK_SIZE];

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();
  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_I2C1_Init();

  // Create the task, passing in a priority level and stack size
  xTaskCreateStatic(&TestTask, "Test", configMINIMAL_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + 2, taskStack, &task_buffer);

  // Start the scheduler, which will begin executing tasks
  vTaskStartScheduler();

  // If the scheduler starts successfully, code below will not run
  while (1) {
  }
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  i2c_init(&hi2c1);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // Configure I2C SCL and SDA pins
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin =
      GPIO_PIN_8 | GPIO_PIN_9;  // Example for I2C1 SCL and SDA pins on STM32F4
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint8_t rxBuff[] = {0, 1, 2, 3};

/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
void TestTask(void *argument) {
  while (1) {
    HAL_StatusTypeDef status = i2c_send(&hi2c1, 0x08, rxBuff, 4);
    if (status != HAL_OK) {
      // Handle reception error
    }

    vTaskDelay(pdMS_TO_TICKS(1000));  // Delay for 1 second before receiving again
  }
}
