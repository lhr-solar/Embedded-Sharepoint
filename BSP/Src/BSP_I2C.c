
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"

#define I2C_ITEM_LENGTH   sizeof(uint8_t)

static StaticQueue_t DataStaticQueue;
static StaticQueue_t MetaStaticQueue;
static StaticQueue_t ReceiveStaticQueue;

uint8_t __DataBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
uint8_t __MetaBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH * 2];
static uint8_t ReceiveBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
static uint8_t ReceiveRawData[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];

SemaphoreHandle_t semaphore = NULL;
StaticSemaphore_t semaphoreBuffer;

static QueueHandle_t I2C_DataQueue;

static QueueHandle_t I2C_DataRegisterQueue;

//addresses, registers, and lengths
static QueueHandle_t I2C_MetaQueue;

static QueueHandle_t I2C_MetaRegisterQueue;

static QueueHandle_t I2C_ReceiveQueue; 

HAL_StatusTypeDef BSP_I2C_Init() {
    //QUEUEs
    I2C_DataQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &DataStaticQueue);
    I2C_DataRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &DataStaticQueue);
    I2C_MetaQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &MetaStaticQueue);
    I2C_MetaRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &MetaStaticQueue);
    I2C_ReceiveQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, ReceiveBufferStorage, &ReceiveStaticQueue);

    semaphore = xSemaphoreCreateBinaryStatic(&semaphoreBuffer);

    GPIO_InitTypeDef gpio_struct;
    I2C_HandleTypeDef i2c_struct;

    if (__HAL_RCC_I2C3_IS_CLK_DISABLED()) {
        __HAL_RCC_I2C3_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOA_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
    }
    if (__HAL_RCC_GPIOC_IS_CLK_DISABLED()) {
        __HAL_RCC_GPIOC_CLK_ENABLE();
    }

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
    HAL_StatusTypeDef stat = HAL_I2C_Init(&i2c_struct);

    i2c_struct.Instance = I2C1;
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);

    return stat;
}

/** 
 * @brief    preform I2C write
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    pDataBuff          data buffer
 * @param    len                amount of data   
 * @return   void
 */
bool BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint8_t* pDataBuff, 
              uint16_t len) {
    xSemaphoreTake(semaphore, 1);
    struct metaInfo meta;
    struct metaInfo len;
    meta.deviceAddr = deviceAddress; 
    meta.length = len;
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress << 1, *pDataBuff, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataQueue) >= len) {
            //not protected
                // xQueueSend(I2C_MetaQueue, deviceAddress << 1, 0);
                // xQueueSend(I2C_MetaQueue, len, 0);

            // HAL requires 7b I2C address left shifted
            xQueueSend(I2C_MetaQueue, meta.deviceAddr << 1, 0);
            xQueueSend(I2C_MetaQueue, meta.length, 0);
    }
    xSemaphoreGive(semaphore);
}

/**
 * @brief Recursively go through to see if there is any more info that needs to be transmitted,
 * if there is transmit it
 * @param hi2c                  I2C handle that interrupted (passed in by IRQ)
 */
bool HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (xQueueIsQueueEmptyFromISR(I2C_MetaQueue) == pdFALSE) {
        uint8_t deviceAddress, len;
        uint8_t* pDataBuff;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaQueue, &deviceAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaQueue, &len, &temp);
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress << 1, *pDataBuff, len);
    }
}

/** 
 * @brief    preform I2C write accounting for registers
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    memoryAddress      the register address to write to in the IC's memory.
 * @param    memoryAddressSize  the register address size
 * @param    pDataBuff          data buffer
 * @param    len                amount of data   
 * @return   void    
 */
bool BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint32_t memoryAddress, //register
              uint32_t memoryAddressSize,
              uint8_t* pDataBuff, 
              uint16_t len) {
    xSemaphoreTake(semaphore, 1);
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, memoryAddressSize, *pDataBuff, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataRegisterQueue) >= len) {
            // HAL requires 7b I2C address left shifted
            xQueueSend(I2C_MetaRegisterQueue, deviceAddress << 1, 0);
            xQueueSend(I2C_MetaRegisterQueue, memoryAddress, 0);
            xQueueSend(I2C_MetaRegisterQueue, memoryAddressSize, 0);
            xQueueSend(I2C_MetaRegisterQueue, len, 0);
    }
    xSemaphoreGive(semaphore);
}

/**
 * @brief Recursively go through to see if there is any more info that needs to be transmitted,
 * if there is transmit it
 * @param hi2c                  I2C handle that interrupted (passed in by IRQ)
 */
bool HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (xQueueIsQueueEmptyFromISR(I2C_MetaRegisterQueue) == pdFALSE) {
        uint8_t deviceAddress, memoryAddress, memoryAddressSize, len;
        uint8_t* pDataBuff;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &deviceAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAddressSize, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &len, &temp);
        HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, memoryAddressSize, *pDataBuff, len);
    }
}

/**
 * @brief    Gets the data from a device through the I2C bus.
 * @param    hi2c               struct with config data
 * @param    deviceAddress      target device address
 * @param    memoryAddress      the register address to write to in the IC's memory.
 * @param    memoryAddressSize  the register address size
 * @return   void
 */
//Register Read
void BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint8_t deviceAddress,
              uint16_t memoryAddress,
              uint16_t memoryAddressSize) {
    HAL_I2C_Mem_Read_IT(hi2c, deviceAddress << 1, memoryAddress, sizeof(uint8_t), ReceiveRawData, I2C_QUEUE_SIZE * I2C_ITEM_LENGTH);
}

/**
 * @brief When a read is complete we want to copy all data from the local buffer
 *  into the RX queue the user provides
 * @param hi2c  I2C handle that interrupted (passed in by IRQ)
 */
bool HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t temp;
    for (uint32_t i = 0; i < I2C_QUEUE_SIZE * I2C_ITEM_LENGTH; i++) {
        xQueueSendFromISR(I2C_ReceiveQueue, &(ReceiveRawData[i]), &temp);
    }
}