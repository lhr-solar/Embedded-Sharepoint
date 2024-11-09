	/*
	 * BSP_UART.c
	 *
	 *  Created on: Nov 2, 2024
	 *      Author: jimfa
	 */


	#include "BSP_UART.h"

	/* The queue is to be created to hold a maximum of 128 UARTMessages */
	#define QUEUE_LENGTH 128
	#define QUEUE_ITEM_SIZE sizeof(uint8_t)

	/* The variable used to old the queue's data structure. */
	static StaticQueue_t xStaticTxQueue;

	/* The array to use as the queue's storage area. */
	static uint8_t ucTxQueueStorageArea[ QUEUE_LENGTH * QUEUE_ITEM_SIZE ];

	/* Handle for the TX Queue*/
	static QueueHandle_t xTxQueue;

	/* Pointer to the RX Queue Handle (provided by the user) */
	static QueueHandle_t *xRxQueuePtr;

	/* UART handle*/
	static UART_HandleTypeDef *huartPtr;



	/**
	 * @brief Initializes UART peripheral and creates TX queue
	 * @param huart pointer to the UART handle
	 * @param rxQueue pointer to the receive queue (provided by user)
	 * @return HAL status
	 * @note TX pointer is not an argument passed into init since TX queue is within BSP layer, not user-layer like with RX Queue
	 */
	HAL_StatusTypeDef BSP_UART_Init(UART_HandleTypeDef *huart, QueueHandle_t *rxQueue) {
		HAL_StatusTypeDef status;

		huartPtr = huart; // set the pointer to the UART handle
		xRxQueuePtr = rxQueue; // set the pointer to the receive queue

		// create the TX queue
		xTxQueue = xQueueCreateStatic( QUEUE_LENGTH, QUEUE_ITEM_SIZE, ucTxQueueStorageArea, &xStaticTxQueue );
		//configASSERT( xTxQueuePtr ); // Check if the queue was created successfully

		status = HAL_UART_Init(huart);
		if (status != HAL_OK) {
			return status;
		}

		// Only needed if register callbacks is defined to be active, else weak callbacks are used
		// Make sure that USE_HAL_UART_REGISTER_CALLBACKS is set to 1 in the stm32f4xx_hal_conf.h file, this enables us to use the HAL_UART_RegisterCallback functions
		// Register TX and RX interrupt callbacks
//		status = HAL_UART_RegisterCallback(huart, HAL_UART_TX_COMPLETE_CB_ID, HAL_UART_TxCpltCallback);
//		if (status != HAL_OK) {
//			return status;
//		}
//		status = HAL_UART_RegisterCallback(huart, HAL_UART_RX_COMPLETE_CB_ID, HAL_UART_RxCpltCallback);
//		if (status != HAL_OK) {
//			return status;
//		}
//		status = HAL_UART_RegisterCallback(huart, HAL_UART_ERROR_CB_ID, HAL_UART_ErrorCallback);
//		if (status != HAL_OK) {
//			return status;
//		}


		// Enable interrupts and set priorities
		HAL_NVIC_SetPriority(UART4_IRQn, 5, 0); // set to priority 5 (not the highest priority) for USART1 peripheral's interrupt
		HAL_NVIC_EnableIRQ(UART4_IRQn); // Enable the UART1 interrupt

		// Start first reception
//		status = HAL_UART_Receive_IT(huart, rxBuffer, 1);

		return status; // Should return HAL_OK if everything was successful
	}

	/**
	 * @brief Perform a UART write to the specified UART device
	 * @param data pointer to the data that will be written
	 * @param length number of bytes to send
	 * @param busId the bus ID (currently unused)
	 */
	HAL_StatusTypeDef BSP_UART_Write(char *data, uint8_t length, uint8_t busId) {
		// void(busId); // Currently unused

		if (data == NULL || length == 0) {
			return HAL_ERROR;
		}

		// Check if we can send immediately
		if (HAL_UART_GetState(huartPtr) == HAL_UART_STATE_READY && uxQueueMessagesWaiting(xTxQueue) == 0) {
			return HAL_UART_Transmit_IT(huartPtr, (uint8_t*)data, length); // will return either HAL_ERROR, HAL_OK or HAL_BUSY
		}

		// Otherwise, add to Transmit queue
		for (uint8_t i = 0; i < length; i++) {
			if (xQueueSendToBack(xTxQueue, &data[i], 0) != pdPASS) {
				return HAL_BUSY; // If unable to send, then Queue is full
			}
		}

		// If transmission is not already ongoing, then start transmitting it
		if (HAL_UART_GetState(huartPtr) != HAL_UART_STATE_BUSY_TX) {
			uint8_t byte;
			if (xQueueReceive(xTxQueue, &byte, 0) == pdTRUE) { // get the first byte from the queue
				return HAL_UART_Transmit_IT(huartPtr, &byte, 1); // Once this is done, this will trigger the TX complete callback, which will then send the next byte
			}
		}

		return HAL_OK; // Return OK if the data was successfully added to the queue
	}
	/**
	 * @brief Read data from the RX queue
	 * @param data buffer to store the received data
	 * @param length of number of bytes to read
	 * @param busId the bus ID (currently unused)
	 * @return HAL status
	 */
	HAL_StatusTypeDef BSP_UART_Read(char* data, uint8_t length, uint8_t busId) {
		// void (busId); // Currently unused

		if ( data == NULL || length == 0 || xRxQueuePtr == NULL) {
			return HAL_ERROR;
		}

		// If not already receiving, then start receiving it
		if (HAL_UART_GetState(huartPtr) != HAL_UART_STATE_BUSY_RX) {
			HAL_UART_Receive_IT(huartPtr, (uint8_t*)huartPtr->pRxBuffPtr, 1);
		}

		// Read the data from the user's receive queue and store it in the data buffer
		for (uint8_t i = 0; i < length; i++) {
			if (xQueueReceive(*xRxQueuePtr, &data[i], 0) != pdPASS) { // Puts the received byte from xRxQueuePtr into data[i]
				return HAL_BUSY; // Return busy if the queue is empty
			}
		}

		return HAL_OK; // Return OK if the queue is not empty
	}


	/**
	 * @brief UART TX Complete callback Interrupts whenever a UART tranmission completes
	 * @param huart pointer to the UART handle
	 */
	void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
		uint8_t byteToTransmit;
		// BaseType_t xHigherPriorityTaskWoken = pdFALSE; // Used to determine if a context switch is required if a higher priority task is woken during the ISR


		if (xQueueReceiveFromISR(xTxQueue, &byteToTransmit, 0) == pdTRUE) { // Substitute &xHigherPriorityTaskWoken with 0 for now, indicating that no context switch is required
			HAL_UART_Transmit_IT(huart, &byteToTransmit, 1);
		}

		// portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // If a higher priority task is woken, then a context switch is required
	}

	/**
	 * @brief UART RX Complete callback Interrupts whenever a UART reception completes
	 * @param huart pointer to the UART handle
	 */
	void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
		uint8_t byteToReceive = huart->pRxBuffPtr[0]; // Get the received byte from the UART peripheral and store it in byteToReceive to then send to the receive queue
		// uint8_t byteToReceive = huart->Instance->DR; // Not sure if this would be better since it would theoretically directly read the data register, which is faster/more reliable?
		// BaseType_t xHigherPriorityTaskWoken = pdFALSE; // Used to determine if a context switch is required if a higher priority task is woken during the ISR

		xQueueSendFromISR(*xRxQueuePtr, &byteToReceive, 0); // Send the received byte to the user's receive queue, substitute &xHigherPriorityTaskWoken with 0 for now, indicating that no context switch is required

		HAL_UART_Receive_IT(huart, (uint8_t*)huart->pRxBuffPtr, 1); // Start the next reception for the UART peripheral with size 1

		// portYIELD_FROM_ISR(xHigherPriorityTaskWoken); // If a higher priority task is woken, then a context switch is required

	}



	/**
	 * @brief UART Error callback Interrupts whenever a UART error occurs
	 * @param huart pointer to the UART handle
	 */
	void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
		// UART Error handling code here
		// For now, just restart the receive process
		HAL_UART_Receive_IT(huart, (uint8_t*)huart->pRxBuffPtr, 1);
	}
