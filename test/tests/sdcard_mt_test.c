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
SPI_HandleTypeDef hspi_user;   
FATFS fs;

StaticTask_t xTask1Buffer;
StackType_t xTask1Stack[1024]; 
TaskHandle_t hTask1;

StaticTask_t xTask2Buffer;
StackType_t xTask2Stack[1024];
TaskHandle_t hTask2;

StaticTask_t xTask3Buffer;
StackType_t xTask3Stack[configMINIMAL_STACK_SIZE]; 
TaskHandle_t hTask3;

/* pin def*/
// ADAPT THESE TO YOUR BOARD
#if defined(STM32L476xx)
    #define USER_SPI_INSTANCE  SPI2
    #define USER_CS_PORT       GPIOB
    #define USER_CS_PIN        GPIO_PIN_12

    #define USER_SCK_PORT      GPIOB
    #define USER_SCK_PIN       GPIO_PIN_10
    #define USER_SCK_AF        GPIO_AF5_SPI2

    #define USER_MISO_PORT     GPIOC
    #define USER_MISO_PIN      GPIO_PIN_2
    #define USER_MISO_AF       GPIO_AF5_SPI2

    #define USER_MOSI_PORT     GPIOC
    #define USER_MOSI_PIN      GPIO_PIN_3
    #define USER_MOSI_AF       GPIO_AF5_SPI2

    #define LED_PORT           GPIOA
    #define LED_PIN            GPIO_PIN_5

    #define LED_PORT GPIOA
    #define LED_PIN  GPIO_PIN_5 

#elif defined (lsom)
    #define USER_SPI_INSTANCE      SPI2
    
    // Chip Select (NSS) - PB9
    #define USER_CS_PORT           GPIOB
    #define USER_CS_PIN            GPIO_PIN_9

    // Serial Clock (SCK) - PB13
    #define USER_SCK_PORT          GPIOB
    #define USER_SCK_PIN           GPIO_PIN_13
    #define USER_SCK_AF            GPIO_AF5_SPI2

    // MISO - PB14
    #define USER_MISO_PORT         GPIOB
    #define USER_MISO_PIN          GPIO_PIN_14
    #define USER_MISO_AF           GPIO_AF5_SPI2

    // MOSI - PB15
    #define USER_MOSI_PORT         GPIOB
    #define USER_MOSI_PIN          GPIO_PIN_15
    #define USER_MOSI_AF           GPIO_AF5_SPI2

    // LED - (Keeping original as it wasn't specified in the image)
    #define LED_PORT               GPIOA
    #define LED_PIN                GPIO_PIN_5

#else
    #define USER_SPI_INSTANCE  SPI1
    #define USER_CS_PORT       GPIOA
    #define USER_CS_PIN        GPIO_PIN_4

    #define USER_SCK_PORT      GPIOA
    #define USER_SCK_PIN       GPIO_PIN_5
    #define USER_SCK_AF        GPIO_AF5_SPI1

    #define USER_MISO_PORT     GPIOA
    #define USER_MISO_PIN      GPIO_PIN_6
    #define USER_MISO_AF       GPIO_AF5_SPI1

    #define USER_MOSI_PORT     GPIOA
    #define USER_MOSI_PIN      GPIO_PIN_7
    #define USER_MOSI_AF       GPIO_AF5_SPI1

    #define LED_PORT           GPIOA
    #define LED_PIN            GPIO_PIN_5
#endif

void SystemClock_Config(void);
void LED_Init(void);
void User_Hardware_Init(void);
void Task1_Entry(void *params);
void Task2_Entry(void *params);
void Task3_Entry(void *params); // task for high frequency


int main(void)
{
    /* Hardware Init */
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    User_Hardware_Init();

    /* Link SD Handle */
    sd.hspi = &hspi_user;
    sd.cs_port = USER_CS_PORT; 
    sd.cs_pin  = USER_CS_PIN;

    
    // Task 1: Writes every 200ms
    xTaskCreateStatic(Task1_Entry, "WriteTask1", 1024, NULL, tskIDLE_PRIORITY + 1, xTask1Stack, &xTask1Buffer);
    
    // Task 2: Writes every 250ms
    xTaskCreateStatic(Task2_Entry, "WriteTask2", 1024, NULL, tskIDLE_PRIORITY + 1, xTask2Stack, &xTask2Buffer);

    // Task 3: toggle every 10ms
    xTaskCreateStatic(Task3_Entry, "HighFreq", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, xTask3Stack, &xTask3Buffer);

    /* Scheduler */
    vTaskStartScheduler();

    while(1); 
}

void Task1_Entry(void *params)
{
    // Initialize SD Card 
    if (USER_SD_Card_Init(&sd) != 0) {
        // Fail:
        for(;;) 
        {
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN); 
            vTaskDelay(50); 
        }
    }

    // //  Init FatFs Middleware 
    // MX_FATFS_Init(); 

    // Mount Filesystem
    if(f_mount(&fs, "", 1) != FR_OK) {
        // Fail:
        for(;;) 
        { 
            HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
            vTaskDelay(500); 
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
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        
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

void Task3_Entry(void *params)
{
    for(;;) {
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(10);
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

void User_Hardware_Init(void) {
    
    /* 1. Enable Clocks */
    // Note: You must enable clocks for ALL ports you use
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // Enable SPI Clock based on your choice
    if(USER_SPI_INSTANCE == SPI1) __HAL_RCC_SPI1_CLK_ENABLE();
    #ifdef SPI2
        else if(USER_SPI_INSTANCE == SPI2) {
            __HAL_RCC_SPI2_CLK_ENABLE();
        }
    #endif

    #ifdef SPI3
        else if(USER_SPI_INSTANCE == SPI3) {
            __HAL_RCC_SPI3_CLK_ENABLE();
        }
    #endif

    /* 2. Configure GPIO (SCK, MISO, MOSI) */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    // SCK
    GPIO_InitStruct.Pin = USER_SCK_PIN;
    GPIO_InitStruct.Alternate = USER_SCK_AF; 
    HAL_GPIO_Init(USER_SCK_PORT, &GPIO_InitStruct);

    // MISO
    GPIO_InitStruct.Pin = USER_MISO_PIN;
    GPIO_InitStruct.Alternate = USER_MISO_AF;
    HAL_GPIO_Init(USER_MISO_PORT, &GPIO_InitStruct);

    // MOSI
    GPIO_InitStruct.Pin = USER_MOSI_PIN;
    GPIO_InitStruct.Alternate = USER_MOSI_AF;
    HAL_GPIO_Init(USER_MOSI_PORT, &GPIO_InitStruct);

    /* 3. Configure Chip Select (CS) - Standard Output */
    HAL_GPIO_WritePin(USER_CS_PORT, USER_CS_PIN, GPIO_PIN_SET); // Default High
    GPIO_InitStruct.Pin = USER_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(USER_CS_PORT, &GPIO_InitStruct);

    /* 4. Configure SPI Peripheral */
    hspi_user.Instance = USER_SPI_INSTANCE;
    hspi_user.Init.Mode = SPI_MODE_MASTER;
    hspi_user.Init.Direction = SPI_DIRECTION_2LINES;
    hspi_user.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi_user.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi_user.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi_user.Init.NSS = SPI_NSS_SOFT;
    hspi_user.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; 
    hspi_user.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi_user.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi_user.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    
    // STM32L4/G4 specific
    #if defined(STM32L4xx) || defined(STM32G4xx)
       hspi_user.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
       hspi_user.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    #endif

    if (HAL_SPI_Init(&hspi_user) != HAL_OK) { 
        while(1); 
    }
    __HAL_SPI_ENABLE(&hspi_user);

    // ENABLE INTERRUPTS 
    if (USER_SPI_INSTANCE == SPI1) {
        HAL_NVIC_SetPriority(SPI1_IRQn, 5, 0); 
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    }
    #ifdef SPI2
    else if (USER_SPI_INSTANCE == SPI2) {
        HAL_NVIC_SetPriority(SPI2_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    }
    #endif
    #ifdef SPI3
    else if (USER_SPI_INSTANCE == SPI3) {
        HAL_NVIC_SetPriority(SPI3_IRQn, 5, 0);
        HAL_NVIC_EnableIRQ(SPI3_IRQn);
    }
    #endif
}

// *** IRQ HANDLERS ***
void SPI1_IRQHandler(void) { 
    HAL_SPI_IRQHandler(&hspi_user); 
}
#ifdef SPI2
void SPI2_IRQHandler(void) { 
    HAL_SPI_IRQHandler(&hspi_user); 
}
#endif
#ifdef SPI3
void SPI3_IRQHandler(void) { 
    HAL_SPI_IRQHandler(&hspi_user); 
}
#endif