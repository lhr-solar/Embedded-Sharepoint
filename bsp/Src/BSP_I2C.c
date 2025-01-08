/*
 * BSP_I2C.c
 *
 *  Created on: Nov 2, 2024
 *      Author: shaun
 */
#include "../Inc/BSP_I2C.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"
#include "task.h"
#include "assert.h"

/* Used Functions */
static void GPIO_Init(void);
void Single_I2C_Init(I2C_HandleTypeDef*);
void Error_Handler(void);


typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t deviceAdd;
	uint8_t* pDataBuff;
	uint16_t len;
} DataInfo_t;


#define I2C_QUEUE_SIZE   128
#define I2C_DATA_BUFFER_SIZE (I2C_QUEUE_SIZE * sizeof(DataInfo_t))
static DataInfo_t I2C_DataStore[I2C_QUEUE_SIZE];  // Storage for the queue
static StaticQueue_t I2C_DataQueue;
static QueueHandle_t I2C_Queue;
//static DataPacket_t DataBufferStorage[I2C_QUEUE_SIZE];

I2C_HandleTypeDef *last_hi2c;

/**
 * @brief   Sets up the I2C ports as described by the
 */
HAL_StatusTypeDef BSP_I2C_Init(I2C_HandleTypeDef *hi2c) {
	//QUEUES
//	I2C_DataQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof(DataInfo_t), (uint8_t *)DataBufferStorage, &DataStaticQueue);
	I2C_Queue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof (DataInfo_t), (uint8_t *) I2C_DataStore, &I2C_DataQueue);
	// Check if the queue was created successfully
	if (I2C_Queue == NULL) {
		// Handle queue creation failure
		return HAL_ERROR;
	}

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Initialize all configured peripherals */
	GPIO_Init();
	Single_I2C_Init(hi2c);

	last_hi2c = hi2c;

	return HAL_OK;
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   HAL_StatusTypeDef
*/
HAL_StatusTypeDef BSP_I2C_TX(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAdd,
              uint8_t* pDataBuff,
              uint16_t len)
{
	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		last_hi2c = hi2c;
		return HAL_I2C_Master_Transmit_IT(hi2c, deviceAdd, pDataBuff, len);
	}
	else
	{
		DataInfo_t dataToEnqueue = {
			.hi2c = hi2c,
			.deviceAdd = deviceAdd,
			.pDataBuff = pDataBuff,
			.len = len
		};

		if (xQueueSend(I2C_Queue, &dataToEnqueue, portMAX_DELAY) != pdPASS) {
		    // Handle queue full error
		}
	}
	return HAL_OK;
}

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   HAL_StatusTypeDef
*/
HAL_StatusTypeDef BSP_I2C_RX(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAdd,
              uint8_t* pDataBuff,
              uint16_t len)
{
	last_hi2c = hi2c;
    return HAL_I2C_Master_Receive_IT(hi2c, deviceAdd, pDataBuff, len);
}


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
void Single_I2C_Init(I2C_HandleTypeDef *hi2c)
{
	/* Initialize the interrupts */
	// TODO: Make this change based on which I2C is being used
	IRQn_Type EventIRQ;
	IRQn_Type ErrorIRQ;
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_ENABLE();
		EventIRQ = I2C1_EV_IRQn;
		ErrorIRQ = I2C1_ER_IRQn;
	}
	// TODO: Remove this line once finished debugging
	assert(hi2c->Instance == I2C1);

	HAL_NVIC_SetPriority(EventIRQ , 3, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
	HAL_NVIC_EnableIRQ(EventIRQ ); // Enable the I2C interrupt
	HAL_NVIC_SetPriority(ErrorIRQ , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
	HAL_NVIC_EnableIRQ(ErrorIRQ ); // Enable the I2C interrupt

	last_hi2c = hi2c;
	if (HAL_I2C_Init(hi2c) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void GPIO_Init(void)
{
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

void QueueSend(void)
{
	if (HAL_I2C_GetState(last_hi2c) != HAL_I2C_STATE_READY)
		return;

	DataInfo_t data;
	if (xQueueReceive(I2C_Queue, &data, portMAX_DELAY) == pdPASS) {
	    // Process dequeuedData
		HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAdd, data.pDataBuff, data.len);
	} else {
	    // Handle queue empty error
	}
}

/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void)
{
	/* USER CODE BEGIN I2C1_EV_IRQn 0 */
	/* USER CODE BEGIN I2C1_ER_IRQn 0 */
	if (__HAL_I2C_GET_FLAG(last_hi2c, I2C_FLAG_TXE)) {
	    // TXE flag: transmit buffer empty, ready for new data
		;
	}

	if (__HAL_I2C_GET_FLAG(last_hi2c, I2C_FLAG_BTF)) {
	    // BTF flag: Byte transfer finished, transmission complete
		;
	}

	if (__HAL_I2C_GET_FLAG(last_hi2c, I2C_FLAG_STOPF)) {
	    // STOPF flag: stop condition detection, the bus is free
		HAL_I2C_MasterTxCpltCallback(last_hi2c);; // Clear the stop flag
	}
	/* USER CODE END I2C1_EV_IRQn 0 */
	HAL_I2C_EV_IRQHandler(last_hi2c);
	/* USER CODE BEGIN I2C1_EV_IRQn 1 */
	QueueSend();
	/* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
	/* USER CODE END I2C1_ER_IRQn 0 */
	if (__HAL_I2C_GET_FLAG(last_hi2c, I2C_FLAG_AF)) {
		// AF: Acknowledge Failure (NACK received)
		;
	}
	HAL_I2C_ER_IRQHandler(last_hi2c);
	// The NACK has been acknowledged here and checked that the
	// peripheral is ready to send a new item.
	QueueSend();
	/* USER CODE BEGIN I2C1_ER_IRQn 1 */

  /* USER CODE END I2C1_ER_IRQn 1 */
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	;
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	;
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