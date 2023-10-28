
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"
#include "queue.h"

#define I2C_CLOCK_SPEED  100000
#define I2C_QUEUE_SIZE   64
#define I2C_ITEM_LENGTH   sizeof(uint8_t)

static StaticQueue_t __DataStaticQueue;
static StaticQueue_t __MetaStaticQueue;
static StaticQueue_t __ReceiveStaticQueue;

uint8_t __DataBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
uint8_t __MetaBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH * 2];
uint8_t __ReceiveBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
uint8_t __ReceiveRawData[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];

static QueueHandle_t I2C_DataQueue;
//addresses, registers, and lengths

static QueueHandle_t I2C_DataRegisterQueue;

static QueueHandle_t I2C_MetaQueue;

static QueueHandle_t I2C_MetaRegisterQueue;

static QueueHandle_t I2C_ReceiveQueue; 

BSP_Status BSP_I2C_Init() {
    //QUEUES
    I2C_DataQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &__DataStaticQueue);
    I2C_DataRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &__DataStaticQueue);
    I2C_MetaQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &__MetaStaticQueue);
    I2C_MetaRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &__MetaStaticQueue);
    I2C_ReceiveQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __ReceiveBufferStorage, &__ReceiveStaticQueue);


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

    // i2c config NAHHH NO WAY
    i2c_struct.Init.ClockSpeed = I2C_CLOCK_SPEED;
    i2c_struct.Init.DutyCycle = I2C_DUTYCYCLE_2;
    // 7 bit addr mode
    i2c_struct.Init.OwnAddress1 = 0x50 << 1;
    i2c_struct.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    i2c_struct.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    i2c_struct.Init.GeneralCallMode = I2C_GENERALCALL_ENABLED; 
    i2c_struct.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    HAL_I2C_Init(&i2c_struct);

    i2c_struct.Instance = I2C1;
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
}

/** 
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    pDataBuff :      data buffer
* @param    len :           amount of data   
* @return   HAL Status    
*/
void BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len) {
    //copy buffer data over for security incase it changes 
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
        for (uint16_t i = 0; i < len; i++) {
            buffer[i] = pDataBuff[i];
        }
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAdd << 1, buffer, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataQueue) > len) {
        for (uint32_t i = 0; i < len; i++) {
            xQueueSend(I2C_DataQueue, buffer[i], 0);
        }
            // HAL requires 7b I2C address left shifted
            xQueueSend(I2C_MetaQueue, deviceAdd << 1, 0);
            xQueueSend(I2C_MetaQueue, len, 0);
    }
}

//overwrite this
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
    if (xQueueIsQueueEmptyFromISR(I2C_MetaQueue) == pdFALSE) {
        uint32_t deviceAdd, len;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaQueue, &deviceAdd, &temp);
        xQueueReceiveFromISR(I2C_MetaQueue, &len, &temp);
        for (uint32_t i = 0; i < len; i++) {
            xQueueReceiveFromISR(I2C_DataQueue, &(buffer[i]), &temp);
        }
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAdd, buffer, len);
    }
}

/** 
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
* @param    pDataBuff :      data buffer
* @param    len :            amount of data   
* @return   HAL Status    
*/
void BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAdd, 
              uint32_t memoryAdd, //register
              uint32_t memoryAddSize,
              uint8_t* pDataBuff, 
              uint16_t len) {
    //copy buffer data over for security incase it changes 
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
        for (uint16_t i = 0; i < len; i++) {
            buffer[i] = pDataBuff[i];
        }
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Mem_Write_IT(hi2c, deviceAdd << 1, memoryAdd, memoryAddSize, buffer, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataRegisterQueue) > len) {
        for (uint32_t i = 0; i < len; i++) {
            xQueueSend(I2C_DataRegisterQueue, buffer[i], 0);
        }
            // HAL requires 7b I2C address left shifted
            xQueueSend(I2C_MetaRegisterQueue, deviceAdd << 1, 0);
            xQueueSend(I2C_MetaRegisterQueue, memoryAdd, 0);
            xQueueSend(I2C_MetaRegisterQueue, memoryAddSize, 0);
            xQueueSend(I2C_MetaRegisterQueue, len, 0);
    }
}

//register overwrite version
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
    if (xQueueIsQueueEmptyFromISR(I2C_MetaRegisterQueue) == pdFALSE) {
        uint32_t deviceAdd, memoryAdd, memoryAddSize, len;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &deviceAdd, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAdd, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAddSize, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &len, &temp);
        for (uint32_t i = 0; i < len; i++) {
            xQueueReceiveFromISR(I2C_DataRegisterQueue, &(buffer[i]), &temp);
        }
        HAL_I2C_Mem_Write_IT(hi2c, deviceAdd, memoryAdd, memoryAddSize, buffer, len);
    }
}

/**
* @brief   Gets the data from a device through the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd :      target device address
* @param    memoryAdd :      the register address to write to in the IC's memory.
* @param    memoryAddSize :  the register address size
 */
//Register Read
void BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint16_t deviceAdd,
              uint16_t memoryAdd,
              uint16_t memoryAddSize) {
    HAL_I2C_Mem_Read_IT(hi2c, deviceAdd, memoryAdd, sizeof(uint8_t), __ReceiveRawData, I2C_QUEUE_SIZE * I2C_ITEM_LENGTH);
}

//overwrite
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t temp;
    for (uint8_t i = 0; i < I2C_QUEUE_SIZE * I2C_ITEM_LENGTH; i++) {
        xQueueSendFromISR(I2C_ReceiveQueue, &(__ReceiveRawData[i]), &temp);
    }
}