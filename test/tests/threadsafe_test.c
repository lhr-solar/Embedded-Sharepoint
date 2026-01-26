#include "stm32xx_hal.h"
#include "fatfs.h"
#include "ffconf.h"
#include "ff.h"
#include "user_diskio.h"
#include "fatfs.h"
#include "sdcard.h"
#include <string.h>
#include <stdio.h>

// thread safe:
#include "FreeRTOS.h"
#include "task.h"

/* handles */
sd_handle_t sd;
SPI_HandleTypeDef hspi_handle;
FATFS fs;

StaticTask_t xTask1Buffer;
StackType_t xTask1Stack[1024]; 
TaskHandle_t hTask1;

StaticTask_t xTask2Buffer;
StackType_t xTask2Stack[1024];
TaskHandle_t hTask2;

/* pin def*/
#define LED_PORT GPIOA
#define LED_PIN  GPIO_PIN_5 

void SystemClock_Config(void);
void LED_Init(void);
void Task1_Entry(void *params);
void Task2_Entry(void *params);

int main(void)
{
    /* Hardware Init */
    HAL_Init();
    SystemClock_Config();
    LED_Init();

    /* Link SD Handle */
    sd.hspi = &hspi_handle;
    sd.cs_port = SD_CS_PORT; 
    sd.cs_pin  = SD_CS_PIN;
    
    // Task 1: Writes every 200ms
    xTaskCreateStatic(Task1_Entry, "WriteTask1", 1024, NULL, tskIDLE_PRIORITY + 1, xTask1Stack, &xTask1Buffer);
    
    // Task 2: Writes every 250ms
    xTaskCreateStatic(Task2_Entry, "WriteTask2", 1024, NULL, tskIDLE_PRIORITY + 1, xTask2Stack, &xTask2Buffer);

    /* Scheduler */
    vTaskStartScheduler();

    while(1); 
}

void Task1_Entry(void *params)
{
    // Initialize SD Card 
    if (SD_Init(&sd) != 0) {
        // Fail:
        for(;;) 
        {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN); vTaskDelay(50); 
        }
    }

    //  Init FatFs Middleware 
    MX_FATFS_Init(); 

    // Mount Filesystem
    if(f_mount(&fs, "", 1) != FR_OK) {
        // Fail:
        for(;;) 
        { 
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN); vTaskDelay(500); 
        }
    }

    FIL f1;
    char *data = "Task 1 done\r\n";
    UINT bw;

    for(;;)
    {
        // test blink
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

        // 1. Open File
        if (f_open(&f1, "LOG1.TXT", FA_OPEN_ALWAYS | FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
            
            // 2. Write Data
            f_write(&f1, data, strlen(data), &bw);
            
            // 3. Close (Save)
            f_close(&f1);
        }

        // Wait 200ms
        vTaskDelay(200);
    }
}

void Task2_Entry(void *params)
{
    // Wait a bit for Task 1 to finish Init
    vTaskDelay(500);

    FIL f2;
    char *data = "Task 2 done\r\n";
    UINT bw;

    for(;;)
    {
        // 1. Open File
        if (f_open(&f2, "LOG2.TXT", FA_OPEN_ALWAYS | FA_WRITE | FA_OPEN_APPEND) == FR_OK)
        {
            // 2. Write Data
            f_write(&f2, data, strlen(data), &bw);
            
            // 3. Close
            f_close(&f2);
        }

        vTaskDelay(250);
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

/* CONFIGURATION FOR L4 / G4 CHIPS             */
#if defined(STM32L4xx) || defined(STM32G4xx)

    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
        Error_Handler();
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    
    // L4 uses DIV7, which is what caused your error on F4
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7; 
    
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }

/* CONFIGURATION FOR F4 CHIPS (NEW)            */
#elif defined(STM32F4xx)

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    
    // Standard F4 Config (HSI = 16MHz)
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4; // F4 uses DIV4, not DIV7
    RCC_OscInitStruct.PLL.PLLQ = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }

#endif
}