#include "sdcard.h"

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

    uint8_t testByte = 0xFF;

    // SPI test: send one byte to SD card
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Select SD
    HAL_SPI_Transmit(&hspi1, &testByte, 1, 100); //send
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // Deselect SD

}

/*
  while (1) {
      HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Toggle LED
      HAL_Delay(500);                         // Wait 500 ms
  }

  //test:
  uint8_t testByte = 0xFF;

  while(1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Select SD
    HAL_SPI_Transmit(&hspi1, &testByte, 1, 100);          // Send 0xFF
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // Deselect SD
    HAL_Delay(500);                                       // Small delay
}


  // Select SD
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

// Send one byte
HAL_SPI_Transmit(&hspi1, &testByte, 1, 100);

// Deselect SD
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);*/

