// Driver for Microchip EMC2305 PWM Fan Controller

#include "EMC2305.h"

// Buffer for static semaphores
StaticSemaphore_t chip_semaphore_buffer;
StaticSemaphore_t caller_semaphore_buffer;

// Queue of I2C messages to be sent to the EMC2305
QueueHandle_t EMC2305_I2C_Queue;
// Array to use as the queue's storage area
uint8_t ucQueueStorageArea[EMC2305_QUEUE_LENGTH * EMC2305_QUEUE_ITEM_SIZE];
// Variable to hold the queue's data structure
static StaticQueue_t xStaticQueue;

// Task buffer for I2C worker
StaticTask_t EMC2305_I2C_TaskBuffer;
// Task stack for I2C worker
StackType_t EMC2305_I2C_TaskStack[configMINIMAL_STACK_SIZE];

// Some bullshit until i find a better way
static EMC2305_HandleTypeDef* chip_I2C1 = NULL;
static EMC2305_HandleTypeDef* chip_I2C2 = NULL;
static EMC2305_HandleTypeDef* chip_I2C3 = NULL;

// TODO: for debug, REMOVE
#define STATUS_LED_PORT GPIOA
#define STATUS_LED_PIN_1 GPIO_PIN_7
#define STATUS_LED_PIN_2 GPIO_PIN_8
#define STATUS_LED_PIN_3 GPIO_PIN_15

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

    // Create RTOS semaphore
    chip->i2c_complete = xSemaphoreCreateBinaryStatic(&chip_semaphore_buffer);
    if (chip->i2c_complete == NULL) {
        return EMC2305_ERR;
    }

    // Create RTOS queue
    EMC2305_I2C_Queue = xQueueCreateStatic(EMC2305_QUEUE_LENGTH, EMC2305_QUEUE_ITEM_SIZE, ucQueueStorageArea, &xStaticQueue);
    if (EMC2305_I2C_Queue == NULL) {
        return EMC2305_ERR;
    }

    // Create I2C Worker Task
    xTaskCreateStatic(EMC2305_I2C_Worker_Task,
        "EMC2305 I2C Worker Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        EMC2305_I2C_TaskStack,
        &EMC2305_I2C_TaskBuffer);

    // I hate this so much
    if (hi2c->Instance == I2C1) {
        chip_I2C1 = chip;
    }
    else if (hi2c->Instance == I2C2) {
        chip_I2C2 = chip;
    }
    else if (hi2c->Instance == I2C3) {
        chip_I2C3 = chip;
    }
    else {
        return EMC2305_ERR;
    }

    // Check Product ID
    uint8_t product_id = 0;
    if (EMC2305_ReadReg(chip, EMC2305_REG_PRODUCT_ID, &product_id) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    if ((product_id & 0b11) != 0b00) {
        // EMC2305 is id 00
        return EMC2305_ERR;
    }

    // Check Manufacturer ID
    uint8_t mfg_id = 0;
    if (EMC2305_ReadReg(chip, EMC2305_REG_MANUFACTURER_ID, &mfg_id) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    if (mfg_id != 0x5D) {
        // ur cooked lmao
        return EMC2305_ERR;
    }

    return EMC2305_OK;
}

/**
 * @brief   Software locks all SWL registers. SWL registers are now read-only until power cycle.
 * @param   chip EMC2305 to lock
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_EnableSWLock(EMC2305_HandleTypeDef* chip) {
    if (EMC2305_WriteReg(chip, EMC2305_REG_SW_LOCK, EMC2305_SWL) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

/**
 * @brief   Sets the EMC2305 global configuration register based on provided config
 * @param   chip EMC2305 to configure
 * @param   config Global configuration to use
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetGlobalConfig(EMC2305_HandleTypeDef* chip, EMC2305_Global_Config* config) {
    // Set bits based on config
    uint8_t config_bits = 0;
    if (config->alert_mask) {
        config_bits |= EMC2305_CFG_MASK_ALERT;
    }
    if (config->disable_smbus_timeout) {
        config_bits |= EMC2305_CFG_DIS_TO;
    }
    if (config->watchdog_enable) {
        config_bits |= EMC2305_CFG_WD_EN;
    }
    if (config->drive_ext_clk) {
        config_bits |= EMC2305_CFG_DRECK;
    }
    if (config->use_ext_clk) {
        config_bits |= EMC2305_CFG_USECK;
    }

    // Write configuration
    if (EMC2305_WriteReg(chip, EMC2305_REG_CONFIGURATION, config_bits) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

// Fan Configuration Functions

/**
 * @brief   Sets the base frequency of the specified fan's PWM driver
 * @param   chip EMC2305 to use
 * @param   fan Fan to use (1-5)
 * @param   freq Base frequency as specified in Section 6.10
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetPWMBaseFrequency(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, EMC2305_PWM_BaseFreq freq) {
    // Check if the user is stupid (lakshay)
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return EMC2305_ERR;
    }

    // Determine which register to use
    uint8_t reg;
    if ((fan == EMC2305_FAN4) || (fan == EMC2305_FAN5)) {
        // PWM BaseF45 Register
        reg = EMC2305_REG_PWM_BASEF45;
    }
    else {
        // PWM BaseF123 Register
        reg = EMC2305_REG_PWM_BASEF123;
    }

    // Read current value of register (need to be friendly :PPP)
    uint8_t val = 0;
    if (EMC2305_ReadReg(chip, reg, &val) != EMC2305_OK) {
        return EMC2305_ERR;
    }

    // Clear and update frequency
    switch (fan)
    {
    case EMC2305_FAN1:
        val &= ~EMC2305_PWM_FAN1_MASK;
        val |= freq << EMC2305_PWM_FAN1_SHIFT;
        break;
    case EMC2305_FAN2:
        val &= ~EMC2305_PWM_FAN2_MASK;
        val |= freq << EMC2305_PWM_FAN2_SHIFT;
        break;
    case EMC2305_FAN3:
        val &= ~EMC2305_PWM_FAN3_MASK;
        val |= freq << EMC2305_PWM_FAN3_SHIFT;
        break;
    case EMC2305_FAN4:
        val &= ~EMC2305_PWM_FAN4_MASK;
        val |= freq << EMC2305_PWM_FAN4_SHIFT;
        break;
    case EMC2305_FAN5:
        val &= ~EMC2305_PWM_FAN5_MASK;
        val |= freq << EMC2305_PWM_FAN5_SHIFT;
        break;
    default:
        // lmfao
        return EMC2305_ERR;
        break;
    }

    // Write PWM base frequency
    if (EMC2305_WriteReg(chip, reg, val) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

/**
 * @brief   Sets the EMC2305 configuration 1 and 2 registers based on the provided config
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   config1 Configuration for register 1
 * @param   config2 Configuration for register 2
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanConfig(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, EMC2305_Fan_Config1* config1, EMC2305_Fan_Config2* config2) {
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return EMC2305_ERR;
    }

    // Pack config 1
    uint8_t config1_bits = 0;
    config1_bits |= (config1->enable_closed_loop << EMC2305_CONFIG1_ENAG_SHIFT);
    config1_bits |= (config1->range << EMC2305_CONFIG1_RNG_SHIFT);
    config1_bits |= (config1->edges << EMC2305_CONFIG1_EDG_SHIFT);
    config1_bits |= (config1->update_time << EMC2305_CONFIG1_UDT_SHIFT);

    // Pack config 2
    uint8_t config2_bits = 0;
    config2_bits |= (config2->enable_ramp_rate_ctl << EMC2305_CONFIG2_ENRC_SHIFT);
    config2_bits |= (config2->enable_glitch_filter << EMC2305_CONFIG2_GHEN_SHIFT);
    config2_bits |= (config2->derivative_options << EMC2305_CONFIG2_DPT_SHIFT);
    config2_bits |= (config2->error_window << EMC2305_CONFIG2_ERG_SHIFT);

    // Write config 1
    if (EMC2305_WriteReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_CONFIG1), config1_bits) != EMC2305_OK) {
        return EMC2305_ERR;
    }

    // Write config 2
    if (EMC2305_WriteReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_CONFIG2), config2_bits) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

// Fan Control Functions

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanPWM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, uint8_t duty_cycle) {
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return EMC2305_ERR;
    }

    // oops
    if (duty_cycle > 100) {
        return EMC2305_ERR;
    }

    // Convert duty cycle percentage drive to register value using Equation 4-1: Drive = (Value/255) * 100
    uint8_t val = (duty_cycle * 255) / 100;

    // Write PWM to drive register
    if (EMC2305_WriteReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_SETTING), val) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

/**
 * @brief   Sets the RPM target for the control algorithm. Works only with fan speed control (FSC) enabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   rpm_target RPM target to set (500 to 16k)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanRPM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, uint16_t rpm_target) {
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return EMC2305_ERR;
    }

    // idiot check
    if ((rpm_target < 500) || (rpm_target > 16000)) {
        return EMC2305_ERR;
    }

    // Convert RPM target to tachometer counts
    // DO NOT CHANGE THIS NUMBER OR CALCULATION YOU WILL REGRET
    // WARNING: assuming 2 poles, 32.768 measurement freq, 5 edges, multiplier = 1
    // This is the default configuration. Reference app note (https://ww1.microchip.com/downloads/en/AppNotes/en562764.pdf)
    uint16_t tach_target = (EMC2305_TACH_RPM_CONV * EMC2305_TACH_MULT) / rpm_target;

    // Split into high and low bytes
    tach_target &= 0x1FFF; // ensure 13-bit range
    uint8_t high_byte = (tach_target >> 5) & 0xFF;     // bits [12:5]
    uint8_t low_byte = (tach_target & 0x1F) << 3;      // bits [4:0]

    // Must write low byte first, value is saved once high byte is written
    if (EMC2305_WriteReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_TACH_TARGET_L), low_byte) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    if (EMC2305_WriteReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_TACH_TARGET_H), high_byte) != EMC2305_OK) {
        return EMC2305_ERR;
    }
    return EMC2305_OK;
}

// Status & Measurement Functions

/**
 * @brief   Gets current fan RPM based on tachometer measurement
 * @param   chip EMC2305 to get
 * @param   fan Fan to get (1-5)
 * @return  Measured fan RPM. UINT16_MAX on error
 */
uint16_t EMC2305_GetFanRPM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan) {
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return UINT16_MAX;
    }

    // Read tachometer reading high and low bytes
    uint8_t high_byte = 0;
    uint8_t low_byte = 0;
    if (EMC2305_ReadReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_TACH_READING_H), &high_byte) != EMC2305_OK) {
        return UINT16_MAX;
    }
    if (EMC2305_ReadReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_TACH_READING_L), &low_byte) != EMC2305_OK) {
        return UINT16_MAX;
    }

    // Combine into tach measurement
    uint16_t tach_reading = ((high_byte << 5) + ((low_byte >> 3) & 0x1F)) & 0x1FFF;

    // Divide by zero bad
    if (tach_reading == 0) {
        return UINT16_MAX;
    }

    // Convert tachometer counts to RPM
    uint16_t rpm = (EMC2305_TACH_RPM_CONV * EMC2305_TACH_MULT) / tach_reading;
    return rpm;
}

/**
 * @brief   Gets current fan driver PWM duty cycle
 * @param   chip EMC2305 to get
 * @param   fan Fan to get (1-5)
 * @return  Driven fan PWM duty cycle (0-100). UINT8_MAX on error
 */
uint8_t EMC2305_GetFanPWM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan) {
    if (fan < EMC2305_FAN1 || fan > EMC2305_FAN5) {
        return UINT8_MAX;
    }

    // Read PWM from drive register
    uint8_t val = 0;
    if (EMC2305_ReadReg(chip, EMC2305_FAN_REG_ADDR(fan, EMC2305_REG_FAN1_SETTING), &val) != EMC2305_OK) {
        return UINT8_MAX;
    }

    // Convert register value to duty cycle percentage
    uint8_t pwm = (val * 100) / 255;

    return pwm;
}

/**
 * @brief   Gets current fan status for all drivers
 * @param   chip EMC2305 to get
 * @return  Fan status for all drivers
 */
EMC2305_Fan_Status EMC2305_GetFanStatus(EMC2305_HandleTypeDef* chip) {
    EMC2305_Fan_Status status = { 0 };

    // Read fan status register
    uint8_t val = 0;
    if (EMC2305_ReadReg(chip, EMC2305_REG_FAN_STATUS, &val) != EMC2305_OK) {
        // Error handling is fake :D
        return status;
    }

    // Update status values
    status.watchdog_fired = ((val & EMC2305_STAT_WATCH) == EMC2305_STAT_WATCH);
    status.drive_failed = ((val & EMC2305_STAT_DRVFAIL) == EMC2305_STAT_DRVFAIL);
    status.spin_failed = ((val & EMC2305_STAT_FNSPIN) == EMC2305_STAT_FNSPIN);
    status.stalled = ((val & EMC2305_STAT_FNSTL) == EMC2305_STAT_FNSTL);

    return status;
}

// Register Read/Write Functions

/**
 * @brief   Reads a byte of data from the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to read from
 * @param   data Pointer where register data will be stored
 * @return  OK if successful, ERR if message queue is full
 */
EMC2305_Status EMC2305_ReadReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t* data) {
    // Create RTOS semaphore
    SemaphoreHandle_t complete = xSemaphoreCreateBinaryStatic(&caller_semaphore_buffer);
    if (complete == NULL) {
        return EMC2305_ERR;
    }

    // Pack message struct
    EMC2305_I2C_Message msg = {
        .chip = chip,
        .operation = EMC2305_OP_READ,
        .reg_addr = reg,
        .write_data = 0,
        .read_data = data,
        .complete = complete,
    };

    if (xQueueSend(EMC2305_I2C_Queue, &msg, 0) != pdTRUE) {
        // Error if queue is full
        vSemaphoreDelete(complete);
        return EMC2305_ERR;
    }

    // Wait for the worker task to signal completion
    if (xSemaphoreTake(complete, pdMS_TO_TICKS(EMC2305_I2C_TIMEOUT * 2)) != pdTRUE) {
        // Timeout waiting for worker to finish the I2C operation
        vSemaphoreDelete(complete);
        return EMC2305_ERR;
    }

    vSemaphoreDelete(complete);
    return EMC2305_OK;
}

/**
 * @brief   Writes a byte of data to the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to write to
 * @param   data Data to write
 * @return  OK if successful, ERR if message queue is full
 */
EMC2305_Status EMC2305_WriteReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t data) {
    // Create RTOS semaphore
    SemaphoreHandle_t complete = xSemaphoreCreateBinaryStatic(&caller_semaphore_buffer);
    if (complete == NULL) {
        return EMC2305_ERR;
    }

    // Pack message struct
    EMC2305_I2C_Message msg = {
        .chip = chip,
        .operation = EMC2305_OP_WRITE,
        .reg_addr = reg,
        .write_data = data,
        .read_data = NULL,
    };

    if (xQueueSend(EMC2305_I2C_Queue, &msg, 0) != pdTRUE) {
        // Error if queue is full
        vSemaphoreDelete(complete);
        return EMC2305_ERR;
    }

    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_2);

    // Wait for the worker task to signal completion
    if (xSemaphoreTake(complete, pdMS_TO_TICKS(EMC2305_I2C_TIMEOUT * 2)) != pdTRUE) {
        // Timeout waiting for worker to finish the I2C operation
        vSemaphoreDelete(complete);
        return EMC2305_ERR;
    }

    vSemaphoreDelete(complete);
    return EMC2305_OK;
}

// Worker task to consume messages from the queue and send on I2C bus
void EMC2305_I2C_Worker_Task(void* pvParameters) {
    EMC2305_I2C_Message msg = { 0 };
    EMC2305_Status status = EMC2305_OK;

    while (1) {
        // Block until message enters the queue
        if (xQueueReceive(EMC2305_I2C_Queue, &msg, portMAX_DELAY) == pdTRUE) {
            status = EMC2305_OK;
            if (msg.operation == EMC2305_OP_WRITE) {
                HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_3);
                // Perform write operation
                // Transmit register + data
                if (HAL_I2C_Master_Transmit_IT(msg.chip->hi2c, msg.chip->dev_addr, (uint8_t[]) { msg.reg_addr, msg.write_data }, 2) != HAL_OK) {
                    status = EMC2305_ERR;
                }

                // Wait for ISR to signal completion
                if ((status == EMC2305_OK) && (xSemaphoreTake(msg.chip->i2c_complete, pdMS_TO_TICKS(EMC2305_I2C_TIMEOUT)) != pdTRUE)) {
                    status = EMC2305_ERR;
                }
            }
            else if (msg.operation == EMC2305_OP_READ) {
                // Perform read operation
                // Transmit register to read
                if (HAL_I2C_Master_Transmit_IT(msg.chip->hi2c, msg.chip->dev_addr, &msg.reg_addr, 1) != HAL_OK) {
                    status = EMC2305_ERR;
                }

                // Wait for ISR to signal completion
                if ((status == EMC2305_OK) && (xSemaphoreTake(msg.chip->i2c_complete, pdMS_TO_TICKS(EMC2305_I2C_TIMEOUT))) != pdTRUE) {
                    status = EMC2305_ERR;
                }

                // Receive response
                if ((status == EMC2305_OK) && (HAL_I2C_Master_Receive_IT(msg.chip->hi2c, msg.chip->dev_addr, msg.read_data, 1)) != HAL_OK) {
                    status = EMC2305_ERR;
                }

                // Wait for ISR to signal completion
                if ((status == EMC2305_OK) && (xSemaphoreTake(msg.chip->i2c_complete, pdMS_TO_TICKS(EMC2305_I2C_TIMEOUT)) != pdTRUE)) {
                    status = EMC2305_ERR;
                }
            }
        }

        // Signal to the calling task that I2C transaction has finished
        xSemaphoreGive(msg.complete);
        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_1);
    }
}

// I2C Transmit Interrupt Callback
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Get the chip using this I2C bus
    EMC2305_HandleTypeDef* chip = NULL;
    if (hi2c->Instance == I2C1) {
        chip = chip_I2C1;
    }
    else if (hi2c->Instance == I2C2) {
        chip = chip_I2C2;
    }
    else if (hi2c->Instance == I2C3) {
        chip = chip_I2C3;
    }

    if (chip != NULL) {
        xSemaphoreGiveFromISR(chip->i2c_complete, &xHigherPriorityTaskWoken);
    }

    // Context switch back to higher priority task if woken
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// I2C Receive Interrupt Callback
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Get the chip using this I2C bus
    EMC2305_HandleTypeDef* chip = NULL;
    if (hi2c->Instance == I2C1) {
        chip = chip_I2C1;
    }
    else if (hi2c->Instance == I2C2) {
        chip = chip_I2C2;
    }
    else if (hi2c->Instance == I2C3) {
        chip = chip_I2C3;
    }

    if (chip != NULL) {
        xSemaphoreGiveFromISR(chip->i2c_complete, &xHigherPriorityTaskWoken);
    }

    // Context switch back to higher priority task if woken
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

// I2C Error Interrupt Callback
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
    // TODO: remove....
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
}
