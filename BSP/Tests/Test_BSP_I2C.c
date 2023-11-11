#include "FreeRTOS.h"
#include "queue.h"
#include "BSP_I2C.h"
#include "BSP.h"

extern QueueHandle_t getQ();

int main () {
    BSP_Init(); // config all gpio
    BSP_I2C_Init(&i2c3_handle);

    uint8_t* buffer = {0xD, 0xE, 0xA, 0xD};
    BSP_I2C_RegisterWrite(&i2c3_handle, 0x50, 0x000A, 4, buffer, 4);
    uint8_t readData[64];
    BSP_I2C_RegisterRead(&i2c3_handle, 0x50, 0x000A, 4, 4);
    
    QueueHandle_t abcd = getQ();
    for (int i = 0; i < 4; i++) {
        xQueueReceive(abcd, &(readData[i]), 0);
    }
    for (int i = 0; i < 64; i++) {
        printf("%c", readData[i]);
    }
}