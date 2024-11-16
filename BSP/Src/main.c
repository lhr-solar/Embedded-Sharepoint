/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "string.h"
#include "BSP_UART.h"    // Add this
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TEST_MSG         "Hello World\r\n"
#define TEST_MSG_LEN     13

#define RX_QUEUE_LENGTH 128
#define RX_QUEUE_ITEM_SIZE sizeof(uint8_t)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart4;

/* Definitions for UartTxTask */
osThreadId_t UartTxTaskHandle;
const osThreadAttr_t UartTxTask_attributes = {
  .name = "UartTxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UartRxTask */
osThreadId_t UartRxTaskHandle;
const osThreadAttr_t UartRxTask_attributes = {
  .name = "UartRxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
//static QueueHandle_t rxQueue;
//static StaticQueue_t xRxStaticQueue;
//static uint8_t ucRxQueueStorage[RX_QUEUE_LENGTH];

//uint8_t txdata[13] = "Hello World\n";

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);
void StartUartTxTask(void *argument);
void StartUartRxTask(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//	uint8_t tx_buff[] = {0,1,2,3,4,5,6,7,8,9};
//	uint8_t rx_buff[10];
//
//	uint8_t TxData[10240];
//
////	uint8_t FinalData[20];
//	uint8_t RxData[20];
//	uint8_t temp[2];
//	int indx = 0;
//
//	int isSent = 1;
//	int countloop = 0;
//	int countinterrupt = 0;
// Comment these out since they are now handled in BSP_UART
//	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//	{
//		isSent = 1;
//		countinterrupt++;
//	}
//

	uint8_t rxData[5];
//	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//	{
////		memcpy(RxData+indx, temp, 1);
////		if (++indx >= 20) {
////			indx = 0;
////		}
//		HAL_UART_Receive_IT(&huart4, rxData, 5);
//	}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
  /* USER CODE BEGIN 2 */
//  HAL_UART_Receive_IT(&huart2, rx_buff, 10);
//  HAL_UART_Transmit_IT(&huart2, tx_buff, 10);

//  for (uint32_t i=0; i<10240; i++) {
//	  TxData[i] = i&(0xff);
//  }
//
//  HAL_UART_Receive_IT(&huart4, temp, 1);
  static StaticQueue_t xStaticRxQueue;
  static uint8_t ucRxQueueStorageArea[RX_QUEUE_LENGTH * RX_QUEUE_ITEM_SIZE];
  QueueHandle_t xRxQueue;

  // Create the receive queue
  xRxQueue = xQueueCreateStatic(RX_QUEUE_LENGTH,
                             RX_QUEUE_ITEM_SIZE,
                             ucRxQueueStorageArea,
                             &xStaticRxQueue);


  BSP_UART_Init(&huart4, &xRxQueue);

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of UartTxTask */
//  UartTxTaskHandle = osThreadNew(StartUartTxTask, NULL, &UartTxTask_attributes);

  /* creation of UartRxTask */
  UartRxTaskHandle = osThreadNew(StartUartRxTask, NULL, &UartRxTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

//	  if (isSent == 1){
//		  HAL_UART_Transmit_IT(&huart4, TxData, 10240);
//	  }
//	  HAL_UART_Transmit(&huart4, TxData, 10240, HAL_MAX_DELAY);

//	  HAL_UART_Receive(&huart4, RxData, 13, HAL_MAX_DELAY);


//	  HAL_UART_Transmit(&huart4, txdata, sizeof(txdata), 100);
//	  if(temp[0] == '\n')
//	  {
//		  memcpy (FinalData, RxData, indx);
//		  indx = 0;
//	  }
//
//	  HAL_UART_Transmit(&huart4, FinalData, sizeof(FinalData), 100);

//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//	  HAL_Delay(500); //Delay for 0.5 seconds
//	  countloop++;

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief UART4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 9600;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartUartTxTask */
/**
* @brief Function implementing the UartTxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartTxTask */
void StartUartTxTask(void *argument)
{
  /* USER CODE BEGIN 5 */

    const TickType_t xDelay = pdMS_TO_TICKS(500);
//    char txData[] = TEST_MSG;
    char txData[] = "Hello World!\n"; // 13 characters long

    /* Wait for queues to be created */
    osDelay(100);

//	uint8_t txdata[13] = "Hello World\n";
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	  BSP_UART_Write(txData, TEST_MSG_LEN, 0); // message length is 13, busId is 0

//      if (BSP_UART_Write(txData, TEST_MSG_LEN, 0) == HAL_OK)
//      {
//          HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//      }
//	  HAL_UART_Transmit(&huart4, txdata, 13, 100);
//	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//	  HAL_Delay(250); //Delay for 0.5 seconds

      osDelay(xDelay);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartUartRxTask */
/**
* @brief Function implementing the UartRxTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartUartRxTask */
void StartUartRxTask(void *argument)
{
  /* USER CODE BEGIN StartUartRxTask */

	const TickType_t xDelay = pdMS_TO_TICKS(500);

    uint8_t rxData[20];

  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

//	  HAL_UART_Receive_IT(&huart4, rxData, 5);
//	  HAL_UART_Transmit_IT(&huart4, rxData, 5);
//	  HAL_UART_Transmit(&huart4, rxData, 5, HAL_MAX_DELAY);
//	  uint8_t data[5] = "Hello";
//	  BSP_UART_Write((char*)data, 5, 0); // busId is 0
	  // Reads message and puts into rxData
//      if (BSP_UART_Read((char*)rxData, TEST_MSG_LEN, 0) == HAL_OK) // busId is 0
//      {
//          /* Echo/transmit received data back */
//          BSP_UART_Write((char*)rxData, TEST_MSG_LEN, 0); // busId is 0
//      }
	  BSP_UART_Read((char*)rxData, TEST_MSG_LEN, 0);
	  BSP_UART_Write((char*)rxData, TEST_MSG_LEN, 0);
      osDelay(xDelay);
  }
  /* USER CODE END StartUartRxTask */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
