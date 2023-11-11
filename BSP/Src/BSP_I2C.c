
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"

static StaticQueue_t DataStaticQueue;
static StaticQueue_t MetaStaticQueue;
static StaticQueue_t ReceiveStaticQueue;

uint8_t __DataBufferStorage[I2C_QUEUE_SIZE * sizeof(MetaInfo_t)];
uint8_t __MetaBufferStorage[I2C_QUEUE_SIZE * sizeof(MetaInfo_t) * 2];
static uint8_t ReceiveBufferStorage[I2C_QUEUE_SIZE * sizeof(MetaInfo_t)];
static uint8_t ReceiveRawData[I2C_QUEUE_SIZE * sizeof(MetaInfo_t)];

static QueueHandle_t I2C_DataQueue;

static QueueHandle_t I2C_DataRegisterQueue;

//addresses, registers, and lengths
static QueueHandle_t I2C_MetaQueue;

static QueueHandle_t I2C_MetaRegisterQueue;

static QueueHandle_t I2C_ReceiveQueue; 

HAL_StatusTypeDef BSP_I2C_Init() {
    //QUEUES
    I2C_DataQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof(MetaInfo_t), __DataBufferStorage, &DataStaticQueue);
    I2C_DataRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof(MetaInfo_t), __DataBufferStorage, &DataStaticQueue);
    I2C_MetaQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, sizeof(MetaInfo_t), __MetaBufferStorage, &MetaStaticQueue);
    I2C_MetaRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, sizeof(MetaInfo_t), __MetaBufferStorage, &MetaStaticQueue);
    I2C_ReceiveQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, sizeof(MetaInfo_t), ReceiveBufferStorage, &ReceiveStaticQueue);

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
 * @return   HAL_StatusTypeDef 
 */
HAL_StatusTypeDef BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint8_t* pDataBuff, 
              uint16_t len) {
    HAL_StatusTypeDef stat;
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        stat = HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress << 1, *pDataBuff, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataQueue) >= len) {
            
            // HAL requires 7b I2C address left shifted
            MetaInfo_t metaData = {.deviceAddr = deviceAddress << 1, .length = len};
            xQueueSend(I2C_MetaQueue, (void *) &metaData, 0);
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
        uint8_t deviceAddress, temp;
        uint8_t* pDataBuff;
        uint16_t len;

        MetaInfo_t metaData = {.deviceAddr = deviceAddress << 1, .pDataBuffer = pDataBuff, 
            .length = len, .temp = temp};
        xQueueReceiveFromISR(I2C_MetaQueue, (void *) &metaData, 0);
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
 * @return   HAL_StatusTypeDef    
 */
HAL_StatusTypeDef BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAddress, 
              uint8_t memoryAddress, //register
              uint8_t memoryAddressSize,
              uint8_t* pDataBuff, 
              uint16_t len) {
    HAL_StatusTypeDef stat;
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        stat = HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, 
            memoryAddressSize, *pDataBuff, len);
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
        uint8_t deviceAddress, memoryAddress, memoryAddressSize, temp;
        uint8_t* pDataBuff;
        uint16_t len;

        MetaInfo_t metaData = {.deviceAddr = deviceAddress << 1, .memoryAddr = memoryAddress, 
            .memoryAddrSize = memoryAddressSize, .pDataBuffer = pDataBuff, 
            .length = len, .temp = temp};
        xQueueReceiveFromISR(I2C_MetaQueue, (void *) &metaData, 0);
        HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, memoryAddressSize, *pDataBuff, len);
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
//Register Read
HAL_StatusTypeDef BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint8_t deviceAddress,
              uint8_t memoryAddress,
              uint8_t memoryAddressSize) {
    return HAL_I2C_Mem_Read_IT(hi2c, deviceAddress << 1, memoryAddress, sizeof(uint8_t), ReceiveRawData, I2C_QUEUE_SIZE * sizeof(MetaInfo_t));
}

/**
 * @brief When a read is complete we want to copy all data from the local buffer
 *  into the RX queue the user provides
 * @param hi2c  I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t temp;
    for (uint32_t i = 0; i < sizeof(MetaInfo_t); i++) {
        xQueueSendFromISR(I2C_ReceiveQueue, &(ReceiveRawData[i]), &temp);
    }
}