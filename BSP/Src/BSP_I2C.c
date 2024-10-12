#include "../inc/BSP_I2C.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "stdbool.h"


typedef struct {
    uint8_t deviceAddr;
    uint8_t memoryAddr;
    uint8_t memoryAddrSize;
    uint8_t* pDataBuffer;
    uint16_t length;
    uint8_t temp;
} MetaInfo_t;

typedef struct {
    uint16_t devID;
    uint8_t *data;
    uint16_t size;
} DataPacket_t;

static StaticQueue_t DataStaticQueue;
static StaticQueue_t MetaStaticQueue;
static StaticQueue_t ReceiveStaticQueue;
static StaticQueue_t ReadMutexStatic;

#define I2C_QUEUE_SIZE   128
#define I2C_DATA_BUFFER_SIZE (I2C_QUEUE_SIZE * 2)
#define I2C_META_BUFFER_SIZE (I2C_QUEUE_SIZE * sizeof(MetaInfo_t))
#define I2C_RECIEVE_BUFFER_SIZE (I2C_QUEUE_SIZE * sizeof(MetaInfo_t))
#define I2C_RECIEVE_RAW_SIZE (I2C_QUEUE_SIZE * 2)

static DataPacket_t DataBufferStorage[I2C_DATA_BUFFER_SIZE];
static QueueHandle_t I2C_DataQueue;

/**
 * @brief   Sets up the I2C ports as described by the 
 */
HAL_StatusTypeDef BSP_I2C_Init(I2C_HandleTypeDef *hi2c) {
    //QUEUES
    I2C_DataQueue = xQueueCreateStatic(I2C_DATA_BUFFER_SIZE, sizeof(DataPacket_t), DataBufferStorage, &DataStaticQueue);
    return HAL_I2C_Init(hi2c);
}

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_TX(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len)
{
    // HAL_I2C_Master_Transmit_IT(hi2c, deviceAdd, pDataBuff, len);
}

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c:            to structure that has config data
* @param    deviceAdd      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data   
* @return   HAL_StatusTypeDef   
*/
HAL_StatusTypeDef BSP_I2C_RX(I2C_HandleTypeDef *hi2c, 
              uint8_t deviceAdd, 
              uint8_t* pDataBuff, 
              uint16_t len)
{
    return HAL_I2C_Master_Receive_IT(hi2c, deviceAdd, pDataBuff, len);
}
