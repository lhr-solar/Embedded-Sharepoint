#include "stm32l4xx_hal.h" 

//should use:
//#include "stm32xx_hal.h"
#include "fatfs.h"
#include "ffconf.h"
#include "ff.h"
#include "user_diskio.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

//#include "gpio.h"

sd_handle_t sd;

// UART handle for debug printing
UART_HandleTypeDef huart1;

SPI_HandleTypeDef hspi2;

// FatFs objects
FATFS fs;
FIL fil;
FRESULT fr;
UINT br, bw;

#define DEBUG_PIN GPIO_PIN_5
#define DEBUG_PORT GPIOA

// Forward declarations
void SystemClock_Config(void);
void MX_USART1_UART_Init(void);
void myprintf(const char *fmt, ...);
void Error_Handler(void);
void uart_print(const char *msg);

char buffer[128]; // Buffer for reading back

// use led to test if write is done ! 
// LED pin (onboard LED, e.g., PA5)
#define LED_PORT GPIOA
#define LED_PIN  GPIO_PIN_5

// GPIO for LED init
void LED_Init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

// Blink LED n times, then pause and repeat
void LED_BlinkCount(int count)
{

        for (int i = 0; i < count; i++)
        {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            HAL_Delay(200);
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            HAL_Delay(200);
        }
        HAL_Delay(1000); // pause before repeating pattern
    
}

int main(void)
{
    // HAL_Init();
    // SystemClock_Config();

    // // --- Configure SD handle ---
    // sd.hspi = &hspi2;
    // sd.cs_port = GPIOB;
    // sd.cs_pin = GPIO_PIN_1;

    // // --- Initialize SPI & GPIO (includes your CS setup) ---
    // if (SD_SPI_Init(&sd) != 0) {
    //     Error_Handler();
    // }

    // //SD_Transmit(&sd, 0xAF);

    // //     uint8_t pattern = 0xAF; // simple pattern
    // //     uint8_t dummy;

    // // while (1)
    // // {
    // //     HAL_GPIO_WritePin(sd.cs_port, sd.cs_pin, GPIO_PIN_RESET); // CS low (active)
    // //     HAL_SPI_TransmitReceive(&hspi2, &pattern, &dummy, sizeof(pattern), HAL_MAX_DELAY);
    // //     HAL_GPIO_WritePin(sd.cs_port, sd.cs_pin, GPIO_PIN_SET);   // CS high (inactive)
    // //     HAL_Delay(1000);
    // // }

    // while (1)
    // {
    //     SD_SendDummyClocks(&sd);
    //     // Toggle LED to indicate dummy clocks were sent
    //     HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    //     HAL_Delay(500);
    // }


    HAL_Init();
    LED_Init();

    // --- Configure SD handle ---
    sd.hspi = &hspi2;
    sd.cs_port = GPIOB;
    sd.cs_pin = GPIO_PIN_1;

    HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_RESET);

    SystemClock_Config();

    HAL_Delay(200);
    
   // Ensure PB2 high
    // HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);

    // --- Step 1: SPI + SD handle setup ---
    SPI_HandleTypeDef hspi2;
    sd_handle_t sd;
    sd.hspi = &hspi2;
    sd.cs_port = GPIOB;
    sd.cs_pin = GPIO_PIN_1;

    uint8_t spi_status = SD_SPI_Init(&sd);

    if (spi_status == 0)
    {
        // SPI init success → raise debug pin
        //HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);
    }
    else
    {
        while(1);
    }

    // pre mount filesystem sd.init

    //uint8_t sd_status = SD_Init(&sd);
    uint8_t sd_status = SD_Init(&sd);

    if (sd_status == 0)
    {
        HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);
    }
    else
    {
        while(1);
    }


    // --- Step 2: Mount filesystem ---
    FATFS fs;
    FRESULT fr = f_mount(&fs, "", 1);
    if (fr != FR_OK)
    {
         while(1);
    }

    //HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);
    uint8_t testByte = 0xAB;  // dummy byte / idle clock
    SD_Transmit(&sd, testByte);

    // test sd transmit:
    // uint8_t test = 0xFF;
    // for (int i = 0; i < 10; i++) {
    //     SD_Transmit(&sd, test);
    //     //HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
    //     HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);
    //     HAL_Delay(100);
    // }

    // uint8_t sent = 0xFF;       // dummy clocks / idle byte
    // uint8_t received = SD_Transmit(&sd, sent);

    // if (received == 0xFF) {
    //     HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET); // expected idle response
    // } else {
    //     HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_RESET); // something is wrong
    // }

    // test sd command
    // uint8_t r = SD_SendCommand(&sd, 0, 0, 0x95); // CMD0
    // if (r == 0x01) {
    //     HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // LED ON → CMD0 OK
    // } else {
    //     while(1); // LED stays OFF → CMD0 failed
    // }

    uint8_t response = SD_SendCommand(&sd, 0, 0, 0x95); // CMD0
    if (response == 0x01) // expected response: idle state
        HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET); // LED ON
    else
        HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_RESET); // LED OFF

    while (1)
    {
        // optional: toggle LED to show alive
        HAL_Delay(500);
        HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN);
    }

    // test readsector
    // uint8_t buffer[512] = {0};
    // uint8_t r = SD_ReadSector(&sd, 0, buffer);
    // if (r == 0) {
    //     HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // LED ON → read succeeded
    // } else {
    //     while(1); // LED stays OFF → read failed
    // }

        // test writesector
    uint8_t buffer[512];
    memset(buffer, 0xAA, 512); // test pattern
    uint8_t r = SD_WriteSector(&sd, 0, buffer);
    if (r == 0) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); // LED ON → write succeeded
    } else {
        while(1); // LED stays OFF → write failed
    }

    // --- Step 2: Mount filesystem ---
    // FATFS fs;
    // FRESULT fr = f_mount(&fs, "", 1);
    // if (fr != FR_OK)
    // {
    //      while(1);
    // }

    //HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);


    // --- Step 3: Open file for writing ---
    FIL fil;
    fr = f_open(&fil, "write.txt", FA_WRITE | FA_CREATE_ALWAYS);
    if (fr != FR_OK)
    {
        while(1);
    }

    // --- Step 4: Write test string ---
    BYTE writeBuf[32];
    memcpy(writeBuf, "a new file is made!", 19);  // no null needed for f_write
    UINT bytesWritten;
    fr = f_write(&fil, writeBuf, 19, &bytesWritten);
    if (fr == FR_OK)
    {
    }
    else
    {
        while(1);
    }

    // --- Step 5: Close file ---
    f_close(&fil);
    myprintf("File closed.\r\n");

    // --- Step 6: Read back to verify ---
    fr = f_open(&fil, "write.txt", FA_READ);
    if (fr != FR_OK)
    {
        while(1);
    }

    BYTE readBuf[32] = {0};
    UINT bytesRead;
    fr = f_read(&fil, readBuf, sizeof(readBuf)-1, &bytesRead);
    f_close(&fil);

    if (fr == FR_OK)
    {
        readBuf[bytesRead] = '\0'; // null-terminate for printf
    }
    else
    {
        while(1);
    }

    // --- Step 7: Indicate full success on debug pin ---
    HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_SET);

    // Done, infinite loop
    while (1)
    {
    }
}

// int main(void)
// {
//     HAL_Init();
//     SystemClock_Config();
//     MX_USART1_UART_Init();

//     myprintf("\r\n SD card test\r\n\r\n");

//     HAL_Delay(1000); // let SD card settle

//     // --- Step 1: Mount filesystem ---
//     fr = f_mount(&fs, "", 1);
//     if (fr != FR_OK) {
//         myprintf("f_mount failed (%i)\r\n", fr);
//         while(1);
//     }
//     myprintf("Filesystem mounted successfully.\r\n");

//     // --- Step 2: Check free space ---
//     DWORD free_clusters, free_sectors, total_sectors;
//     FATFS* getFreeFs;
//     fr = f_getfree("", &free_clusters, &getFreeFs);
//     if (fr != FR_OK) {
//         myprintf("f_getfree failed (%i)\r\n", fr);
//         while(1);
//     }
//     total_sectors = (getFreeFs->n_fatent - 2) * getFreeFs->csize;
//     free_sectors = free_clusters * getFreeFs->csize;
//     myprintf("SD card stats: %lu KiB total, %lu KiB free.\r\n", total_sectors/2, free_sectors/2);

//     // --- Step 3: Open file for reading ---
//     fr = f_open(&fil, "test.txt", FA_READ);
//     if (fr != FR_OK) {
//         myprintf("f_open test.txt for reading failed (%i)\r\n", fr);
//         while(1);
//     }
//     myprintf("Opened 'test.txt' for reading.\r\n");

//     BYTE readBuf[30];
//     TCHAR* rres = f_gets((TCHAR*)readBuf, 30, &fil);
//     if(rres != 0) {
//         myprintf("Read from 'test.txt': %s\r\n", readBuf);
//     } else {
//         myprintf("f_gets failed (%i)\r\n", fr);
//     }
//     f_close(&fil);

//     // --- Step 4: Open file for writing ---
//     fr = f_open(&fil, "write.txt", FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
//     if(fr != FR_OK) {
//         myprintf("f_open write.txt for writing failed (%i)\r\n", fr);
//         while(1);
//     }
//     myprintf("Opened 'write.txt' for writing.\r\n");

//     strncpy((char*)readBuf, "a new file is made!", 19);
//     fr = f_write(&fil, readBuf, 19, &bw);
//     if(fr == FR_OK) {
//         myprintf("Wrote %i bytes to 'write.txt'.\r\n", bw);
//     } else {
//         myprintf("f_write failed (%i)\r\n", fr);
//     }
//     f_close(&fil);

//     // --- Step 5: Unmount ---
//     f_mount(NULL, "", 0);
//     myprintf("Filesystem unmounted. Demo complete.\r\n");

//     while(1)
//     {
//         HAL_Delay(1000);
//     }
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

// Simple printf via UART
void myprintf(const char *fmt, ...)
{
    static char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    HAL_UART_Transmit(&huart1, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);
}

// Error handler
void Error_Handler(void)
{
    //uart_print("Error occurred!\r\n");
        myprintf("Error occurred!\r\n");
    while(1);
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

// Basic system clock (adjust as CubeMX generated if needed) ver1 old
// void SystemClock_Config(void)
// {
//   RCC_OscInitTypeDef RCC_OscInitStruct = {0};
//   RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

//   /** Configure the main internal regulator output voltage
//   */
//   if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the RCC Oscillators according to the specified parameters
//   * in the RCC_OscInitTypeDef structure.
//   */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
//   RCC_OscInitStruct.HSIState = RCC_HSI_ON;
//   RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
//   RCC_OscInitStruct.PLL.PLLM = 1;
//   RCC_OscInitStruct.PLL.PLLN = 10;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
//   RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
//   RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
//   if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
//   {
//     Error_Handler();
//   }

//   /** Initializes the CPU, AHB and APB buses clocks
//   */
//   RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
//                               |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

//   if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
//   {
//     Error_Handler();
//   }
// }
