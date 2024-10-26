//#include <stdint.h>
#include "stdbool.h"
#include "stm32x4xx_hal_init.h"
#include "stm32xx_hal.h"
#include "stm32xx_hal_spi.h"

SPI_HandleTypeDef hspi1;

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 0; //dont care because crccalc is disabled

  __SPI1_CLK_ENABLE();
}

 /**  if (HAL_SPI_Init(&hspi1) != HAL_OK) {
    // Error_Handler();
  }
} 
*/

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef SPI1_conf1 = {.Pin = GPIO_PIN_5, // SCK
                                   .Mode = GPIO_MODE_AF_PP, //might be AF
                                   .Pull = GPIO_NOPULL,
                                   .Speed = GPIO_SPEED_FREQ_LOW,
                                   .Alternate = GPIO_AF5_SPI1};
  GPIO_InitTypeDef SPI1_conf2 = {.Pin = GPIO_PIN_6, // MISO
                                   .Mode = GPIO_MODE_AF_PP,
                                   .Pull = GPIO_NOPULL,
                                   .Speed = GPIO_SPEED_FREQ_LOW,
                                   .Alternate = GPIO_AF5_SPI1};
  GPIO_InitTypeDef SPI1_conf3 = {.Pin = GPIO_PIN_7, // MOSI
                                   .Mode = GPIO_MODE_AF_PP,
                                   .Pull = GPIO_NOPULL,
                                   .Speed = GPIO_SPEED_FREQ_LOW,
                                   .Alternate = GPIO_AF5_SPI1};

  /* GPIO Ports Clock Enable */
  __GPIOA_CLK_ENABLE();

  HAL_GPIO_Init(GPIOA, &SPI1_conf1);
  HAL_GPIO_Init(GPIOA, &SPI1_conf2);
  HAL_GPIO_Init(GPIOA, &SPI1_conf3);
}

int main(void) {
  HAL_Init();
  // SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  uint8_t txData[3] = {1,2,3}; // Example data to transmit

  while (1) {
    // Example: Transmit data over SPI
    if (HAL_SPI_GetState(&hspi1) == HAL_SPI_STATE_READY) {
      HAL_SPI_Transmit(&hspi1, txData, sizeof(txData), HAL_MAX_DELAY);

      HAL_Delay(100); // Delay between transmissions
    }
  }
}
