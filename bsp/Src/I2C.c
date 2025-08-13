/*
 * BSP_I2C.c
 *
 *  Created on: Nov 2, 2024
 *      Author: shaun
 */
#include "I2C.h"
#include "stm32xx_hal.h"

// Shift I2C address left by 1 bit to give space for read write bit
#define I2C_ADDR_LEFT_SHIFT(addr)   ((uint8_t)(((addr) & 0x7F) << 1))

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t deviceAddr;
	uint8_t pDataBuff;
	uint8_t payloadsLeft;
} I2C_DataInfo_t;


typedef enum {
	I2C1_index,
	#ifdef I2C2
	I2C2_index,
	#endif
	#ifdef I2C3
	I2C3_index,
	#endif
	I2C_MAX_index,
	I2C_bad_index,
} I2C_index_t;

typedef struct {
	I2C_index_t index;
	uint8_t queue_size;
	uint8_t max_payloads;
	uint8_t preempt_priority;
	uint8_t sub_priority;
	StaticSemaphore_t mutex_obj;
	SemaphoreHandle_t i2c_mutex;
} I2C_PeripheralConfig_t;

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

static uint8_t I2C1_DataStore[I2C1_QUEUE_SIZE * sizeof(I2C_DataInfo_t)];
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

static uint8_t I2C2_DataStore[I2C2_QUEUE_SIZE * sizeof(I2C_DataInfo_t)];
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

static uint8_t I2C3_DataStore[I2C3_QUEUE_SIZE * sizeof(I2C_DataInfo_t)];
static StaticQueue_t I2C3_DataQueue;
static QueueHandle_t I2C3_Queue;
I2C_HandleTypeDef *last_hi2c3;
#endif

static I2C_PeripheralConfig_t I2C_PeripheralConfigs[] = {
	// i2c_index, queue_size, max_payloads, preempt_priority, sub_priority, mutex_obj, i2c_mutex
	{ I2C1_index, I2C1_QUEUE_SIZE, I2C1_MAX_PAYLOADS, I2C1PRIO, I2C1SPRIO, {{0}}, NULL },
#ifdef I2C2
	{ I2C2_index, I2C2_QUEUE_SIZE, I2C2_MAX_PAYLOADS, I2C2PRIO, I2C2SPRIO, {{0}}, NULL },
#endif
#ifdef I2C3
	{ I2C3_index, I2C3_QUEUE_SIZE, I2C3_MAX_PAYLOADS, I2C3PRIO, I2C3SPRIO, {{0}}, NULL },
#endif
};

static I2C_index_t get_i2c_index(I2C_HandleTypeDef *hi2c) {
	if (hi2c->Instance == I2C1) {
		return I2C1_index;
	}
	#ifdef I2C2
	else if (hi2c->Instance == I2C2) {
		return I2C2_index;
	}
	#endif
	#ifdef I2C3
	else if (hi2c->Instance == I2C3) {
		return I2C3_index;
	}
	#endif
	return I2C_bad_index;
}

// Call this function from while interrupts are disabled since last_hi2c1 is used in the ISR  
static i2c_status update_last_i2c(I2C_HandleTypeDef *hi2c) {
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
 * @return  i2c_status
 */
i2c_status i2c_init(I2C_HandleTypeDef *hi2c) {
	
	I2C_index_t index = get_i2c_index(hi2c);
	I2C_PeripheralConfigs[index].i2c_mutex = xSemaphoreCreateMutexStatic(&I2C_PeripheralConfigs[index].mutex_obj);

	// Initialize the respective I2C peripheral's queue and interrupts
	if(hi2c->Instance == I2C1)
	{
		I2C1_Queue = xQueueCreateStatic(I2C1_QUEUE_SIZE, sizeof(uint8_t), I2C1_DataStore, &I2C1_DataQueue);
		// Check if the queue was created successfully
		if (I2C1_Queue == NULL) return I2C_QFAIL;
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
	else
	{
		return I2C_BADINST;
	}

	// Initialize the I2C peripheral
	return HAL_I2C_Init(hi2c) == HAL_OK ? I2C_OK : I2C_IFAIL;
}

i2c_status i2c_deinit(I2C_HandleTypeDef *hi2c) {
	if(HAL_I2C_DeInit(hi2c) != HAL_OK) return I2C_IFAIL;

	return I2C_OK;
}

__weak void HAL_I2C_GPIO_MspInit(I2C_HandleTypeDef* hi2c) {
	if(hi2c->Instance == I2C1)
	{
		// Configure I2C SCL and SDA pins
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		// Configure I2C SCL and SDA pins
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	}
	#endif
	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
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
	}
	#endif
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c) {
	IRQn_Type i2c_event_irqn = I2C1_EV_IRQn;
	IRQn_Type i2c_error_irqn = I2C1_ER_IRQn;
	uint32_t i2c_preempt_priority = I2C1PRIO;
	uint32_t i2c_sub_priority = I2C1SPRIO;
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_ENABLE();

		i2c_event_irqn = I2C1_EV_IRQn;
		i2c_error_irqn = I2C1_ER_IRQn;
		i2c_preempt_priority = I2C1PRIO;
		i2c_sub_priority = I2C1SPRIO;
	}

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		__HAL_RCC_I2C2_CLK_ENABLE();
		
		i2c_event_irqn = I2C2_EV_IRQn;
		i2c_error_irqn = I2C2_ER_IRQn;
		i2c_preempt_priority = I2C2PRIO;
		i2c_sub_priority = I2C2SPRIO;
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		__HAL_RCC_I2C3_CLK_ENABLE();

		i2c_event_irqn = I2C3_EV_IRQn;
		i2c_error_irqn = I2C3_ER_IRQn;
		i2c_preempt_priority = I2C3PRIO;
		i2c_sub_priority = I2C3SPRIO;
	}
	#endif

	HAL_I2C_GPIO_MspInit(hi2c); // Call the user defined GPIO init function

	HAL_NVIC_SetPriority(i2c_event_irqn , i2c_preempt_priority, i2c_sub_priority); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
	HAL_NVIC_EnableIRQ(i2c_event_irqn);


	HAL_NVIC_SetPriority(i2c_event_irqn , i2c_preempt_priority, i2c_sub_priority); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
	HAL_NVIC_EnableIRQ(i2c_event_irqn); // Enable the I2C interrupt
	HAL_NVIC_SetPriority(i2c_error_irqn , i2c_preempt_priority, i2c_sub_priority); // set to priority (default 5) (not the highest priority) for I2C peripheral's interrupt
	HAL_NVIC_EnableIRQ(i2c_error_irqn); // Enable the I2C interrupt
}

__weak void HAL_I2C_GPIO_MspDeInit(I2C_HandleTypeDef* hi2c) {
	if(hi2c->Instance == I2C1){
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_7);
	}
	#ifdef I2C2
	if(hi2c->Instance == I2C2){
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_10);
	}
	#endif
	#ifdef I2C3
	if(hi2c->Instance == I2C3){
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
	}
	#endif

}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c) {
	if(hi2c->Instance == I2C1)
	{
		__HAL_RCC_I2C1_CLK_DISABLE();

		HAL_NVIC_DisableIRQ(I2C1_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_DisableIRQ(I2C1_ER_IRQn); // Disable the I2C interrupt
	}

	#ifdef I2C2
	else if(hi2c->Instance == I2C2)
	{
		__HAL_RCC_I2C2_CLK_DISABLE();

		HAL_NVIC_DisableIRQ(I2C2_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_DisableIRQ(I2C2_ER_IRQn); // Disable the I2C interrupt
	}
	#endif

	#ifdef I2C3
	else if(hi2c->Instance == I2C3)
	{
		__HAL_RCC_I2C3_CLK_DISABLE();

		HAL_NVIC_DisableIRQ(I2C3_EV_IRQn); // Disable the I2C interrupt
		HAL_NVIC_DisableIRQ(I2C3_ER_IRQn); // Diable the I2C interrupt
	}
	HAL_I2C_GPIO_MspDeInit(hi2c); // Call the user defined GPIO deinit function
	#endif
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @param    delay_ticks    ticks to wait to send data
* @return   i2c_status
*/
i2c_status i2c_send_2(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len,
			  TickType_t delay_ticks)
{
	uint8_t shiftedDeviceAddr = I2C_ADDR_LEFT_SHIFT(deviceAddr);
	I2C_index_t index = get_i2c_index(hi2c);
	if(index == I2C_bad_index){
		return I2C_BADINST;
	}

	// Confirm length is not larger than its designated payload size
	if(len > I2C_PeripheralConfigs[index].max_payloads)
	{
		return I2C_BADSIZE;
	}

	// Check if someone else is using the I2C peripheral
	if(xSemaphoreTake(I2C_PeripheralConfigs[index].i2c_mutex, delay_ticks) != pdTRUE)
	{
		// Did not get the mutex in time
		return I2C_BUSY;
	}

	i2c_status ret = I2C_OK;
	// Disable interrupts to prevent i2c interrupts occuring and modifying the handle
	portENTER_CRITICAL();
	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		HAL_I2C_Master_Transmit_IT(hi2c, shiftedDeviceAddr, pDataBuff, len);
		ret = I2C_SENT;
	}
	else
	{
		ret = I2C_BUSY;
	}
	portEXIT_CRITICAL();
	return ret;
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   i2c_status
*/
i2c_status i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{
	uint8_t shiftedDeviceAddr = I2C_ADDR_LEFT_SHIFT(deviceAddr);
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

	// Disable interrupts to prevent i2c interrupts occuring and modifying the handle
	portENTER_CRITICAL();

	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

	// If I2C peripheral is ready, send data immediately
	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		HAL_I2C_Master_Transmit_IT(hi2c, shiftedDeviceAddr, pDataBuff, len);
		// Renable interrupts
		portEXIT_CRITICAL();
		return I2C_SENT;
	}

	// Renable interrupts
	portEXIT_CRITICAL();
	
	QueueHandle_t I2C_Queue = I2C1_Queue;
	if(hi2c->Instance == I2C1) 
	{
		I2C_Queue = I2C1_Queue;
	}
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
		I2C_DataInfo_t dataToEnqueue = {
			.hi2c = hi2c,
			.deviceAddr = shiftedDeviceAddr,
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

i2c_status i2c_recv_2(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len,
			  TickType_t delay_ticks)

{
	I2C_index_t index = get_i2c_index(hi2c);
	if(index == I2C_bad_index){
		return I2C_BADINST;
	}

	if(xSemaphoreTake(I2C_PeripheralConfigs[index].i2c_mutex, delay_ticks) != pdTRUE){
		// Did not get the mutex in delay_ticks
		return I2C_BUSY;
	}

	// Currently using the shared I2C peripheral, mutex will be released in callback
	i2c_status ret = I2C_OK;
	// Disable interrupts to prevent i2c interrupts occuring and modifying the handle
	portENTER_CRITICAL();
	if(HAL_I2C_GetState(hi2c) == HAL_I2C_STATE_READY)
	{
		HAL_I2C_Master_Receive_IT(hi2c, I2C_ADDR_LEFT_SHIFT(deviceAddr), pDataBuff, len);
	}
	else
	{
		ret = I2C_BUSY;
	}
	portEXIT_CRITICAL();
	return ret;

}

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAddr      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   i2c_status
*/
i2c_status i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len)
{

	// Disable and enable interrupts so modifying the handle is atomic
	portENTER_CRITICAL();
	// Update the latest I2C peripheral
	update_last_i2c(hi2c);

	portEXIT_CRITICAL();

    HAL_I2C_Master_Receive_IT(hi2c, I2C_ADDR_LEFT_SHIFT(deviceAddr), pDataBuff, len);
	return I2C_OK;
}

/**
 * @brief 	Internal function to send the value of the specified i2c
 * peripheral's queue
 *
 * @param	i2c_peripheral Either I2C1, I2C1, or I2C3
//  */
// static i2c_status transmit(I2C_TypeDef *i2c_peripheral)
// {
// 	I2C_HandleTypeDef *last_hi2c = NULL;
// 	QueueHandle_t *I2C_Queue = NULL;
// 	BaseType_t higherPriorityTaskWoken = pdFALSE;
// 	i2c_status status = I2C_OK;
// 	uint8_t max_payload_size = 0;

// 	if (i2c_peripheral == I2C1)
// 	{
// 		last_hi2c = last_hi2c1;
// 		I2C_Queue = &I2C1_Queue;
// 		max_payload_size = I2C1_MAX_PAYLOADS;
// 	}
// 	#ifdef I2C2
// 	else if (i2c_peripheral == I2C2)
// 	{
// 		last_hi2c = last_hi2c2;
// 		I2C_Queue = &I2C2_Queue;
// 		max_payload_size = I2C2_MAX_PAYLOADS;
// 	}
// 	#endif
// 	#ifdef I2C3
// 	else if (i2c_peripheral == I2C3)
// 	{
// 		last_hi2c = last_hi2c3;
// 		I2C_Queue = &I2C3_Queue;
// 		max_payload_size = I2C3_MAX_PAYLOADS;
// 	}
// 	#endif

// 	if(I2C_Queue == NULL || last_hi2c == NULL || max_payload_size == 0)
// 	{
// 		// Invalid I2C peripheral
// 		return I2C_BADINST;
// 	}

// 	if (HAL_I2C_GetState(last_hi2c) != HAL_I2C_STATE_READY)
// 		return I2C_IFAIL;

// 	I2C_DataInfo_t data;
// 	if (xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken) == pdPASS) {
// 	    // Process dequeuedData
// 		uint8_t dataToTransmit[max_payload_size];
// 		uint8_t index = 0;
// 		dataToTransmit[index] = data.pDataBuff;
// 		while(data.payloadsLeft != 0)
// 		{
// 			xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
// 			dataToTransmit[++index] = data.pDataBuff;
// 		}

// 		HAL_I2C_Master_Transmit_IT(data.hi2c, I2C_ADDR_LEFT_SHIFT(data.deviceAddr), dataToTransmit, index + 1);	
// 		if(i2c_peripheral == I2C1)
// 		{

// 		}
// 		#ifdef I2C2
// 		else if(i2c_peripheral == I2C2)
// 		{
// 			uint8_t dataToTransmit[I2C2_MAX_PAYLOADS];
// 			uint8_t index = 0;
// 			dataToTransmit[index] = data.pDataBuff;
// 			while(data.payloadsLeft != 0)
// 			{
// 				xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
// 				dataToTransmit[++index] = data.pDataBuff;
// 			}

// 			HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, dataToTransmit, index + 1);	
// 		}
// 		#endif
// 		#ifdef I2C3
// 		else if(i2c_peripheral == I2C3)
// 		{
// 			uint8_t dataToTransmit[I2C3_MAX_PAYLOADS];
// 			uint8_t index = 0;
// 			dataToTransmit[index] = data.pDataBuff;
// 			while(data.payloadsLeft != 0)
// 			{
// 				xQueueReceiveFromISR(*I2C_Queue, &data, &higherPriorityTaskWoken);
// 				dataToTransmit[++index] = data.pDataBuff;
// 			}

// 			HAL_I2C_Master_Transmit_IT(data.hi2c, data.deviceAddr, dataToTransmit, index + 1);	
// 		}
// 		#endif
		
// 		status = I2C_BADINST;
// 		// Control should not reach here as only valid I2C should be able to enter the queue
// 	}
// 	portYIELD_FROM_ISR(higherPriorityTaskWoken);
// 	return status;
// }

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
	I2C_index_t index = get_i2c_index(hi2c);
	if(index == I2C_bad_index){
		return;
	}
	BaseType_t higherPriorityTaskWoken = pdFALSE;

	// Release the mutex for the I2C peripheral
	xSemaphoreGiveFromISR(I2C_PeripheralConfigs[index].i2c_mutex, &higherPriorityTaskWoken);

	// Signal a task that is waiting for the I2C peripheral to be free
	portYIELD_FROM_ISR(higherPriorityTaskWoken);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c){
	I2C_index_t index = get_i2c_index(hi2c);
	if(index == I2C_bad_index){
		return;
	}
	BaseType_t higherPriorityTaskWoken = pdFALSE;

	// Release the mutex for the I2C peripheral
	xSemaphoreGiveFromISR(I2C_PeripheralConfigs[index].i2c_mutex, &higherPriorityTaskWoken);
}

// /**
//   * @brief This function handles I2C1 event interrupt.
//   */
// void I2C1_EV_IRQHandler(void)
// {
// 	HAL_I2C_EV_IRQHandler(last_hi2c1);
// 	transmit(I2C1);
// }

// /**
//   * @brief This function handles I2C1 error interrupt.
//   */
// void I2C1_ER_IRQHandler(void)
// {
// 	HAL_I2C_ER_IRQHandler(last_hi2c1);
// 	transmit(I2C1);
// }

// #ifdef I2C2
// /**
//   * @brief This function handles I2C1 event interrupt.
//   */
// void I2C2_EV_IRQHandler(void)
// {
// 	HAL_I2C_EV_IRQHandler(last_hi2c2);
// 	transmit(I2C2);
// }

// /**
//   * @brief This function handles I2C1 error interrupt.
//   */
// void I2C2_ER_IRQHandler(void)
// {
// 	HAL_I2C_ER_IRQHandler(last_hi2c2);
// 	transmit(I2C2);
// }
// #endif

// #ifdef I2C3
// /**
//   * @brief This function handles I2C1 event interrupt.
//   */
// void I2C3_EV_IRQHandler(void)
// {
// 	HAL_I2C_EV_IRQHandler(last_hi2c3);
// 	transmit(I2C3);
// }

// /**
//   * @brief This function handles I2C1 error interrupt.
//   */
// void I2C3_ER_IRQHandler(void)
// {
// 	HAL_I2C_ER_IRQHandler(last_hi2c3);
// 	transmit(I2C3);
// }
// #endif