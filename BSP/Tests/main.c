#include "stm32f4xx_hal.h"
#include "BSP_UART.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define RX_QUEUE_LENGTH 128
#define RX_QUEUE_ITEM_SIZE sizeof(uint8_t)

UART_HandleTypeDef huart1;
static QueueHandle_t xRxQueue;

void SystemClock_Config(void);
void Error_Handler(void);
void UART_Init(void);
void TestTask(void *pvParameters);

int main(void) {
    HAL_Init();
    SystemClock_Config();
    
    // User defines a queue for the RX queue
    xRxQueue = xQueueCreate(RX_QUEUE_LENGTH, RX_QUEUE_ITEM_SIZE);
    if (xRxQueue == NULL) {
        Error_Handler();
    }

    UART_Init();

    if (BSP_UART_Init(&huart1, &xRxQueue) != HAL_OK) {
        Error_Handler();
    }

    xTaskCreate(TestTask, "UARTTest", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);

    vTaskStartScheduler();

    while (1) {}
}

void TestTask(void *pvParameters) {
    char txData[] = "UART_TEST";
    int rxData[10];
    uint8_t txLength = strlen(txData);
    uint8_t rxLength = 10;

    while (1) {
        // Transmit test
        BSP_UART_Write(txData, txLength, 0);
        
        // Small delay
        vTaskDelay(pdMS_TO_TICKS(100));

        // Receive test
        BSP_UART_Read((int*)rxData, rxLength, 0);

        // Delay before next iteration
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void UART_Init(void) {
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;

    if (HAL_UART_Init(&huart1) != HAL_OK) {
        Error_Handler();
    }
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line) {
}
#endif