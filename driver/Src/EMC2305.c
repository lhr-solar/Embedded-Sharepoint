// Driver for Microchip EMC2305 PWM Fan Controller

#include "EMC2305.h"

// Device Management Functions

/**
 * @brief   Initializes the I2C bus and validates the chip by reading the Product ID and Manufacturer ID
 * @param   chip EMC2305 to initialize
 * @param   hi2c STM32 HAL I2C handle
 * @param   dev_addr Device address (7-bit address << 1)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_Init(EMC2305_HandleTypeDef* chip, I2C_HandleTypeDef* hi2c, uint16_t dev_addr) {
    // Set I2C handle and device address in EMC2305 handle
    chip->hi2c = hi2c;
    chip->dev_addr = dev_addr;

    // Check Product ID
    uint8_t product_id = 0;
    EMC2305_ReadReg(chip, EMC2305_REG_PRODUCT_ID, &product_id);
    if ((product_id & 0b11) != 0b00) {
        // EMC2305 is id 00
        return EMC2305_ERR;
    }

    // Check Manufacturer ID
    uint8_t mfg_id = 0;
    EMC2305_ReadReg(chip, EMC2305_REG_MANUFACTURER_ID, &mfg_id);
    if (mfg_id != 0x5D) {
        // ur cooked lmao
        return EMC2305_ERR;
    }

    return EMC2305_OK;
}

// Fan Configuration Functions

// Fan Control Functions

// Status & Measurement Functions

// Register Read/Write Functions

/**
 * @brief   Reads a byte of data from the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to read from
 * @param   data Pointer where register data will be stored
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_ReadReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t* data) {
    // Transmit register to read
    if (HAL_I2C_Master_Transmit(chip->hi2c, chip->dev_addr, &reg, 1, EMC2305_I2C_TIMEOUT) != HAL_OK) {
        return EMC2305_ERR;
    }
    HAL_Delay(1);
    // Receive response
    if (HAL_I2C_Master_Receive(chip->hi2c, chip->dev_addr, data, 1, EMC2305_I2C_TIMEOUT) != HAL_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

/**
 * @brief   Writes a byte of data to the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to write to
 * @param   data Data to write
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_WriteReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t data) {
    // Transmit register + data
    if (HAL_I2C_Master_Transmit(chip->hi2c, chip->dev_addr, (uint8_t[]) { reg, data }, 2, EMC2305_I2C_TIMEOUT) != HAL_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}
