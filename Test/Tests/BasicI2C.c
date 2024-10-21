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

  //   hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  //   hi2c1.Init.OwnAddress2 = 0;
  //   hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  //   hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;

  __I2C1_CLK_ENABLE();

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

int main() {
  HAL_Init();
  // SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  uint8_t daBuff[1] = {1};

  while (1) {
    // HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
    // HAL_I2C_Master_Transmit_IT(&hi2c1, 1 << 1, buffer, 4);

    // if (!*daBuff && (HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)) {
    //   HAL_Delay(100);
    //   HAL_I2C_Master_Receive_IT(&hi2c1, 0x5A, daBuff, 1);
    // }

    if ((HAL_I2C_GetState(&hi2c1) == HAL_I2C_STATE_READY)) {
      HAL_Delay(100);
      HAL_I2C_Master_Transmit_IT(&hi2c1, 0x5A, daBuff,
                              sizeof(daBuff) / sizeof(*daBuff));
      daBuff[0] = 0;
    }
    // HAL_Delay(100);
  }
}