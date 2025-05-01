/*
 * BSP_I2C.c
 *
 *  Created on: Nov 2, 2024
 *      Author: shaun
 */
#include "I2C.h"
#include "stm32xx_hal.h"

/* Used Functions */
void Single_I2C_Init(I2C_HandleTypeDef*);


typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t deviceAddr;
	uint8_t pDataBuff;
	uint8_t payloadsLeft;
} DataInfo_t;

// Define queue sizes for each I2C peripheral if not done so already
#ifdef I2C1
#ifndef I2C1_QUEUE_SIZE
#define I2C1_QUEUE_SIZE 128
#endif

#ifndef I2C1_MAX_PAYLOADS
#define I2C1_MAX_PAYLOADS 16
#endif 

#ifndef I2C1PRIO
#define I2C1PRIO 5
#endif 

#ifndef I2C1SPRIO
#define I2C1SPRIO 0
#endif

static uint8_t I2C1_DataStore[I2C1_QUEUE_SIZE * sizeof(DataInfo_t)];
static QueueHandle_t I2C1_Queue;
static StaticQueue_t I2C1_DataQueue;
I2C_HandleTypeDef *last_hi2c1;
#endif

#ifdef I2C2
#ifndef I2C2_QUEUE_SIZE 
#define I2C2_QUEUE_SIZE 128
#endif

#ifndef I2C2_MAX_PAYLOADS
#define I2C2_MAX_PAYLOADS 16
#endif

#ifndef I2C2PRIO
#define I2C2PRIO 5
#endif 

#ifndef I2C2SPRIO
#define I2C2SPRIO 0
#endif

static uint8_t I2C2_DataStore[I2C2_QUEUE_SIZE * sizeof(DataInfo_t)];
static StaticQueue_t I2C2_DataQueue;
static QueueHandle_t I2C2_Queue;
I2C_HandleTypeDef *last_hi2c2;
#endif

#ifdef I2C3
#ifndef I2C3_QUEUE_SIZE 
#define I2C3_QUEUE_SIZE 128
#endif

#ifndef I2C3_MAX_PAYLOADS
#define I2C3_MAX_PAYLOADS 16
#endif

#ifndef I2C3PRIO
#define I2C3PRIO 5
#endif 

#ifndef I2C3SPRIO
#define I2C3SPRIO 0
#endif

static uint8_t I2C3_DataStore[I2C3_QUEUE_SIZE * sizeof(DataInfo_t)];
static StaticQueue_t I2C3_DataQueue;
static QueueHandle_t I2C3_Queue;
I2C_HandleTypeDef *last_hi2c3;
#endif

#define I2C_ConvertHALStatus(hal_status) ((I2C_StatusTypeDef)((hal_status) + I2C_HAL_OFFSET))

static I2C_StatusTypeDef update_last_i2c(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C1) {
		last_hi2c1 = hi2c;
		return I2C_OK;
	}
	#ifdef I2C2
	else if (hi2c->Instance == I2C2) {
		last_hi2c2 = hi2c;
		return I2C_OK;
	}
	#endif
	#ifdef I2C3
	else if (hi2c->Instance == I2C3) {
		last_hi2c3 = hi2c;
		return I2C_OK;
	}
	#endif
	
	// Handle invalid I2C peripheral under here
	return I2C_BADINST;
}

/**
 * @brief   Sets up the I2C ports as described by the user
 * @return  I2C_StatusTypeDef
 */
I2C_StatusTypeDef i2c_init(I2C_HandleTypeDef *hi2c) {

	// Initialize the respective I2C peripheral's queue and interrupts
	if(hi2c->Instance == I2C1)
	{
		I2C1_Queue = xQueueCreateStatic(I2C1_QUEUE_SIZE, sizeof(uint8_t), I2C1_DataStore, &I2C1_DataQueue);
		// Check if the queue was created successfully
		if (I2C1_Queue == NULL) return I2C_QFAIL;
		// Initialize the I2C peripheral
		if (HAL_I2C_Init(hi2c) != HAL_OK) return I2C_IFAIL;

		return I2C_OK;
	}

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		I2C2_Queue = xQueueCreateStatic(I2C2_QUEUE_SIZE, sizeof(uint8_t), I2C2_DataStore, &I2C2_DataQueue);
		// Check if the queue was created successfully
		if (I2C2_Queue == NULL) return I2C_QFAIL;
		// Initialize the I2C peripheral
		if (HAL_I2C_Init(hi2c) != HAL_OK) return I2C_IFAIL;

		return I2C_OK;
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		I2C3_Queue = xQueueCreateStatic(I2C3_QUEUE_SIZE, sizeof(uint8_t), I2C3_DataStore, &I2C3_DataQueue);
		// Check if the queue was created successfully
		if (I2C3_Queue == NULL) return I2C_QFAIL;
		// Initialize the I2C peripheral
		if (HAL_I2C_Init(hi2c) != HAL_OK) return I2C_IFAIL;

		return I2C_OK;
	}
	#endif

	return I2C_BADINST;
}

I2C_StatusTypeDef i2c_deinit(I2C_HandleTypeDef *hi2c) {
	if(HAL_I2C_DeInit(hi2c) != HAL_OK) return I2C_IFAIL;

	return I2C_OK;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c) {
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_ENABLE();

		// Configure I2C SCL and SDA pins
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(I2C1_EV_IRQn , I2C1PRIO, I2C1SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C1_ER_IRQn , I2C1PRIO, I2C1SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn); // Enable the I2C interrupt
		
	}

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		__HAL_RCC_I2C2_CLK_ENABLE();

		// Configure I2C SCL and SDA pins
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(I2C2_EV_IRQn , I2C2PRIO, I2C2SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C2_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C2_ER_IRQn , I2C2PRIO, I2C2SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C2_ER_IRQn); // Enable the I2C interrupt
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		__HAL_RCC_I2C3_CLK_ENABLE();

		// Configure I2C SCL and SDA pins
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(I2C3_EV_IRQn , I2C3PRIO, I2C3SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C3_EV_IRQn); // Enable the I2C interrupt
		HAL_NVIC_SetPriority(I2C3_ER_IRQn , I2C3PRIO, I2C3SPRIO); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
		HAL_NVIC_EnableIRQ(I2C3_ER_IRQn); // Enable the I2C interrupt
	}
	#endif
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c) {
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);

		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn); // Disable the I2C interrupt
		
	}

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		__HAL_RCC_I2C2_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_10);

		HAL_NVIC_DisableIRQ(I2C2_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_DisableIRQ(I2C2_ER_IRQn); // Disable the I2C interrupt
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		__HAL_RCC_I2C3_CLK_DISABLE();

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);

		HAL_NVIC_DisableIRQ(I2C3_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_DisableIRQ(I2C3_ER_IRQn); // Diable the I2C interrupt
	}
	#endif
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   I2C_StatusTypeDef
*/
I2C_StatusTypeDef i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{
	// Confirm length is not larger than its designated payload size
	if(hi2c->Instance == I2C1 && len > I2C1_MAX_PAYLOADS)
	{
		// ERROR
		return I2C_BADSIZE;
	}
	#ifdef I2C2
	else if(hi2c->Instance == I2C2 && len > I2C2_MAX_PAYLOADS)
	{
		// ERROR
		return I2C_BADSIZE;
	}
	#endif
	#ifdef I2C3
	else if(hi2c->Instance == I2C3 && len > I2C3_MAX_PAYLOADS)
	{
		// ERROR
		return I2C_BADSIZE;
	}
	#endif

	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		return I2C_ConvertHALStatus(HAL_I2C_Master_Transmit_IT(hi2c, deviceAddr, pDataBuff, len));
	}
	
	QueueHandle_t I2C_Queue = I2C1_Queue;
	if(hi2c->Instance == I2C1) {}
	#ifdef I2C2
	else if (hi2c->Instance == I2C2)
	{
		I2C_Queue = I2C2_Queue;
	}
	#endif
	#ifdef I2C3
	else if (hi2c->Instance == I2C3)
	{
		I2C_Queue = I2C3_Queue;
	}
	#endif
	else
	{
		// Handle invalid I2C peripheral
		return I2C_BADINST;
	}

	if (len > uxQueueSpacesAvailable(I2C_Queue))
	{
		return I2C_QFULL;
	}
	
	for(int packetsLeft = len - 1; packetsLeft >= 0; packetsLeft--)
	{
		DataInfo_t dataToEnqueue = {
			.hi2c = hi2c,
			.deviceAddr = deviceAddr,
			.pDataBuff = pDataBuff[len - packetsLeft - 1],
			.payloadsLeft = packetsLeft
		};

		if (xQueueSend(I2C_Queue, &dataToEnqueue, 0) != pdPASS)
		{
			// Handle queue full error
			return I2C_QFULL;
		}
	}

	return I2C_SENT;
}

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAddr      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   I2C_StatusTypeDef
*/
I2C_StatusTypeDef i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{
	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

    return I2C_ConvertHALStatus(HAL_I2C_Master_Receive_IT(hi2c, deviceAddr, pDataBuff, len));
}

/**
 * @brief 	Internal function to send the value of the specified i2c
 * peripheral's queue
 *
 * @param	i2c_peripheral Either I2C1, I2C1, or I2C3
 */
static I2C_StatusTypeDef queue_send(I2C_TypeDef *i2c_peripheral)
{
	I2C_HandleTypeDef *last_hi2c = NULL;
	QueueHandle_t *I2C_Queue = NULL;
	BaseType_t higherPriorityTaskWoken = pdFALSE;
	I2C_StatusTypeDef status = I2C_OK;

	if (i2c_peripheral == I2C1)
	{
		last_hi2c = last_hi2c1;
		I2C_Queue = &I2C1_Queue;
	}
	#ifdef I2C2
	else if (i2c_peripheral == I2C2)
	{
		last_hi2c = last_hi2c2;
		I2C_Queue = &I2C2_Queue;
	}
	#endif
	#ifdef I2C3
	else if (i2c_peripheral == I2C3)
	{
		last_hi2c = last_hi2c3;
		I2C_Queue = &I2C3_Queue;
	}
	#endif

	if(I2C_Queue == NULL || last_hi2c == NULL)
	{
		// Invalid I2C peripheral
		return I2C_BADINST;
	}

	if (HAL_I2C_GetState(last_hi2c) != HAL_I2C_STATE_READY)
		return I2C_IFAIL;

	DataInfo_t data;
	if (xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken) == pdPASS) {
	    // Process dequeuedData
		if(i2c_peripheral == I2C1)
		{
			uint8_t dataToTransmit[I2C1_MAX_PAYLOADS];
			uint8_t index = 0;
			dataToTransmit[index] = data.pDataBuff;
			while(data.payloadsLeft != 0)
			{
				xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
				dataToTransmit[++index] = data.pDataBuff;
			}

			HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, dataToTransmit, index + 1);	
		}
		#ifdef I2C2
		else if(i2c_peripheral == I2C2)
		{
			uint8_t dataToTransmit[I2C2_MAX_PAYLOADS];
			uint8_t index = 0;
			dataToTransmit[index] = data.pDataBuff;
			while(data.payloadsLeft != 0)
			{
				xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
				dataToTransmit[++index] = data.pDataBuff;
			}

			HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, dataToTransmit, index + 1);	
		}
		#endif
		#ifdef I2C3
		else if(i2c_peripheral == I2C3)
		{
			uint8_t dataToTransmit[I2C3_MAX_PAYLOADS];
			uint8_t index = 0;
			dataToTransmit[index] = data.pDataBuff;
			while(data.payloadsLeft != 0)
			{
				xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
				dataToTransmit[++index] = data.pDataBuff;
			}

			HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, dataToTransmit, index + 1);	
		}
		#endif
		
		status = I2C_BADINST;
		// Control should not reach here as only valid I2C should be able to enter the queue
	}
	portYIELD_FROM_ISR(higherPriorityTaskWoken);
	return status;
}

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