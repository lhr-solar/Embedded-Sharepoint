#include "stm32xx_hal.h"
#include "sdcard.h"
#include <string.h>

// thread safe:
#include "FreeRTOS.h"
#include "task.h"

/* handles */
sd_handle_t sd;
SPI_HandleTypeDef hspi_user;   

/* RTOS Task Buffers */
StaticTask_t xTask1Buffer;
StackType_t xTask1Stack[1024]; 

StaticTask_t xTask2Buffer;
StackType_t xTask2Stack[1024];

StaticTask_t xTask3Buffer;
StackType_t xTask3Stack[512]; 

StaticTask_t xWorkerBuffer;
StackType_t xWorkerStack[2048]; // Worker needs large stack for FatFs

/* pin def*/
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
#else
    #define USER_SPI_INSTANCE      SPI2
    #define USER_CS_PORT           GPIOB
    #define USER_CS_PIN            GPIO_PIN_9

    #define USER_SCK_PORT          GPIOB
    #define USER_SCK_PIN           GPIO_PIN_13
    #define USER_SCK_AF            GPIO_AF5_SPI2

    #define USER_MISO_PORT         GPIOB
    #define USER_MISO_PIN          GPIO_PIN_14
    #define USER_MISO_AF           GPIO_AF5_SPI2

    #define USER_MOSI_PORT         GPIOB
    #define USER_MOSI_PIN          GPIO_PIN_15
    #define USER_MOSI_AF           GPIO_AF5_SPI2

    #define LED_PORT               GPIOC
    #define LED_PIN                GPIO_PIN_3
    
#endif

void SystemClock_Config(void);
void LED_Init(void);
void User_Hardware_Init(void);

void Task1_Entry(void *params);
void Task2_Entry(void *params);
void Task3_Entry(void *params); 

// IRQ HANDLER
void SPI2_IRQHandler(void) { 
    HAL_SPI_IRQHandler(&hspi_user); 
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    LED_Init();
    User_Hardware_Init(); // Sets up SPI, Clocks, and NVIC Priority 10

    /* Link SD Handle */
    sd.hspi = &hspi_user;
    sd.cs_port = USER_CS_PORT; 
    sd.cs_pin  = USER_CS_PIN;
    
    // Initialize Queues and Mutexes
    if (USER_SD_Card_Init(&sd) != SD_OK) {
        while(1) { HAL_GPIO_TogglePin(LED_PORT, LED_PIN); HAL_Delay(50); }
    }

    // Create Tasks
    xTaskCreateStatic(USER_SD_Card_Worker_Task, "SD_Worker", 2048, &sd, tskIDLE_PRIORITY + 3, xWorkerStack, &xWorkerBuffer);
    xTaskCreateStatic(Task1_Entry, "WriteTask1", 1024, NULL, tskIDLE_PRIORITY + 1, xTask1Stack, &xTask1Buffer);
    xTaskCreateStatic(Task2_Entry, "WriteTask2", 1024, NULL, tskIDLE_PRIORITY + 1, xTask2Stack, &xTask2Buffer);
    xTaskCreateStatic(Task3_Entry, "HighFreq", 512, NULL, tskIDLE_PRIORITY + 2, xTask3Stack, &xTask3Buffer);

    vTaskStartScheduler();
    while(1); 
}

void Task1_Entry(void *params) {
    char *msg = "lsom221Task 1: Async\r\n";
    vTaskDelay(pdMS_TO_TICKS(2000)); 

    for(;;) {
        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET); 
        
        USER_SD_Card_Write_Async(&sd, "LOG1.TXT", msg, pdMS_TO_TICKS(10));

        HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void Task2_Entry(void *params) {
    char *msg = "lsom221Task 2: Async\r\n";
    vTaskDelay(pdMS_TO_TICKS(2500)); 

    for(;;) {
        USER_SD_Card_Write_Async(&sd, "LOG1.TXT", msg, pdMS_TO_TICKS(10));
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}

void Task3_Entry(void *params) {
    for(;;) {
        // steady heartbeat on the logic analyzer -> the SD card isn't blocking the CPU.
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// GPIO for LED init
void LED_Init(void) {
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
        while(1); // Error Handler
    }

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 10;
    
    // L4 uses DIV7
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7; 
    
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        while(1); // Error Handler
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        while(1); // Error Handler
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
        while(1); // Error Handler
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        while(1); // Error Handler
    }

#endif
}

void User_Hardware_Init(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    
    // Force Clocks On
    #if defined(STM32L476xx)
        __HAL_RCC_SPI2_CLK_ENABLE(); 
    #else
        __HAL_RCC_SPI1_CLK_ENABLE();
    #endif

    /* Configure GPIO (SCK, MISO, MOSI, CS) */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct.Pin = USER_SCK_PIN;
    GPIO_InitStruct.Alternate = USER_SCK_AF; 
    HAL_GPIO_Init(USER_SCK_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USER_MISO_PIN;
    GPIO_InitStruct.Alternate = USER_MISO_AF;
    HAL_GPIO_Init(USER_MISO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = USER_MOSI_PIN;
    GPIO_InitStruct.Alternate = USER_MOSI_AF;
    HAL_GPIO_Init(USER_MOSI_PORT, &GPIO_InitStruct);

    HAL_GPIO_WritePin(USER_CS_PORT, USER_CS_PIN, GPIO_PIN_SET); 
    GPIO_InitStruct.Pin = USER_CS_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Alternate = 0;
    HAL_GPIO_Init(USER_CS_PORT, &GPIO_InitStruct);

    /* Configure SPI Peripheral */
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
    
    #if defined(STM32L4xx)
       hspi_user.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
       hspi_user.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
    #endif

    if (HAL_SPI_Init(&hspi_user) != HAL_OK) { while(1); }
    __HAL_SPI_ENABLE(&hspi_user);

    // ENABLE INTERRUPTS (Priority 10)
    #if defined(STM32L476xx)
        HAL_NVIC_SetPriority(SPI2_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(SPI2_IRQn);
    #else
        HAL_NVIC_SetPriority(SPI1_IRQn, 10, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
    #endif
}
