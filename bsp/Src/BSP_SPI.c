#include "BSP_SPI.h"

#define LOCAL_BUFFER_SIZE 128 // Modify if needed
static uint8_t localTxMetaBuffer[LOCAL_BUFFER_SIZE];
static StaticQueue_t localTxMetaStatic;
static QueueHandle_t localTxMetaQueue;
static uint8_t localRxMetaBuffer[LOCAL_BUFFER_SIZE];
static StaticQueue_t localRxMetaStatic;
static QueueHandle_t localRxMetaQueue;

static QueueHandle_t* _transmitQueuePtr;
static QueueHandle_t* _receiveQueuePtr;

/**
 * @brief Initialize the provided SPI port
 * 
 * @param spiHandle The SPI handle to configure and init
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_Init(SPI_HandleTypeDef* spiHandle, QueueHandle_t* txPtr, QueueHandle_t* rxPtr) {
    HAL_StatusTypeDef stat;

    // assign local pointers
    // and create metadata queues
    _transmitQueuePtr = txPtr;
    _receiveQueuePtr = rxPtr;
    localTxMetaQueue = xQueueCreateStatic(LOCAL_BUFFER_SIZE, sizeof(uint8_t), localTxMetaBuffer, &localTxMetaStatic);
    localRxMetaQueue = xQueueCreateStatic(LOCAL_BUFFER_SIZE, sizeof(uint8_t), localRxMetaBuffer, &localRxMetaStatic);
    
    // We are only allowing SPI2/3
    if (spiHandle->Instance == SPI2) {
        if (__HAL_RCC_SPI2_IS_CLK_DISABLED()) {
            __HAL_RCC_SPI2_CLK_ENABLE();
        }
    }
    else if (spiHandle->Instance == SPI3) {
        if (__HAL_RCC_SPI3_IS_CLK_DISABLED()) {
            __HAL_RCC_SPI3_CLK_ENABLE();
        }
    } else {
        return HAL_ERROR;
    }
    
    stat = HAL_SPI_Init(spiHandle);

    if (stat == HAL_OK) {
        // Enable the interrupts and priorities
        if (spiHandle->Instance == SPI2) {
            HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(SPI2_IRQn);
        } else if (spiHandle->Instance == SPI3) {
            HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(SPI3_IRQn);
        }
        __HAL_SPI_ENABLE(spiHandle); // manually enable the peripheral

        // Register our TX and RX interrupt callbacks
        // These two calls are important! You must also ensure the corresponding peripheral's
        // #define USE_HAL_<periph>_CALLBACKS is set to 1 in the stm32f4xx_hal_conf.h file.
        HAL_SPI_RegisterCallback(spiHandle, HAL_SPI_TX_COMPLETE_CB_ID, HAL_SPI_TxCpltCallback);
        HAL_SPI_RegisterCallback(spiHandle, HAL_SPI_RX_COMPLETE_CB_ID, HAL_SPI_RxCpltCallback);
    }

    return stat;
}

/**
 * @brief Set SPI clock speed
 * 
 * @param spiHandle The SPI handle whose clock you're configuring
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_SetClock(SPI_HandleTypeDef* spiHandle, BSP_SPI_Speed speed) {
    HAL_StatusTypeDef stat;

    __HAL_SPI_DISABLE(spiHandle); // Disable the SPI periph while we set clock
    spiHandle->Init.BaudRatePrescaler = _speedLut[speed];
    stat = HAL_SPI_Init(spiHandle);
    if (stat == HAL_OK) {
        __HAL_SPI_ENABLE(spiHandle); // Re enable if all ok
    }
    return stat;
}

/**
 * @brief Perform a SPI write
 * 
 * @param spiHandle The SPI handle to write from
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_Write(SPI_HandleTypeDef* spiHandle, uint8_t* buffer, uint16_t len) {
    HAL_StatusTypeDef stat;

    // Get TX queue stats
    uint16_t spacesLeft = uxQueueSpacesAvailable(*_transmitQueuePtr);

    // Just transmit without queue stuff if the peripheral is ready,
    // AND the queue is empty. Otherwise if the TX queue has items in it we might
    // be in some weird small period between a transmit finishing, but the ISR hasn't kicked off yet.
    if (HAL_SPI_GetState(spiHandle) == HAL_SPI_STATE_READY && spacesLeft == LOCAL_BUFFER_SIZE) {
        stat = HAL_SPI_Transmit_IT(spiHandle, buffer, len);
    }
    // Add to TX queue if we have room
    else if (HAL_SPI_GetState(spiHandle) == HAL_SPI_STATE_BUSY_TX && spacesLeft >= len) {
        xQueueSend(localTxMetaQueue, &len, 0);
        for (uint16_t i = 0; i < len; i++) {
            xQueueSend(*_transmitQueuePtr, &(buffer[i]), 0);
        }
        stat = HAL_BUSY;
    } else { // otherwise we didn't have enough room, so inform
        stat = HAL_ERROR; // should this be error? should we add as many as we can?
    }

    return stat;
}

/**
 * @brief Automatically called on transmit complete; recursively transmit
 * SPI messages while we have messages in the tx queue to send.
 * 
 * @param hspi 
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    // This executes whenever a transmit completes
    // We need this static second queue of lengths of messages we are sending so we can
    // pop data in order appropriately.

    // If the queue isn't empty...
    uint8_t data_buf[LOCAL_BUFFER_SIZE];
    if (!xQueueIsQueueEmptyFromISR(*_transmitQueuePtr) && !xQueueIsQueueEmptyFromISR(localTxMetaQueue)) {
        uint16_t lengthToSend;
        uint8_t temp;
        xQueueReceiveFromISR(localTxMetaQueue, &lengthToSend, NULL); // get # of bytes to keep transmitting
        for (uint16_t i = 0; i < lengthToSend; i++) {
            xQueueReceiveFromISR(*_transmitQueuePtr, &(data_buf[i]), NULL);
        }
        HAL_SPI_Transmit_IT(hspi, data_buf, lengthToSend); // recurse
    }
}

/**
 * @brief Perform a SPI read
 * 
 * @param spiHandle The SPI handle to read from
 * @return HAL_StatusTypeDef 
 */
static uint8_t spiRxTempBuffer[LOCAL_BUFFER_SIZE] = {0};
HAL_StatusTypeDef BSP_SPI_Read(SPI_HandleTypeDef* spiHandle, uint16_t len) {
    HAL_StatusTypeDef stat;
    
    if (len <= LOCAL_BUFFER_SIZE) {
        stat = HAL_SPI_Receive_IT(spiHandle, spiRxTempBuffer, len);
        xQueueSend(localRxMetaQueue, &len, 0); // push metadata (length)
    } else {
        stat = HAL_ERROR;
    }

    return stat;
}

/**
 * @brief Automatically executed when a SPI read is finished; copy
 * all data from the read buffer in SPI_Read() into the rxQueue passed
 * in by the init function
 * 
 * @param hspi 
 */
// This specifically needs review. I think I might have some weird logic between
// Read() and this callback.
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    uint16_t toSend;
    xQueueReceiveFromISR(localRxMetaQueue, &toSend, NULL);
    for (uint16_t i = 0; i < toSend; i++) {
        xQueueSendFromISR(*_receiveQueuePtr, &(spiRxTempBuffer[i]), 0);
    }
}

/**
 * @brief Sets SPI chip select.
 * 
 * This function needs to be updated once the leaderSOM board is finalized and SPI CS is set.
 * 
 * @param spiHandle SPI handle to modify
 * @return HAL_StatusTypeDef 
 */
void BSP_SPI_SetStateCS(SPI_HandleTypeDef* spiHandle, uint8_t val) {
    uint8_t hal_val = (val) ? GPIO_PIN_SET : GPIO_PIN_RESET;
    // BSP config only supports SPI2/SPI3
    if (spiHandle->Instance == SPI2) {
        // SPI2 NSS is on PA11, AF5
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, hal_val);
    } else if (spiHandle->Instance == SPI3) {
        // SPI3 NSS is on PA4, AF6
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, hal_val);
    }
}