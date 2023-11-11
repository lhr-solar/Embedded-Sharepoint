
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"

static StaticQueue_t DataStaticQueue;
static StaticQueue_t MetaStaticQueue;
static StaticQueue_t ReceiveStaticQueue;
static StaticQueue_t ReadMutexStatic;

#define I2C_DATA_BUFFER_SIZE (I2C_QUEUE_SIZE * 2)
#define I2C_META_BUFFER_SIZE (I2C_QUEUE_SIZE * sizeof(MetaInfo_t))
#define I2C_RECIEVE_BUFFER_SIZE (I2C_QUEUE_SIZE * sizeof(MetaInfo_t))
#define I2C_RECIEVE_RAW_SIZE (I2C_QUEUE_SIZE * 2)

static uint8_t DataBufferStorage[I2C_DATA_BUFFER_SIZE];
static uint8_t MetaBufferStorage[I2C_META_BUFFER_SIZE];
static uint8_t ReceiveBufferStorage[I2C_RECIEVE_BUFFER_SIZE];
static uint8_t ReceiveRawData[I2C_RECIEVE_RAW_SIZE];

static QueueHandle_t I2C_DataQueue;

static QueueHandle_t I2C_DataRegisterQueue;

//addresses, registers, and lengths
static QueueHandle_t I2C_MetaQueue;

static QueueHandle_t I2C_MetaRegisterQueue;

static QueueHandle_t I2C_ReceiveQueue; 

static SemaphoreHandle_t readMutex;

HAL_StatusTypeDef BSP_I2C_Init(I2C_HandleTypeDef *hi2c) {
    //QUEUES
    I2C_DataQueue = xQueueCreateStatic(I2C_DATA_BUFFER_SIZE, sizeof(uint8_t), DataBufferStorage, &DataStaticQueue);
    I2C_DataRegisterQueue = xQueueCreateStatic(I2C_DATA_BUFFER_SIZE * 2, sizeof(uint8_t), DataBufferStorage, &DataStaticQueue);
    I2C_MetaQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof(MetaInfo_t), MetaBufferStorage, &MetaStaticQueue);
    I2C_MetaRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, sizeof(MetaInfo_t), MetaBufferStorage, &MetaStaticQueue);
    I2C_ReceiveQueue = xQueueCreateStatic(I2C_RECIEVE_RAW_SIZE, sizeof(uint8_t), ReceiveBufferStorage, &ReceiveStaticQueue);
    readMutex = xSemaphoreCreateMutexStatic(&ReadMutexStatic);


    if (hi2c->Instance == I2C1) {
        if (__HAL_RCC_I2C1_CLK_DISABLE()) {
            __HAL_RCC_I2C1_CLK_ENABLE();
        }
    }
    if (hi2c->Instance == I2C3) {
        if (__HAL_RCC_I2C3_CLK_DISABLE()) {
            __HAL_RCC_I2C3_CLK_ENABLE();
        }
    }

    if (HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        HAL_RCC_GPIOC_CLK_ENABLE();
    }\

    /*
    uint32_t I2C_CLOCK_SPEED = 100000;
    // i2c config 
    i2c_struct.Init.ClockSpeed = I2C_CLOCK_SPEED;
    i2c_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
    // 7 bit addr mode
    i2c_struct.Init.OwnAddress1 = 0x50 << 1;
    i2c_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_struct.Init.GeneralCallMode = I2C_GENERALCALL_ENABLED; 
    i2c_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    */
    HAL_StatusTypeDef stat = HAL_I2C_Init(hi2c);

    if (hi2c->Instance == I2C1) {
        HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    }
    if (hi2c->Instance == I2C3) {
        HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    }

    return stat;
}

/** 
 * @brief    preform I2C write
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    pDataBuff          data buffer
 * @param    len                amount of data   
 * @return   HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint8_t* pDataBuff, 
              uint16_t len) {
    HAL_StatusTypeDef stat = HAL_OK;
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        stat = HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress << 1, pDataBuff, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataQueue) >= len) {
            
            // HAL requires 7b I2C address left shifted
            MetaInfo_t metaData = {.deviceAddr = deviceAddress << 1, .length = len};
            xQueueSend(I2C_MetaQueue, &metaData, 0);
    }
    return stat;
}

/**
 * @brief Recursively go through to see if there is any more info that needs to be transmitted,
 * if there is transmit it
 * @param hi2c                  I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (xQueueIsQueueEmptyFromISR(I2C_MetaQueue) == pdFALSE) {
        MetaInfo_t metaData;
        xQueueReceiveFromISR(I2C_MetaQueue, &metaData, 0);
        HAL_I2C_Master_Transmit_IT(hi2c, metaData.deviceAddr, metaData.pDataBuffer, metaData.length);
    }
}

/** 
 * @brief    perform I2C write accounting for registers
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    memoryAddress      the register address to write to in the IC's memory.
 * @param    memoryAddressSize  the register address size
 * @param    pDataBuff          data buffer
 * @param    len                amount of data   
 * @return   HAL_StatusTypeDef    
 */
HAL_StatusTypeDef BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint8_t memoryAddress, //register
              uint8_t memoryAddressSize,
              uint8_t* pDataBuff, 
              uint16_t len) {
    HAL_StatusTypeDef stat = HAL_OK;
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        stat = HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, 
            memoryAddressSize, pDataBuff, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataRegisterQueue) >= len) {
            // HAL requires 7b I2C address left shifted
            MetaInfo_t metaRegData = {.deviceAddr = deviceAddress << 1, 
                .memoryAddr = memoryAddress, .memoryAddrSize = memoryAddressSize, 
                .pDataBuffer = pDataBuff, .length = len};
            xQueueSend(I2C_MetaRegisterQueue, (void *) &metaRegData, 0);
    }
    return stat;
}

/**
 * @brief Recursively go through to see if there is any more info that needs to be transmitted,
 * if there is transmit it
 * @param hi2c                  I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (xQueueIsQueueEmptyFromISR(I2C_MetaRegisterQueue) == pdFALSE) {
        MetaInfo_t metaData;
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, (void *) &metaData, 0);
        HAL_I2C_Mem_Write_IT(hi2c, metaData.deviceAddr, metaData.memoryAddr, 
                metaData.memoryAddrSize, metaData.pDataBuffer, metaData.length);
    }
}

/**
 * @brief    Gets the data from a device through the I2C bus.
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    memoryAddress      the register address to write to in the IC's memory.
 * @param    memoryAddressSize  the register address size
 * @return   HAL_StatusTypeDef 
 */
static uint16_t lenToRead;
HAL_StatusTypeDef BSP_I2C_RegisterRead(I2C_HandleTypeDef* hi2c,
              uint8_t deviceAddress,
              uint8_t memoryAddress,
              uint8_t memoryAddressSize,
              uint16_t len) {
    
    xSemaphoreTake(readMutex, 10);
    lenToRead = len;
    xSemaphoreGive(readMutex);
    return HAL_I2C_Mem_Read_IT(hi2c, deviceAddress << 1, memoryAddress, memoryAddressSize, ReceiveRawData, len);
}

/**
 * @brief When a read is complete we want to copy all data from the local buffer
 *  into the RX queue the user provides
 * @param hi2c  I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint16_t temp;
    if (xSemaphoreTake(readMutex, 0) == pdTRUE) {
        temp = lenToRead;
        xSemaphoreGive(readMutex);
        for (uint8_t i = 0; i < temp; i++) {
            xQueueSendFromISR(I2C_ReceiveQueue, &(ReceiveRawData[i]), NULL);
        }
    }
}