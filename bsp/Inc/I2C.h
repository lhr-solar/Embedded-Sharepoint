/* Copyright (c) 2018-2023 UT Longhorn Racing Solar */
// BSP I2C header file

#ifndef BSP_I2C_H
#define BSP_I2C_H

#define I2C_HAL_OFFSET  0x10
typedef enum {
    I2C_OK          = 0x00U,
    I2C_SENT        = 0x01U,
    I2C_QFAIL       = 0x02U,    // Queue failure
    I2C_QFULL       = 0x03U,    // Queue full
    I2C_IFAIL       = 0x04U,    // Init failure
    I2C_BADSIZE     = 0x05U,    // Incorrect payload size
    I2C_BADINST     = 0x06U,    // Invalid I2C instance
    I2C_BUSY        = 0x07U,    // Bus is busy
} i2c_status;


#ifdef I2C1
/**
 * @brief Initialize I2C port for EEPROM interaction
 *
 * @param hi2c Config for the i2c peripheral setup
 * @return i2c_status
 */
i2c_status i2c_init(I2C_HandleTypeDef *hi2c);

/**
 * @brief Initialize I2C port for EEPROM interaction
 *
 * @param hi2c Config for the i2c peripheral deinit
 * @return i2c_status
 */
i2c_status i2c_deinit(I2C_HandleTypeDef *hi2c);

/**
* @brief    Transmits data onto the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr     target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   i2c_status
*/
i2c_status i2c_send(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len);

/**
* @brief    Receives data from the I2C bus.
* @param    hi2c           to structure that has config data
* @param    deviceAddr      target device address
* @param    pDataBuff      data buffer
* @param    len            amount of data
* @return   i2c_status
*/
i2c_status i2c_recv(I2C_HandleTypeDef *hi2c,
              uint8_t deviceAddr,
              uint8_t* pDataBuff,
              uint16_t len);

#endif
#endif