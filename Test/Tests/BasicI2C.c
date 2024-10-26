#include <stdint.h>

#include "stdbool.h"
#include "stm32x4xx_hal_init.h"
#include "stm32xx_hal.h"
#include "stm32xx_hal_i2c.h"

I2C_HandleTypeDef hi2c1;

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.ClockSpeed = 50000;
  hi2c1.Init.OwnAddress1 = 0x5A;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  //hi2c1.XferOptions = FMPI2C_FIRST_AND_LAST_FRAME;

  HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
  HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

  //   hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  //   hi2c1.Init.OwnAddress2 = 0;
  //   hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  //   hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  __I2C1_CLK_ENABLE();
  __HAL_FMPI2C_ENABLE(&hi2c1);
  //   __HAL_FMPI2C_ENABLE_IT(&hi2c1, FMP_I2C_IT);

  HAL_I2C_Init(&hi2c1);
  //   if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  //   {
  //     Error_Handler();
  //   }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef I2C1_conf1 = {.Pin = GPIO_PIN_8,
                                 .Mode = GPIO_MODE_AF_OD,
                                 .Pull = GPIO_NOPULL,
                                 .Speed = GPIO_SPEED_FREQ_LOW,
                                 .Alternate = GPIO_AF4_I2C1};
  // .Alternate = GPIO_AF4_I2C1
  GPIO_InitTypeDef I2C1_conf2 = {.Pin = GPIO_PIN_9,
                                 .Mode = GPIO_MODE_AF_OD,
                                 .Pull = GPIO_NOPULL,
                                 .Speed = GPIO_SPEED_FREQ_LOW,
                                 .Alternate = GPIO_AF4_I2C1};

  /* GPIO Ports Clock Enable */
  __GPIOB_CLK_ENABLE();

  HAL_GPIO_Init(GPIOB, &I2C1_conf1);
  HAL_GPIO_Init(GPIOB, &I2C1_conf2);
}

void LED_Init(void) {
  GPIO_InitTypeDef led_config = {
      .Mode = GPIO_MODE_OUTPUT_PP, .Pull = GPIO_NOPULL, .Pin = GPIO_PIN_5};

  __HAL_RCC_GPIOA_CLK_ENABLE();       // enable clock for GPIOA
  HAL_GPIO_Init(GPIOA, &led_config);  // initialize GPIOA with led_config
}

int main() {
  HAL_Init();
  // SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  LED_Init();

  uint8_t daBuff[] = {1, 2, 3, 4};

  while (1) {
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
    // HAL_I2C_Master_Transmit_IT(&hi2c1, 1 << 1, buffer, 4);

    // if (!*daBuff && (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)) {
    //   HAL_Delay(100);
    //   HAL_I2C_Master_Receive_IT(&hi2c1, 0x5A, daBuff, 1);
    // }

    if ((HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)) {
      HAL_I2C_Master_Transmit(&hi2c1, 0x5A, daBuff,
                                 sizeof(daBuff) / sizeof(*daBuff), 1000);
      //   daBuff[0] = 0;
    }

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    // HAL_Delay(100);
  }
}

// void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
//   HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//   HAL_Delay(500);
//   __HAL_FMPI2C_CLEAR_FLAG();
// }

// void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
//   // RX Done .. Do Something!
// }