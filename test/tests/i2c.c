#include "FreeRTOS.h"
#include "stm32xx_hal.h"
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
  HAL_Init();
  Clock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  // Create the task, passing in a priority level and stack size
  // Then start the scheduler, which will begin executing tasks
  xTaskCreateStatic(&TestTask, "Test", configMINIMAL_STACK_SIZE, NULL,
                    tskIDLE_PRIORITY + 2, taskStack, &task_buffer);
  vTaskStartScheduler();

  // If the scheduler starts successfully, code below will not run
  while (1) {
  }
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void Clock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  // Configure the main internal regulator output voltage
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  // Initializes the RCC Oscillators according to the specified parameters
  // in the RCC_OscInitTypeDef structure.
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    // Handle Error
  }

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    // Handle Error
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

  __HAL_RCC_I2C1_CLK_ENABLE();
  HAL_I2C_Init(&hi2c1);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  // GPIO Ports Clock Enable
  __HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Configure I2C SCL and SDA pins
  GPIO_InitStruct.Pin =
      GPIO_PIN_8 | GPIO_PIN_9;  // Example for I2C1 SCL and SDA pins on STM32F4
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

uint8_t rxBuff[] = {0, 2, 4, 6};

/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
void TestTask(void *argument) {
  while (1) {
    HAL_I2C_Master_Transmit(&hi2c1, 0x08, rxBuff, 4, 1);
    vTaskDelay(pdMS_TO_TICKS(2));  // Delay for 1 second
  }
}