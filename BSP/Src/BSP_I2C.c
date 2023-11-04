
#include "BSP_I2C.h"
#include "BSP.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

#define I2C_CLOCK_SPEED  100000
#define I2C_QUEUE_SIZE   64
#define I2C_ITEM_LENGTH   sizeof(uint8_t)

static StaticQueue_t __DataStaticQueue;
static StaticQueue_t __MetaStaticQueue;
static StaticQueue_t __ReceiveStaticQueue;

uint8_t __DataBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
uint8_t __MetaBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH * 2];
static uint8_t __ReceiveBufferStorage[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
static uint8_t __ReceiveRawData[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];

SemaphoreHandle_t semaphore = NULL;
StaticSemaphore_t semaphoreBuffer;

static QueueHandle_t I2C_DataQueue;

static QueueHandle_t I2C_DataRegisterQueue;

//addresses, registers, and lengths
static QueueHandle_t I2C_MetaQueue;

static QueueHandle_t I2C_MetaRegisterQueue;

static QueueHandle_t I2C_ReceiveQueue; 

HAL_StatusTypeDef BSP_I2C_Init() {
    //QUEUES
    I2C_DataQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &__DataStaticQueue);
    I2C_DataRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __DataBufferStorage, &__DataStaticQueue);
    I2C_MetaQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &__MetaStaticQueue);
    I2C_MetaRegisterQueue = xQueueCreateStatic(I2C_QUEUE_SIZE * 2, I2C_ITEM_LENGTH, __MetaBufferStorage, &__MetaStaticQueue);
    I2C_ReceiveQueue = xQueueCreateStatic(I2C_QUEUE_SIZE, I2C_ITEM_LENGTH, __ReceiveBufferStorage, &__ReceiveStaticQueue);

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
 * @param    hi2c:            struct with config data
 * @param    deviceAddress :      target device address
 * @param    pDataBuff :      data buffer
 * @param    len :           amount of data   
 * @return       
 */
void BSP_I2C_Write(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAddress, 
              uint8_t* pDataBuff, 
              uint16_t len) {
    //copy buffer data over for security incase it changes 
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
        for (uint16_t i = 0; i < len; i++) {
            buffer[i] = pDataBuff[i];
        }
    xSemaphoreTake(semaphore, 1);
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress << 1, buffer, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataQueue) >= len) {
        for (uint32_t i = 0; i < len; i++) {
            xQueueSend(I2C_DataQueue, buffer[i], 0);
        }
            // HAL requires 7b I2C address left shifted
            xQueueSend(I2C_MetaQueue, deviceAddress << 1, 0);
            xQueueSend(I2C_MetaQueue, len, 0);
    }
    xSemaphoreGive(semaphore);
}

/**
 * @brief Recursively go through to see if there is any more info that needs to be transmitted,
 * if there is transmit it
 * @param hi2c I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
    if (xQueueIsQueueEmptyFromISR(I2C_MetaQueue) == pdFALSE) {
        uint32_t deviceAddress, len;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaQueue, &deviceAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaQueue, &len, &temp);
        for (uint32_t i = 0; i < len; i++) {
            xQueueReceiveFromISR(I2C_DataQueue, &(buffer[i]), &temp);
        }
        HAL_I2C_Master_Transmit_IT(hi2c, deviceAddress, buffer, len);
    }
}

/** 
 * @param    hi2c:            struct with config data
 * @param    deviceAddress :      target device address
 * @param    memoryAddress:      the register address to write to in the IC's memory.
 * @param    memoryAddressSize :  the register address size
 * @param    pDataBuff :      data buffer
 * @param    len :            amount of data   
 * @return       
 */
void BSP_I2C_RegisterWrite(I2C_HandleTypeDef *hi2c, 
              uint32_t deviceAddress, 
              uint32_t memoryAddress, //register
              uint32_t memoryAddressSize,
              uint8_t* pDataBuff, 
              uint16_t len) {
    //copy buffer data over for security incase it changes 
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
        for (uint16_t i = 0; i < len; i++) {
            buffer[i] = pDataBuff[i];
        }
    xSemaphoreTake(semaphore, 1);
    if (!__HAL_I2C_GET_FLAG(hi2c, I2C_FLAG_BUSY)) {
        // HAL requires 7b I2C address left shifted
        HAL_I2C_Mem_Write_IT(hi2c, deviceAddress << 1, memoryAddress, memoryAddressSize, buffer, len);
    }
    else if (uxQueueSpacesAvailable(I2C_DataRegisterQueue) >= len) {
        for (uint32_t i = 0; i < len; i++) {
            xQueueSend(I2C_DataRegisterQueue, buffer[i], 0);
        }
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
 * @param hi2c I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t buffer[I2C_QUEUE_SIZE * I2C_ITEM_LENGTH];
    if (xQueueIsQueueEmptyFromISR(I2C_MetaRegisterQueue) == pdFALSE) {
        uint32_t deviceAddress, memoryAddress, memoryAddressSize, len;
        uint8_t temp;
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &deviceAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAddress, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &memoryAddressSize, &temp);
        xQueueReceiveFromISR(I2C_MetaRegisterQueue, &len, &temp);
        for (uint32_t i = 0; i < len; i++) {
            xQueueReceiveFromISR(I2C_DataRegisterQueue, &(buffer[i]), &temp);
        }
        HAL_I2C_Mem_Write_IT(hi2c, deviceAddress, memoryAddress, memoryAddressSize, buffer, len);
    }
}

/**
 * @brief   Gets the data from a device through the I2C bus.
 * @param    hi2c:            struct with config data
 * @param    deviceAddress :      target device address
 * @param    memoryAddress :      the register address to write to in the IC's memory.
 * @param    memoryAddressSize :  the register address size
 */
//Register Read
void BSP_I2C_Read(I2C_HandleTypeDef* hi2c,
              uint16_t deviceAddress,
              uint16_t memoryAddress,
              uint16_t memoryAddressSize) {
    HAL_I2C_Mem_Read_IT(hi2c, deviceAddress, memoryAddress, sizeof(uint8_t), __ReceiveRawData, I2C_QUEUE_SIZE * I2C_ITEM_LENGTH);
}

/**
 * @brief When a read is complete we want to copy all data from the local buffer
 *  into the RX queue the user provides
 * @param hi2c  I2C handle that interrupted (passed in by IRQ)
 */
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    uint8_t temp;
    for (uint8_t i = 0; i < I2C_QUEUE_SIZE * I2C_ITEM_LENGTH; i++) {
        xQueueSendFromISR(I2C_ReceiveQueue, &(__ReceiveRawData[i]), &temp);
    }
}