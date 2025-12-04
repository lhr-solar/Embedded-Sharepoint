// Driver for Microchip EMC2305 PWM Fan Controller

#pragma once

#include <stdint.h>

#include "stm32xx_hal.h"
#include "FreeRTOS.h"

// Device handle
typedef struct {
    I2C_HandleTypeDef* hi2c;        // STM32 HAL I2C handle
    uint16_t dev_addr;              // HAL convention: 7-bit address << 1
    SemaphoreHandle_t i2c_complete; // Semaphore to signal I2C transaction complete
} EMC2305_HandleTypeDef;

// I2C Response Timeout
#ifndef EMC2305_I2C_TIMEOUT
#define EMC2305_I2C_TIMEOUT 100u // 100ms default
#endif

// I2C Operation Types
typedef enum {
    EMC2305_OP_WRITE,
    EMC2305_OP_READ
} EMC2305_I2C_OP;

// I2C Message Struct
typedef struct {
    EMC2305_HandleTypeDef* chip;    // Chip to send to
    EMC2305_I2C_OP operation;       // Read/Write operation
    uint8_t reg_addr;               // Register address
    uint8_t write_data;             // Data to write (only used for write operations)
    uint8_t* read_data;             // Pointer for storing read data (only used for read operations)
} EMC2305_I2C_Message;

#define EMC2305_QUEUE_LENGTH 10                                 // Message queue length
#define EMC2305_QUEUE_ITEM_SIZE sizeof(EMC2305_I2C_Message)                   // Size of queue item (message)

// Device status
typedef enum {
    EMC2305_OK,
    EMC2305_ERR,
} EMC2305_Status;

// Device configuration
typedef struct {
    bool alert_mask;                // Blocks the alert pin from being asserted
    bool disable_smbus_timeout;     // Disables the SMBus Time-Out function for the SMBus client (if enabled)
    bool watchdog_enable;           // Enables the Watchdog Timer (see Section 4.11 “Watchdog Timer”) to operate in Continuous Mode
    bool drive_ext_clk;             // Enables the internal tachometer clock to be driven out on the CLK pin so that multiple devices can be synced to the same source
    bool use_ext_clk;               // Enables the device to use a clock present on the CLK pin as the tachometer clock. If the DR_EXT_CLK bit is set, then this bit is ignored and the device will use the internal oscillator.
} EMC2305_Global_Config;

// Fans
typedef enum {
    EMC2305_FAN1,
    EMC2305_FAN2,
    EMC2305_FAN3,
    EMC2305_FAN4,
    EMC2305_FAN5,
} EMC2305_Fan;

// PWM Base Frequency
typedef enum {
    EMC2305_PWM_2k441 = 0b11, // 2.441 kHz
    EMC2305_PWM_4k882 = 0b10, // 4.882 kHz
    EMC2305_PWM_19k53 = 0b01, // 19.53 kHz
    EMC2305_PWM_26k00 = 0b00, // 26.00 kHz
} EMC2305_PWM_BaseFreq;

// Range - Sets the minimum fan speed measured and reported
typedef enum {
    EMC2305_RNG_4000 = 0b11, // 4000 RPM minimum, TACH count multiplier = 8
    EMC2305_RNG_2000 = 0b10, // 2000 RPM minimum, TACH count multiplier = 4
    EMC2305_RNG_1000 = 0b01, // 1000 RPM minimum, TACH count multiplier = 2
    EMC2305_RNG_500 = 0b00,  // 500 RPM minimum, TACH count multiplier = 1
} EMC2305_RNG;

// Edges - Sets the number of edges to sample when calculating RPM
typedef enum {
    EMC2305_EDG_9 = 0b11, // 9 edges sampled (4 poles) - effective Tach multiplier is 2, based on two pole fans
    EMC2305_EDG_7 = 0b10, // 7 edges sampled (3 poles) - effective Tach multiplier is 1.5, based on two pole fans
    EMC2305_EDG_5 = 0b01, // 5 edges sampled (2 poles) - effective Tach multiplier is 1, based on two pole fans
    EMC2305_EDG_3 = 0b00, // 3 edges sampled (1 pole) - effective Tach multiplier is 0.5, based on two pole fans
} EMC2305_EDG;

// Update Time - Sets the PID update rate for closed loop control
typedef enum {
    EMC2305_UDT_100 = 0b000,  // 100 ms update interval
    EMC2305_UDT_200 = 0b001,  // 200 ms update interval
    EMC2305_UDT_300 = 0b010,  // 300 ms update interval
    EMC2305_UDT_400 = 0b011,  // 400 ms update interval
    EMC2305_UDT_500 = 0b100,  // 500 ms update interval
    EMC2305_UDT_800 = 0b101,  // 800 ms update interval
    EMC2305_UDT_1200 = 0b110,  // 1200 ms update interval
    EMC2305_UDT_1600 = 0b111,  // 1600 ms update interval
} EMC2305_UDT;

// Fan Configuration 1
typedef struct {
    bool enable_closed_loop;
    EMC2305_RNG range;
    EMC2305_EDG edges;
    EMC2305_UDT update_time;
} EMC2305_Fan_Config1;

// Derivative Options - Selects form of derivative used in PID
typedef enum {
    EMC2305_DPT_NONE = 0b00,  // No derivative term
    EMC2305_DPT_BASIC = 0b01, // Basic derivative
    EMC2305_DPT_STEP = 0b10,  // Step derivative
    EMC2305_DPT_BOTH = 0b11,  // Both basic + step
} EMC2305_DPT;

// Error Window - Acceptable RPM deviation around target
typedef enum {
    EMC2305_ERG_0RPM = 0b00,   // 0 RPM window
    EMC2305_ERG_50RPM = 0b01,  // ±50 RPM
    EMC2305_ERG_100RPM = 0b10, // ±100 RPM
    EMC2305_ERG_200RPM = 0b11, // ±200 RPM
} EMC2305_ERG;

// Fan Configuration 2
typedef struct {
    bool enable_ramp_rate_ctl;
    bool enable_glitch_filter;
    EMC2305_DPT derivative_options;
    EMC2305_ERG error_window;
} EMC2305_Fan_Config2;

typedef struct {
    // TODO(rshah): fill in config values
} EMC2305_SpinUp_Config;

typedef enum {
    EMC2305_PID_8X = 0b11, // 8x gain
    EMC2305_PID_4X = 0b10, // 4x gain
    EMC2305_PID_2X = 0b01, // 2x gain
    EMC2305_PID_1X = 0b00, // 1x gain
} EMC2305_PID_Gain;

typedef struct {
    bool watchdog_fired; // Indicates that the Watchdog Timer has expired. When this bit is set, each fan is driven to 100% duty cycle and will remain at 100% duty cycle until they are programmed. This bit is cleared when it is read.
    bool drive_failed;   // Indicates that one or more fan drivers cannot meet the programmed fan speed at maximum PWM duty cycle.
    bool spin_failed;    // Indicates that one or more fan drivers cannot spin up.
    bool stalled;        // Indicates that one or more fan drivers have stalled.
} EMC2305_Fan_Status;

// Register map from EMC2305 datasheet
// https://ww1.microchip.com/downloads/aemDocuments/documents/MSLD/ProductDocuments/DataSheets/EMC2301-2-3-5-Data-Sheet-DS20006532A.pdf

// Device Configuration
#define EMC2305_REG_CONFIGURATION           0x20u // The Configuration register controls the basic functionality of the EMC2301/2/3/5. The Configuration Register is software locked.
#define EMC2305_REG_FAN_INTERRUPT_ENABLE    0x29u // The Fan Interrupt Enable register controls the masking for each fan channel. When a channel is enabled, it will cause the ALERT pin to be asserted when an Error condition is detected. • ‘1’ - An Error condition (Stall, Spin Up, Drive Fail) on fan X will cause the ALERT pin to be asserted. • ‘0’ (default) - An Error condition on fan X will not cause the ALERT pin to be asserted; however, the status registers will be updated normally.
#define EMC2305_REG_PWM_POLARITY            0x2Au // The PWM Polarity Configuration registers control the output type and polarity of all PWM outputs.
#define EMC2305_REG_PWM_OUTPUT_CONFIG       0x2Bu // The PWM Output Configuration register controls the PWM output type as push-pull or open drain.
#define EMC2305_REG_PWM_BASEF45             0x2Cu // The PWM BASEF45 Register controls the base frequency of PWM drivers 4 and 5.
#define EMC2305_REG_PWM_BASEF123            0x2Du // The PWM BaseF123 Register controls the base frequency of PWM drivers 1, 2 and 3.
#define EMC2305_REG_SW_LOCK                 0xEFu // Software Lock Register - bit 0 LOCK Enables the SWL function: 1 = All SWL registers are locked and read-only. Unlock occurs on power cycle. 0 = All SWL registers are writable.
#define EMC2305_REG_PRODUCT_FEAT            0xFCu // Product Features Register - bit 5-3: ADR[2:0]: SMBus address determined by the ADDR_SEL pin decode. bit 2-0:  FSP[2:0]: Default Fan Speed determined by the CLK pin decode. This is conditional on ADDR_SEL.
#define EMC2305_REG_PRODUCT_ID              0xFDu // Production Identification Register - bit 1-0: PID[1:0]: Product Identification Register. Device defines bit pattern. 00 for EMC2305
#define EMC2305_REG_MANUFACTURER_ID         0xFEu // Manufacturer Identification Register - Hard coded fixed value of 0x5D
#define EMC2305_REG_REVISION                0xFFu // Silicon Revision Register - Hard coded fixed value of 0x80

// Device Status
#define EMC2305_REG_FAN_STATUS              0x24u // The Fan Status register indicates that the fan driver has stalled or failed or that the Watchdog Timer has expired (see Section 4.11 “Watchdog Timer”).
#define EMC2305_REG_FAN_STALL_STATUS        0x25u // The Fan Stall Status register indicates which fan driver has detected a stalled condition (see Section 4.4.3 “Stalled Fan”). All bits are cleared upon a read if the Error condition has been removed.
#define EMC2305_REG_FAN_SPIN_STATUS         0x26u // The Fan Spin Status register indicates which fan driver has failed to spin-up (see Section 4.8 “Spin Up Rou tine”). All bits are cleared upon a read if the Error condition has been removed.
#define EMC2305_REG_DRIVE_FAIL_STATUS       0x27u // The Fan Drive Fail Status register indicates which fan driver cannot drive to the programmed speed even at 100% duty cycle (see Section 4.4.4 “Aging Fan or Invalid Drive Detection” and Register 6-16). All bits are cleared upon a read if the Error condition has been removed.

// Fan 1 Registers (base registers for all fans)
#define EMC2305_REG_FAN1_SETTING            0x30u // Fan 1 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM1_DIVIDE             0x31u // Fan 1 PWM Divide Register - The PWM Divide registers determine the final frequency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
#define EMC2305_REG_FAN1_CONFIG1            0x32u // Fan 1 Configuration Register 1 - The Fan Configuration 1 registers control the general operation of the RPM-based Fan Speed Control algorithm used for the respective Fan Driver (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”).
#define EMC2305_REG_FAN1_CONFIG2            0x33u // Fan 1 Configuration Register 2 - The Fan Configuration 2 registers control the tachometer measurement and advanced features of the RPM based Fan Speed Control algorithm (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”)
#define EMC2305_REG_GAIN1                   0x35u // Fan 1 PID Gain Register - See Section 4.3.3.2 “Setting the PID Gains”.
#define EMC2305_REG_FAN1_SPIN               0x36u // Fan 1 Spin Up Configuration Registers - The Fan Spin Up Configuration registers control the settings of the Spin Up Routine. These registers are software locked (see Section 4.8 “Spin Up Routine”)
#define EMC2305_REG_FAN1_MAX_STEP           0x37u // Fan 1 Maximum Step Size Register - This register determines the maximum step size for the ramp rate control (see Section 4.3.3.3 “Fan Drive Max Step” and Section 4.10 “Ramp Rate Control”).
#define EMC2305_REG_FAN1_MIN_DRIVE          0x38u // Fan 1 Minimum Drive Register - The Fan Minimum Drive Register (see Section 4.3.3.4 “Minimum Drive Setting”) stores the minimum drive setting for each RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN1_VALID_TACH         0x39u // Fan 1 Valid TACH Count Register - The Valid TACH Count registers store the maximum TACH Reading register value to indicate that each fan is spinning properly (see Section 4.4.2 “Valid Tachometer Readings”). 
#define EMC2305_REG_FAN1_DRVFAIL_L          0x3Au // Fan 1 Drive Fail Band Low Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN1_DRVFAIL_H          0x3Bu // Fan 1 Drive Fail Band High Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN1_TACH_TARGET_L      0x3Cu // Fan 1 Tachometer Closed Loop Target Low Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN1_TACH_TARGET_H      0x3Du // Fan 1 Tachometer Closed Loop Target High Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN1_TACH_READING_H     0x3Eu // Fan 1 Tachometer Reading High Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”). 
#define EMC2305_REG_FAN1_TACH_READING_L     0x3Fu // Fan 1 Tachometer Reading Low Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”). 

// Address offset per fan register block
#define EMC2305_FAN_ADDRESS_OFFSET         0x10u

/**
 * @brief Calculates the register address for a specific fan (N) based on the
 * Fan 1 register address and known offset
 *
 * @param fan_num The fan number (0-4 to match fan enum).
 * @param fan1_reg_addr The address of the corresponding Fan 1 register (base)
 * @return The calculated register address for Fan N.
 */
#define EMC2305_FAN_REG_ADDR(fan_num, fan1_reg_addr) ((fan1_reg_addr) + ((fan_num) * EMC2305_FAN_ADDRESS_OFFSET))

 // Bitmasks
 // Config (0x20) bits
#define EMC2305_CFG_MASK_ALERT              (1u << 7)  /* MASK - mask ALERT pin when set */
#define EMC2305_CFG_DIS_TO                  (1u << 6)  /* DIS_TO - SMBus timeout disable (I2C compat) */
#define EMC2305_CFG_WD_EN                   (1u << 5)  /* WD_EN - enable watchdog continuous mode */
#define EMC2305_CFG_DRECK                   (1u << 1)  /* DRECK - CLK pin drives internal clock when set (output) */
#define EMC2305_CFG_USECK                   (1u << 0)  /* USECK - use external CLK pin as tach clock when set */

// Fan Status (0x24) bits
#define EMC2305_STAT_WATCH                  (1u << 7)  /* WATCH - watchdog fired (read-to-clear) */
#define EMC2305_STAT_DRVFAIL                (1u << 2)  /* Drive fail summary */
#define EMC2305_STAT_FNSPIN                 (1u << 1)  /* spin-up failure summary */
#define EMC2305_STAT_FNSTL                  (1u << 0)  /* stall summary */

// Bitmasks for Fan 1-5
#define EMC2305_FAN1_MASK                   (1u << 0)
#define EMC2305_FAN2_MASK                   (1u << 1)
#define EMC2305_FAN3_MASK                   (1u << 2)
#define EMC2305_FAN4_MASK                   (1u << 3)
#define EMC2305_FAN5_MASK                   (1u << 4)

// Software Lock (0xEF)
#define EMC2305_SWL                         (1u << 0)

// Bit masks for PWM Base Freq
#define EMC2305_PWM_FAN1_MASK               0x03u
#define EMC2305_PWM_FAN2_MASK               0x0Cu
#define EMC2305_PWM_FAN3_MASK               0x30u
#define EMC2305_PWM_FAN4_MASK               0x03u
#define EMC2305_PWM_FAN5_MASK               0x0Cu

// Bit shifts for PWM Base Freq
#define EMC2305_PWM_FAN1_SHIFT              0u
#define EMC2305_PWM_FAN2_SHIFT              2u
#define EMC2305_PWM_FAN3_SHIFT              4u
#define EMC2305_PWM_FAN4_SHIFT              0u
#define EMC2305_PWM_FAN5_SHIFT              2u

// Bit shifts for fan configuration 1 register
#define EMC2305_CONFIG1_ENAG_SHIFT          7u
#define EMC2305_CONFIG1_RNG_SHIFT           5u
#define EMC2305_CONFIG1_EDG_SHIFT           3u
#define EMC2305_CONFIG1_UDT_SHIFT           0u

// Bit shifts for fan configuration 2 register
#define EMC2305_CONFIG2_ENRC_SHIFT          6u
#define EMC2305_CONFIG2_GHEN_SHIFT          5u
#define EMC2305_CONFIG2_DPT_SHIFT           3u
#define EMC2305_CONFIG2_ERG_SHIFT           1u

// Conversion from RPM to tach counts (Equation 2 in SMSC AN 17.4)
#define EMC2305_TACH_RPM_CONV               3932160u
#define EMC2305_TACH_MULT                   2u

// Device Management Functions

/**
 * @brief   Initializes the I2C bus and validates the chip by reading the Product ID and Manufacturer ID
 * @param   chip EMC2305 to initialize
 * @param   hi2c STM32 HAL I2C handle
 * @param   dev_addr Device address (7-bit address << 1)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_Init(EMC2305_HandleTypeDef* chip, I2C_HandleTypeDef* hi2c, uint16_t dev_addr);

/**
 * @brief   Software locks all SWL registers. SWL registers are now read-only until power cycle.
 * @param   chip EMC2305 to lock
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_EnableSWLock(EMC2305_HandleTypeDef* chip);

/**
 * @brief   Sets the EMC2305 global configuration register based on provided config
 * @param   chip EMC2305 to configure
 * @param   config Global configuration to use
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetGlobalConfig(EMC2305_HandleTypeDef* chip, EMC2305_Global_Config* config);

// Fan Configuration Functions

/**
 * @brief   Sets the base frequency of the specified fan's PWM driver
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   freq Base frequency as specified in Section 6.10
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetPWMBaseFrequency(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, EMC2305_PWM_BaseFreq freq);

/**
 * @brief   Sets the EMC2305 configuration 1 and 2 registers based on the provided config
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   config1 Configuration for register 1
 * @param   config2 Configuration for register 2
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanConfig(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, EMC2305_Fan_Config1* config1, EMC2305_Fan_Config2* config2);

// Fan Control Functions

/**
 * @brief   Sets the PWM drive of the specified fan directly. Works only with fan speed control (FSC) disabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   duty_cycle PWM duty cycle percentage (0 to 100)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanPWM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, uint8_t duty_cycle);

/**
 * @brief   Sets the RPM target for the control algorithm. Works only with fan speed control (FSC) enabled
 * @param   chip EMC2305 to set
 * @param   fan Fan to set (1-5)
 * @param   rpm_target RPM target to set (500 to 16k)
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_SetFanRPM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan, uint16_t rpm_target);

// Status & Measurement Functions

/**
 * @brief   Gets current fan RPM based on tachometer measurement
 * @param   chip EMC2305 to get
 * @param   fan Fan to get (1-5)
 * @return  Measured fan RPM. UINT16_MAX on error
 */
uint16_t EMC2305_GetFanRPM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan);

/**
 * @brief   Gets current fan driver PWM duty cycle
 * @param   chip EMC2305 to get
 * @param   fan Fan to get (1-5)
 * @return  Driven fan PWM duty cycle (0-100). UINT8_MAX on error
 */
uint8_t EMC2305_GetFanPWM(EMC2305_HandleTypeDef* chip, EMC2305_Fan fan);

/**
 * @brief   Gets current fan status for all drivers
 * @param   chip EMC2305 to get
 * @return  Fan status for all drivers
 */
EMC2305_Fan_Status EMC2305_GetFanStatus(EMC2305_HandleTypeDef* chip);

// Register Read/Write Functions

/**
 * @brief   Reads a byte of data from the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to read from
 * @param   data Pointer where register data will be stored
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_ReadReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t* data);

/**
 * @brief   Writes a byte of data to the specified register
 * @param   chip EMC2305 handle
 * @param   reg Register to write to
 * @param   data Data to write
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_WriteReg(EMC2305_HandleTypeDef* chip, uint8_t reg, uint8_t data);

// Worker task to consume messages from the queue and send on I2C bus
void EMC2305_I2C_Worker_Task(void* pvParameters);
