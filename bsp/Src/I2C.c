/*
 * BSP_I2C.c
 *
 *  Created on: Nov 2, 2024
 *      Author: shaun
 */
#include "I2C.h"
#include "stm32xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"
#include "task.h"
#include "assert.h"

/* Used Functions */
void Single_I2C_Init(I2C_HandleTypeDef*);


typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t deviceAddr;
	uint8_t* pDataBuff;
	uint16_t len;
} DataInfo_t;

// Define queue sizes for each I2C peripheral if not done so already
#ifdef I2C1
#ifndef I2C1_QUEUE_SIZE
#define I2C1_QUEUE_SIZE 128
#endif
static DataInfo_t I2C1_DataStore[I2C1_QUEUE_SIZE];
static QueueHandle_t I2C1_Queue;
static StaticQueue_t I2C1_DataQueue;
I2C_HandleTypeDef *last_hi2c1;
#endif

#ifdef I2C2
#ifndef I2C2_QUEUE_SIZE 
#define I2C2_QUEUE_SIZE 128
#endif
static DataInfo_t I2C2_DataStore[I2C2_QUEUE_SIZE];
static StaticQueue_t I2C2_DataQueue;
static QueueHandle_t I2C2_Queue;
I2C_HandleTypeDef *last_hi2c2;
#endif

#ifdef I2C3
#ifndef I2C3_QUEUE_SIZE 
#define I2C3_QUEUE_SIZE 128
#endif
static DataInfo_t I2C3_DataStore[I2C3_QUEUE_SIZE];
static StaticQueue_t I2C3_DataQueue;
static QueueHandle_t I2C3_Queue;
I2C_HandleTypeDef *last_hi2c3;
#endif


static void update_last_i2c(I2C_HandleTypeDef *hi2c) {
	#ifdef I2C1
	if (hi2c->Instance == I2C1) {
		last_hi2c1 = hi2c;
		return;
	}
	#endif

	#ifdef I2C2
	if (hi2c->Instance == I2C2) {
		last_hi2c2 = hi2c;
		return;
	}
	#endif

	#ifdef I2C3
	if (hi2c->Instance == I2C3) {
		last_hi2c3 = hi2c;
		return;
	}
	#endif
	
	// Handle invalid I2C peripheral under here
}

/**
 * @brief   Sets up the I2C ports as described by the
 */
HAL_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c) {

	// Initialize the respective I2C peripheral's queue and interrupts
	#ifdef I2C1
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_ENABLE();
		I2C1_Queue = xQueueCreateStatic(I2C1_QUEUE_SIZE, sizeof (DataInfo_t), (uint8_t *) I2C1_DataStore, &I2C1_DataQueue);
		// Check if the queue was created successfully
		if (I2C1_Queue == NULL)
			// Handle queue creation failure
			return HAL_ERROR;

		HAL_NVIC_SetPriority(I2C1_EV_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C1_ER_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn); // Enable the I2C interrupt
		if (HAL_I2C_Init(hi2c) != HAL_OK)
		{
			// Hancle I2C initialization error
			return HAL_ERROR;
		}
	}
	#endif

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		__HAL_RCC_I2C2_CLK_ENABLE();
		I2C2_Queue = xQueueCreateStatic(I2C2_QUEUE_SIZE, sizeof (DataInfo_t), (uint8_t *) I2C2_DataStore, &I2C2_DataQueue);
		// Check if the queue was created successfully
		if (I2C2_Queue == NULL)
			// Handle queue creation failure
			return HAL_ERROR;

		HAL_NVIC_SetPriority(I2C2_EV_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C2_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C2_ER_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C2_ER_IRQn); // Enable the I2C interrupt
		if (HAL_I2C_Init(hi2c) != HAL_OK)
		{
			// Hancle I2C initialization error
			return HAL_ERROR;
		}
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		__HAL_RCC_I2C3_CLK_ENABLE();
		I2C3_Queue = xQueueCreateStatic(I2C3_QUEUE_SIZE, sizeof (DataInfo_t), (uint8_t *) I2C3_DataStore, &I2C3_DataQueue);
		// Check if the queue was created successfully
		if (I2C3_Queue == NULL)
			// Handle queue creation failure
			return HAL_ERROR;

		HAL_NVIC_SetPriority(I2C3_EV_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C3_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C3_ER_IRQn , 5, 0); // set to priority 5 (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C3_ER_IRQn); // Enable the I2C interrupt
		if (HAL_I2C_Init(hi2c) != HAL_OK)
		{
			// Hancle I2C initialization error
			return HAL_ERROR;
		}
	}
	#endif

	return HAL_ERROR;
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   HAL_StatusTypeDef
*/
HAL_StatusTypeDef i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{
	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		return HAL_I2C_Master_Transmit_IT(hi2c, deviceAddr, pDataBuff, len);
	}
	else
	{
		DataInfo_t dataToEnqueue = {
			.hi2c = hi2c,
			.deviceAddr = deviceAddr,
			.pDataBuff = pDataBuff,
			.len = len
		};

		QueueHandle_t I2C_Queue = I2C1_Queue;
		if(hi2c->Instance == I2C1) {}
		else if (hi2c->Instance == I2C2)
		{
			I2C_Queue = I2C2_Queue;
		}
		else if (hi2c->Instance == I2C3)
		{
			I2C_Queue = I2C3_Queue;
		}
		else
		{
			// Handle invalid I2C peripheral
			return HAL_ERROR;
		}
		

		if (xQueueSend(I2C_Queue, &dataToEnqueue, 0) != pdPASS)
		{
			// Handle queue full error
			return HAL_ERROR;
		}
	}
	return HAL_OK;
}

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAddr      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   HAL_StatusTypeDef
*/
HAL_StatusTypeDef i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{
	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

    return HAL_I2C_Master_Receive_IT(hi2c, deviceAddr, pDataBuff, len);
}

/**
 * @brief 	Internal function to send the value of the specified i2c
 * peripheral's queue
 *
 * @param	i2c_peripheral Either I2C1, I2C1, or I2C3
 */
static void queue_send(I2C_TypeDef *i2c_peripheral)
{
	I2C_HandleTypeDef *last_hi2c = NULL;
	QueueHandle_t *I2C_Queue = NULL;
	BaseType_t higherPriorityTaskWoken = pdFALSE;

	#ifdef I2C1
	if (i2c_peripheral == I2C1)
	{
		last_hi2c = last_hi2c1;
		I2C_Queue = &I2C1_Queue;
	}
	#endif
	#ifdef I2C2
	if (i2c_peripheral == I2C2)
	{
		last_hi2c = last_hi2c2;
		I2C_Queue = &I2C2_Queue;
	}
	#endif
	#ifdef I2C3
	if (i2c_peripheral == I2C3)
	{
		last_hi2c = last_hi2c3;
		I2C_Queue = &I2C3_Queue;
	}
	#endif

	if(I2C_Queue == NULL || last_hi2c == NULL)
	{
		// Invalid I2C peripheral
		return;
	}

	if (HAL_I2C_GetState(last_hi2c) != HAL_I2C_STATE_READY)
		return;

	DataInfo_t data;
	if (xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken) == pdPASS) {
	    // Process dequeuedData
		HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, data.pDataBuff, data.len);
	} else {
	    // Handle queue empty error
	}
	portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

#ifdef I2C1
/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(last_hi2c1);
	queue_send(I2C1);
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(last_hi2c1);
	queue_send(I2C1);
}
#endif

#ifdef I2C2
/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C2_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(last_hi2c2);
	queue_send(I2C2);
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C2_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(last_hi2c2);
	queue_send(I2C2);
}
#endif

#ifdef I2C3
/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C3_EV_IRQHandler(void)
{
	HAL_I2C_EV_IRQHandler(last_hi2c3);
	queue_send(I2C3);
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C3_ER_IRQHandler(void)
{
	HAL_I2C_ER_IRQHandler(last_hi2c3);
	queue_send(I2C3);
}
#endif