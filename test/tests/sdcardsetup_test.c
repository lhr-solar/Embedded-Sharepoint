#include "stm32l4xx_hal.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>

sd_handle_t sd;
uint8_t buffer[512];       // buffer to read/write a single SD sector
uint8_t writeData[512];    // test data to write

uint8_t writeBuffer[512];
uint8_t readBuffer[512]; 

void Error_Handler(void)
{
    while (1)
    {
        HAL_Delay(100);
    }
}

void SystemClock_Config(void) 
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) 
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
      * in the RCC_OscInitTypeDef structure.
      */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) 
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) 
    {
        Error_Handler();
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    // Configure SD handle
    sd.hspi = &hspi2; 
    sd.cs_port = GPIOB;
    sd.cs_pin = GPIO_PIN_1;

    // Initialize SPI peripheral
    if (SD_SPI_Init(&sd) != 0) {
        Error_Handler();
    }

    // Initialize SD Card
    if (SD_Init(&sd) != 0) {
        Error_Handler();
    }

    // --- Wake up SD card ---
    SD_Deselect(&sd);
    for (int i = 0; i < 10; i++) {
        SD_Transmit(&sd, 0xFF); // 80 clock cycles
    }

    // --- Send CMD0 to reset the card ---
    SD_Select(&sd);
    uint8_t response = SD_SendCommand(&sd, 0, 0, 0x95); // CMD0: GO_IDLE_STATE
    SD_Deselect(&sd);

    // Now MISO should show the response from SD card on logic analyzer
    // 0x01 = idle state (correct response)
    if (response != 0x01) {
        Error_Handler();
    }

    // Loop forever
    while (1) {
        HAL_Delay(1000);
    }
}
