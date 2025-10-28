#include "stm32l4xx_hal.h"
#include "ff.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>
//#include "gpio.h"

sd_handle_t sd;

// UART handle for debug printing
UART_HandleTypeDef huart1;

// FatFs objects
FATFS fs;
FIL fil;
FRESULT fr;
UINT br, bw;

// Forward declarations
void SystemClock_Config(void);
void MX_USART1_UART_Init(void);
void Error_Handler(void);
void uart_print(const char *msg);

char buffer[128]; // Buffer for reading back

int main(void)
{
    HAL_Init();
    SystemClock_Config(); // Make sure your clocks are set
    //MX_GPIO_Init();       // Initialize GPIOs for CS, etc.MX_GPIO_Init();  
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_1;  // or whatever pin your CS uses
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    SD_SPI_Init(&sd);       // Initialize SPI peripheral used for SD
    // You can skip USART if you just want SD test

    // Mount the default drive
    FRESULT res;
    res = f_mount(&fs, "", 1);
    if(res != FR_OK){
        // Failed to mount SD card
        while(1);
    }

    // Open or create test.txt for writing
    res = f_open(&fil, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if(res != FR_OK){
        // Failed to open file
        while(1);
    }    

    // Write test string
    const char *text = "Hello from STM32L476RG!\r\n";
    res = f_write(&fil, text, strlen(text), &bw);
    if(res != FR_OK || bw != strlen(text)){
        // Failed to write
        while(1);
    }

    f_close(&fil); // Close the file

    // Read back to verify
    res = f_open(&fil, "test.txt", FA_READ);
    if(res != FR_OK){
        while(1);
    }

    res = f_read(&fil, buffer, sizeof(buffer)-1, &br);
    if(res != FR_OK){
        while(1);
    }
    buffer[br] = '\0'; // Null-terminate

    f_close(&fil);

    // If you have USART1 set up, you could print the buffer here
    // Otherwise just put a breakpoint to inspect 'buffer'

    while(1)
    {
        // Infinite loop after test
    }
}

// int main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_USART1_UART_Init();

//     uart_print("Starting SD card test...\r\n");

//     // Mount the filesystem
//     fr = f_mount(&fs, "", 1);
//     if(fr != FR_OK) {
//         uart_print("Failed to mount filesystem.\r\n");
//         Error_Handler();
//     }
//     uart_print("Filesystem mounted.\r\n");

//     // Open or create a test file
//     fr = f_open(&fil, "test.txt", FA_WRITE | FA_CREATE_ALWAYS);
//     if(fr != FR_OK) {
//         uart_print("Failed to open test file.\r\n");
//         Error_Handler();
//     }

//     // Write some text
//     const char *text = "Hello from STM32L476RG!\r\n";
//     fr = f_write(&fil, text, strlen(text), &bw);
//     if(fr != FR_OK || bw != strlen(text)) {
//         uart_print("Failed to write to file.\r\n");
//         Error_Handler();
//     }
//     uart_print("Data written successfully.\r\n");

//     // Close the file
//     f_close(&fil);
//     uart_print("File closed.\r\n");

//     // Re-open for reading
//     fr = f_open(&fil, "test.txt", FA_READ);
//     if(fr != FR_OK) {
//         uart_print("Failed to open file for reading.\r\n");
//         Error_Handler();
//     }

//     char buffer[128] = {0};
//     fr = f_read(&fil, buffer, sizeof(buffer)-1, &br);
//     if(fr != FR_OK) {
//         uart_print("Failed to read file.\r\n");
//         Error_Handler();
//     }

//     uart_print("Read from file:\r\n");
//     uart_print(buffer);

//     f_close(&fil);
//     uart_print("Test complete.\r\n");

//     while(1);
// }

// Minimal UART1 init for debug
void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

// Simple debug print function
void uart_print(const char *msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

// Error handler
void Error_Handler(void)
{
    uart_print("Error occurred!\r\n");
    while(1);
}

// Basic system clock (adjust as CubeMX generated if needed)
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

// #include "stm32l4xx_hal.h"
// #include "sdcard.h"
// #include <string.h>
// #include <stdio.h>

// #define LD2_GPIO_Port GPIOA
// #define LD2_Pin       GPIO_PIN_5

// #define B1_Pin        GPIO_PIN_13
// #define B1_GPIO_Port  GPIOC

// #define USART_TX_Pin  GPIO_PIN_2
// #define USART_RX_Pin  GPIO_PIN_3

// SPI_HandleTypeDef hspi2;
// sd_handle_t sd;
// uint8_t buffer[512];       // buffer to read/write a single SD sector
// uint8_t writeData[512];    // test data to write
// uint8_t readData[512];

// #define LD2_GPIO_Port GPIOA
// #define LD2_Pin       GPIO_PIN_5

// void Error_Handler(void)
// {
//     while (1)
//     {
//         HAL_Delay(100);
//     }
// }

// void SystemClock_Config(void) 
// {
//     RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//     RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//     /** Configure the main internal regulator output voltage */
//     if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) 
//     {
//         Error_Handler();
//     }

//     /** Initializes the RCC Oscillators according to the specified parameters
//       * in the RCC_OscInitTypeDef structure.
//       */
//     RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//     RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//     RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//     RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//     RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//     RCC_OscInitStruct.PLL.PLLM = 1;
//     RCC_OscInitStruct.PLL.PLLN = 10;
//     RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//     RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//     RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

//     if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) 
//     {
//         Error_Handler();
//     }

//     /** Initializes the CPU, AHB and APB buses clocks */
//     RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
//                                 | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
//     RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//     RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//     RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//     RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//     if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) 
//     {
//         Error_Handler();
//     }
// }

// void MX_SPI2_Init(void)
// {

//   /* USER CODE BEGIN SPI2_Init 0 */

//   /* USER CODE END SPI2_Init 0 */

//   /* USER CODE BEGIN SPI2_Init 1 */

//   /* USER CODE END SPI2_Init 1 */
//   /* SPI2 parameter configuration*/
//   hspi2.Instance = SPI2;
//   hspi2.Init.Mode = SPI_MODE_MASTER;
//   hspi2.Init.Direction = SPI_DIRECTION_2LINES;
//   hspi2.Init.DataSize = SPI_DATASIZE_8BIT; //hspi2.Init.DataSize = SPI_DATASIZE_4BIT;
//   hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
//   hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
//   hspi2.Init.NSS = SPI_NSS_SOFT;
//   hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
//   hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
//   hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
//   hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//   hspi2.Init.CRCPolynomial = 7;
//   hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
//   hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
//   if (HAL_SPI_Init(&hspi2) != HAL_OK)
//   {
//     Error_Handler();
//   }
//   /* USER CODE BEGIN SPI2_Init 2 */

//   /* USER CODE END SPI2_Init 2 */

// }

// void MX_GPIO_Init(void)
// {
//   GPIO_InitTypeDef GPIO_InitStruct = {0};
//   /* USER CODE BEGIN MX_GPIO_Init_1 */

//   /* USER CODE END MX_GPIO_Init_1 */

//   /* GPIO Ports Clock Enable */
//   __HAL_RCC_GPIOC_CLK_ENABLE();
//   __HAL_RCC_GPIOH_CLK_ENABLE();
//   __HAL_RCC_GPIOA_CLK_ENABLE();
//   __HAL_RCC_GPIOB_CLK_ENABLE();

//   /*Configure GPIO pin Output Level */
//   HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

//   /*Configure GPIO pin Output Level */
//   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

//   /*Configure GPIO pin : B1_Pin */
//   GPIO_InitStruct.Pin = B1_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

//   /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
//   GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//   /*Configure GPIO pin : LD2_Pin */
//   GPIO_InitStruct.Pin = LD2_Pin;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

//   /*Configure GPIO pin : PB1 */
//   GPIO_InitStruct.Pin = GPIO_PIN_1;
//   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

//   /*Configure GPIO pins : PA9 PA10 */
//   GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
//   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//   GPIO_InitStruct.Pull = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//   /* USER CODE BEGIN MX_GPIO_Init_2 */

//   /* USER CODE END MX_GPIO_Init_2 */
// }


// int main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_GPIO_Init();  // setup all GPIO
//     MX_SPI2_Init();  // setup SPI2

//     // --- SD handle configuration ---
//     sd.hspi     = &hspi2;
//     sd.cs_port  = GPIOB;       // change to your CS pin port
//     sd.cs_pin   = GPIO_PIN_1;  // change to your CS pin number
    
//     // --- SPI peripheral init for SD ---
//     if (SD_SPI_Init(&sd) != 0) Error_Handler();

//     // --- Initialize SD card ---
//     if (SD_Init(&sd) != 0) Error_Handler();

//     // --- Fill write buffer with test pattern ---
//     for (int i = 0; i < 512; i++) writeData[i] = i & 0xFF;

//     // --- Write sector 0 ---
//     if (SD_WriteSector(&sd, 0, writeData) != 0) Error_Handler();

//     // --- Read back sector 0 ---
//     if (SD_ReadSector(&sd, 0, readData) != 0) Error_Handler();

//     // --- Verify contents ---
//     for (int i = 0; i < 512; i++) {
//         if (readData[i] != writeData[i]) Error_Handler();
//     }

//     // --- Success: toggle onboard LED ---
//     while (1) {
//         HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // LED pin
//         HAL_Delay(500);
//     }
//     // // --- SPI peripheral initialization ---
//     // if (SD_SPI_Init(&sd) != 0) Error_Handler();

//     // // --- Minimal SD init sequence ---
//     // SD_Deselect(&sd);
//     // for (int i = 0; i < 10; i++) SD_Transmit(&sd, 0xFF); // 80 clocks

//     // //SD_Select(&sd);
//     // uint8_t response;
//     // // do {
//     // //     response = SD_Transmit(&sd, 0x40 | 0);  // CMD0
//     // //     SD_Transmit(&sd, 0x00);  // arg[31:24]
//     // //     SD_Transmit(&sd, 0x00);  // arg[23:16]
//     // //     SD_Transmit(&sd, 0x00);  // arg[15:8]
//     // //     SD_Transmit(&sd, 0x00);  // arg[7:0]
//     // //     SD_Transmit(&sd, 0x95);  // CRC for CMD0
//     // // } while (response != 0x01); // wait until idle

//     // // SD_Deselect(&sd);
//     // // SD_Transmit(&sd, 0xFF); // extra clocks

//     // do {
//     //     SD_Select(&sd);
//     //     SD_Transmit(&sd, 0x40 | 0);  // CMD0
//     //     SD_Transmit(&sd, 0x00);
//     //     SD_Transmit(&sd, 0x00);
//     //     SD_Transmit(&sd, 0x00);
//     //     SD_Transmit(&sd, 0x00);
//     //     SD_Transmit(&sd, 0x95);      // valid CRC for CMD0
//     //     response = SD_Receive(&sd);
//     //     SD_Deselect(&sd);
//     //     SD_Transmit(&sd, 0xFF);
//     // } while (response != 0x01);

//     // // --- Fill write buffer with pattern ---
//     // for (int i = 0; i < 512; i++) writeData[i] = i & 0xFF;

//     // // --- Write sector 0 ---
//     // if (SD_WriteSector(&sd, 0, writeData) != 0) Error_Handler();

//     // // --- Read back sector 0 ---
//     // if (SD_ReadSector(&sd, 0, readData) != 0) Error_Handler();

//     // // --- Verify contents ---
//     // for (int i = 0; i < 512; i++)
//     // {
//     //     if (readData[i] != writeData[i]) Error_Handler();
//     // }

//     // // Toggle LED to indicate success
//     // while (1)
//     // {
//     //     HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//     //     HAL_Delay(500);
//     // }
// }