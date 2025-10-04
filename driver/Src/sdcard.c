#include "sdcard.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>


SPI_HandleTypeDef hspi1; //?


void init() {
  HAL_Init();
  SystemClock_Config();
}

void SD_SPI_Init(void) { 

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  __HAL_RCC_SPI1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /**SPI1 GPIO Configuration
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //manually toggle CS:
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Select SD
  // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // Deselect SD

  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }

    //v1
    // __HAL_RCC_SPI1_CLK_ENABLE();   // Enable SPI1 clock
    // __HAL_RCC_GPIOA_CLK_ENABLE();  // Enable GPIOA for SPI pins
    // SPI1 SCK, MISO, MOSI
    // SCK = PA5, MISO = PA6, MOSI = PA7
    // GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
    // GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;      
    // GPIO_InitStruct.Pull = GPIO_NOPULL;          
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;   
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // // SPI1 CS pin
    // // CS = PA4 (manual control in driver)
    // GPIO_InitStruct.Pin = GPIO_PIN_4;
    // GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;  
    // GPIO_InitStruct.Pull = GPIO_NOPULL;
    // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    // HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); 

}

int main() {
    
  init();

  SD_SPI_Init();

  // Configure PA5 as output for onboard LED
  GPIO_InitTypeDef led_init = {0};
  __HAL_RCC_GPIOA_CLK_ENABLE();      // Make sure GPIOA clock is enabled
  led_init.Pin = GPIO_PIN_5;
  led_init.Mode = GPIO_MODE_OUTPUT_PP;
  led_init.Pull = GPIO_NOPULL;
  led_init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &led_init);

  while (1) {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Toggle LED
      HAL_Delay(500);                         // Wait 500 ms
  }

  //test:
  uint8_t testByte = 0xFF;

  // Select SD
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

  // Send one byte
  HAL_SPI_Transmit(&hspi1, &testByte, 1, 100);

  // Deselect SD
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);



}