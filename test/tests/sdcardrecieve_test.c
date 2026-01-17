#include "stm32xx_hal.h"
#include "fatfs.h"
#include "ffconf.h"
#include "ff.h"
#include "user_diskio.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

// UART handle for debug printing
UART_HandleTypeDef huart1;

sd_handle_t sd;
SPI_HandleTypeDef hspi_handle;

// FatFs objects
FATFS fs;
FIL fil;
FIL logFile;   
FRESULT fr;
UINT br, bw;

#define DEBUG_PIN GPIO_PIN_5
#define DEBUG_PORT GPIOA

// Forward declarations
void SystemClock_Config(void);
void Error_Handler(void);
// void MX_GPIO_Init(void);
// void MX_SPI2_Init(void);
void LED_Init(void);

char buffer[128]; // Buffer for reading back

// use led to test if write is done 
// LED pin (onboard LED, e.g., PA5)
#define LED_PORT GPIOA
#define LED_PIN  GPIO_PIN_5

int main(void)
{
    // --- 1. Hardware Init ---
    HAL_Init();
    SystemClock_Config();
    // MX_GPIO_Init();
    // MX_SPI2_Init();
    LED_Init(); 

    // --- 2. Link the handle ---
    sd.hspi = &hspi_handle;
    sd.cs_port = SD_CS_PORT;       
    sd.cs_pin  = SD_CS_PIN; 

    // Turn LED OFF to start
    HAL_GPIO_WritePin(DEBUG_PORT, DEBUG_PIN, GPIO_PIN_RESET);
    HAL_Delay(100);

    // Calls SD_SPI_INIT() internally
    if (SD_Init(&sd) != 0) {
         // Initialization Error: Fast Blink
         while(1) {
            HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN);
            HAL_Delay(50);
         }
    }

    // --- 3. Init FatFs Middleware ---
    MX_FATFS_Init(); 

    // --- 4. Mount Drive ---
    // If this fails: LED blinks FAST (100ms) 5 times
    if(f_mount(&fs, "", 1) != FR_OK) {
        for(int i=0; i<5; i++) { 
            HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN); 
            HAL_Delay(100); 
        }
        return -1;
    }
    
    // --- 5. Prepare Data ---
    char writeBuff[] = "Hello SD Card! This is a test log.\r\n";

    // --- 6. Open File ---
    // If this fails: LED blinks SLOW (1000ms) 2 times
    if(f_open(&logFile, "log.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_OPEN_APPEND) != FR_OK) {
        // for(int i=0; i<2; i++) { 
        //     HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN); 
        //     HAL_Delay(1000); 
        // }
        return -2;
    }

    // --- 7. Write to File ---
    unsigned int bytesWritten;
    FRESULT res = f_write(&logFile, writeBuff, strlen(writeBuff), &bytesWritten);
    
    if(res != FR_OK || bytesWritten == 0) {
        // Write Failed: Blink Medium (500ms) 3 times
        f_close(&logFile);
        // for(int i=0; i<3; i++) {
        //      HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN); 
        //      HAL_Delay(500); 
        // }
        return -3;
    }

    // --- 8. Close File (Saves Data) ---
    f_close(&logFile);

    // --- 9. Unmount ---
    f_mount(NULL, "", 0);

    // --- SUCCESS ---
    // LED Toggles
    // while(1) {
    //     HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN);
    //     HAL_Delay(200);
    // }

    // --- Verify READ Capability ---
    
    // 1. Clear the buffer to prove we are really reading
    char readBuff[128] = {0}; 
    unsigned int bytesRead;

    // 2. Open the file for READING
    if(f_open(&logFile, "log.txt", FA_READ) != FR_OK) {
        // Error opening for read
        while(1) { HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN); HAL_Delay(100); } 
    }

    // 3. Read the data
    if(f_read(&logFile, readBuff, sizeof(readBuff)-1, &bytesRead) != FR_OK) {
        // Error reading
        f_close(&logFile);
        while(1) { HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN); HAL_Delay(100); }
    }

    // 4. Close
    f_close(&logFile);

    // 5. Verify!
    // If the first letter is 'H' (from "Hello"), Blink SLOWLY to celebrate
    if(readBuff[0] == 'H') {
        while(1) {
            HAL_GPIO_TogglePin(DEBUG_PORT, DEBUG_PIN);
            HAL_Delay(1000); // Slow, happy blink
        }
    }
}

// Error handler
void Error_Handler(void)
{
    while (1)
    {
        HAL_Delay(100);
    }
}

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
