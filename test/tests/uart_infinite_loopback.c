/* UART Loopback Test
 *
 * This test implements a UART loopback where:
 * 1. Send message to RX pin
 * 2. If RX pin receives it, then we send the message again
 *
 * This creates a continuous cycle that verifies both transmit and receive functionality
 * LED pulses on successful receive
 * 
 * 
 * Three Scenarioes:asm
 * Steady State TX/RX succeeds: LED pulses continuously 
 * Steady State TX/RX fails: LED blinks long-short
 */
#include "FreeRTOS.h"
#include "task.h"
#include "stm32xx_hal.h"
#include "UART.h"
#include <string.h>

/* Private defines */
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TEST_MSG_SIZE 17 // Including null terminator

/* Private function prototypes */
void Clock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void LoopbackTask(void *argument);
void Error_Handler(void);

/* Private variables */
extern UART_HandleTypeDef* huart4;  // Using UART4 for the test

// Static task creation resources
StaticTask_t loopbackTaskBuffer;
StackType_t loopbackTaskStack[configMINIMAL_STACK_SIZE];

// Test message
static uint8_t testMessage[TEST_MSG_SIZE] = "Test Message 123\r\n";
static uint8_t rxBuffer[TEST_MSG_SIZE];

int main(void) {
    // Initialize HAL and system clock
    HAL_Init();
    Clock_Config();
    
    // Initialize GPIO (for status LED)
    MX_GPIO_Init();
    
    // Initialize UART peripheral
    MX_UART4_Init();
    
    // Initialize UART driver
    uart_status_t status = uart_init(huart4);
    if (status != UART_OK) {
        Error_Handler();
    }
    
    // Create the loopback task
    xTaskCreateStatic(
        LoopbackTask,
        "Loopback",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        loopbackTaskStack,
        &loopbackTaskBuffer
    );
    
    // Start the scheduler
    vTaskStartScheduler();
    
    // Should never get here
    while (1);
}

// UART configuration for loopback testing
static void MX_UART4_Init(void) {
    huart4->Instance = UART4;
    huart4->Init.BaudRate = 115200;
    huart4->Init.WordLength = UART_WORDLENGTH_8B;
    huart4->Init.StopBits = UART_STOPBITS_1;
    huart4->Init.Parity = UART_PARITY_NONE;
    huart4->Init.Mode = UART_MODE_TX_RX;
    huart4->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4->Init.OverSampling = UART_OVERSAMPLING_16;
    
    #ifdef STM32L4xx
    huart4->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    #endif /* STM32L4xx */
    
    if (HAL_UART_Init(huart4) != HAL_OK) {
        Error_Handler();
    }
}

static void MX_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

    /* Configure LED pin */
    GPIO_InitStruct.Pin = LD2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}

void LoopbackTask(void *argument) {
    uint32_t successCount = 0;
    uint32_t failCount = 0;
    const TickType_t xDelay = pdMS_TO_TICKS(10);  // Short delay between receive attempts
    const uint8_t msgLen = sizeof(testMessage) - 1; // Exclude null terminator
    
    // Initial message transmission to start the cycle
    if (uart_send(huart4, testMessage, msgLen, portMAX_DELAY) != UART_SENT) {
        Error_Handler();
    }
    
    while (1) {
        // Try to receive the message (with timeout)
        uart_status_t rxStatus = uart_recv(huart4, rxBuffer, msgLen, pdMS_TO_TICKS(100));
        
        if (rxStatus == UART_RECV) {
            // Check if received message matches what we sent
            if (memcmp(rxBuffer, testMessage, msgLen) == 0) {
                successCount++;
                
                // Pulse the LED on successful receive
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                vTaskDelay(pdMS_TO_TICKS(50)); // Short pulse
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
                
                // Send the message again to continue the cycle
                if (uart_send(huart4, testMessage, msgLen, portMAX_DELAY) != UART_SENT) {
                    // If we can't send, increment fail count
                    failCount++;
                }
            } else {
                // Received data doesn't match what we sent
                failCount++;
                
                // Indicate an error with a pattern (long-short blink)
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                vTaskDelay(pdMS_TO_TICKS(1000));
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
                vTaskDelay(pdMS_TO_TICKS(250));
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
                vTaskDelay(pdMS_TO_TICKS(250));
                HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
                
                // Try to restart the cycle
                uart_send(huart4, testMessage, msgLen, portMAX_DELAY);
            }
        } else if (rxStatus == UART_EMPTY) {
            // Occasionally restart the cycle if nothing is received
            failCount++;
            
            // After a number of consecutive failures, try to restart the cycle
            if (failCount % 50 == 0) {
                uart_send(huart4, testMessage, msgLen, portMAX_DELAY);
            }
        }
        
        // Short delay before next receive attempt
        vTaskDelay(xDelay);
    }
}

void Clock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    #ifdef STM32L4xx
    // L4 series configuration
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
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    #else
    // F4 series configuration
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
    RCC_OscInitStruct.PLL.PLLM = 16;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
    RCC_OscInitStruct.PLL.PLLQ = 2;
    RCC_OscInitStruct.PLL.PLLR = 2;
    #endif

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    // Configure system clock
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    
    #ifdef STM32L4xx
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
        Error_Handler();
    }
    #else
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    #endif
}

void Error_Handler(void) {
    // Turn on LED to indicate error
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    
    __disable_irq();
    while (1) {
        // Flash LED quickly to indicate error state
        // HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        // HAL_Delay(100);
    }
}