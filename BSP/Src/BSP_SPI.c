#include "BSP_SPI.h"
#include "semphr.h"

#define LOCAL_BUFFER_SIZE 128
static uint16_t localTxMetaBuffer[LOCAL_BUFFER_SIZE];
static StaticQueue_t localTxMetaStatic;
static QueueHandle_t localTxMetaQueue;
static uint16_t localRxMetaBuffer[LOCAL_BUFFER_SIZE];
static StaticQueue_t localRxMetaStatic;
static QueueHandle_t localRxMetaQueue;

/**
 * @brief Initialize the provided SPI port
 * 
 * @param spiHandle The SPI handle to configure and init
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_Init(SPI_HandleTypeDef* spiHandle, QueueHandle_t* txPtr, QueueHandle_t* rxPtr) {
    HAL_StatusTypeDef stat;

    _transmitQueuePtr = txPtr;
    _receiveQueuePtr = rxPtr;
    localTxMetaQueue = xQueueCreateStatic(LOCAL_BUFFER_SIZE, sizeof(uint16_t), localTxMetaBuffer, &localTxMetaStatic);
    localRxMetaQueue = xQueueCreateStatic(LOCAL_BUFFER_SIZE, sizeof(uint16_t), localRxMetaBuffer, &localRxMetaStatic);
    // We are only allowing SPI1/3 (check this against standard config)
    if (spiHandle->Instance == SPI1) {
        if (__HAL_RCC_SPI1_IS_CLK_DISABLED()) {
            __HAL_RCC_SPI1_CLK_ENABLE();
        }
    }
    else if (spiHandle->Instance == SPI3) {
        if (__HAL_RCC_SPI1_IS_CLK_DISABLED()) {
            __HAL_RCC_SPI3_CLK_ENABLE();
        }
    }

    spiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
    spiHandle->Init.CLKPhase = SPI_PHASE_2EDGE;
    spiHandle->Init.CLKPolarity = SPI_POLARITY_HIGH; // clock idles high
    spiHandle->Init.CRCPolynomial = 0;
    spiHandle->Init.DataSize = SPI_DATASIZE_8BIT;
    spiHandle->Init.Direction = SPI_DIRECTION_2LINES; // duplex
    spiHandle->Init.FirstBit = SPI_FIRSTBIT_MSB;
    spiHandle->Init.Mode = SPI_MODE_MASTER;
    spiHandle->Init.NSS = SPI_NSS_SOFT; // software controlled slave-select (I think)
    
    stat = HAL_SPI_Init(spiHandle);
    if (IS_OK(stat)) {
        // Enable the interrupts and priorities
        if (spiHandle->Instance == SPI1) {
            HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(SPI1_IRQn);
        } else if (spiHandle->Instance == SPI3) {
            HAL_NVIC_SetPriority(SPI3_IRQn, 0, 0);
            HAL_NVIC_EnableIRQ(SPI3_IRQn);
        }
        __HAL_SPI_ENABLE(spiHandle); // manually enable the peripheral
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
    if (IS_OK(stat)) {
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
        stat = CONVERT_RETURN(HAL_SPI_Transmit_IT(spiHandle, buffer, len));
    }
    // Add to TX queue if we have room
    else if (HAL_SPI_GetState(spiHandle) == HAL_SPI_STATE_BUSY_TX && spacesLeft >= len) {
        xQueueSend(localTxMetaQueue, len, 0);
        for (uint16_t i = 0; i < len; i++) {
            xQueueSend(*_transmitQueuePtr, buffer[i], 0);
        }
        stat = HAL_BUSY;
    } else { // otherwise we didn't have enough room, so inform
        stat = HAL_TIMEOUT; // should this be error? should we add as many as we can?
    }

    return stat;
}
// SPI Interrupt callback to recursively perform writes while we have data to transmit
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
    // This executes whenever a transmit completes
    // We need this local, second queue of lengths of messages we are sending so we can
    // pop data in order appropriately.

    // If the queue isn't empty...
    uint8_t data_buf[LOCAL_BUFFER_SIZE];
    if (!xQueueIsQueueEmptyFromISR(*_transmitQueuePtr) && !xQueueIsQueueEmptyFromISR(localTxMetaQueue)) {
        uint16_t lengthToSend;
        uint8_t temp;
        xQueueReceiveFromISR(localTxMetaQueue, &lengthToSend, &temp); // get # of bytes to keep transmitting
        for (uint16_t i = 0; i < lengthToSend; i++) {
            xQueueReceiveFromISR(*_transmitQueuePtr, &(data_buf[i]), &temp);
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
        xQueueSend(localRxMetaQueue, len, 0); // push metadata
    } else {
        stat = HAL_ERROR;
    }

    return stat;
}

// When a read is complete we want to copy all data from 
// our local buffer into the RX queue the user provides
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi) {
    if (uxQueueGetQueueLength(localRxMetaQueue) > 0) {
        uint16_t toSend;
        xQueueReceiveFromISR(localRxMetaQueue, &toSend, 0);
        for (uint16_t i = 0; i < toSend; i++) {
            xQueueSendFromISR(*_receiveQueuePtr, &(spiRxTempBuffer[i]), 0);
        }
    }
}

/**
 * @brief Set's SPI chip select
 * 
 * @param spiHandle SPI handle to modify - WIP
 * @return HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_SPI_SetStateCS(SPI_HandleTypeDef* spiHandle) {

}