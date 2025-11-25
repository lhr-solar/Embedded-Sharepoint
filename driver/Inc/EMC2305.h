// Driver for Microchip EMC2305 PWM Fan Controller

#pragma once

#include "stm32xx_hal.h"
#include <stdint.h>

// Device handle
typedef struct {
    I2C_HandleTypeDef* hi2c;   // STM32 HAL I2C handle
    uint16_t dev_addr;         // HAL convention: 7-bit address << 1
} EMC2305_HandleTypeDef;

// I2C Response Timeout
#ifndef EMC2305_I2C_TIMEOUT
#define EMC2305_I2C_TIMEOUT 100u // 100ms default
#endif

// Device status
typedef enum {
    EMC2305_OK,
    EMC2305_ERR,
} EMC2305_Status;

// Device configuration
typedef struct {
    // TODO(rshah): fill in config values
} EMC2305_Config;

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

// TODO(rshah): config 1/2 enums

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

// Fan 1
#define EMC2305_REG_FAN1_SETTING            0x30u // Fan 1 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM1_DIVIDE             0x31u // Fan 1 PWM Divide Register - The PWM Divide registers determine the final fre quency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
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

// Fan 2
#define EMC2305_REG_FAN2_SETTING           0x40u // Fan 2 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM2_DIVIDE            0x41u // Fan 2 PWM Divide Register - The PWM Divide registers determine the final frequency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
#define EMC2305_REG_FAN2_CONFIG1           0x42u // Fan 2 Configuration Register 1 - The Fan Configuration 1 registers control the general operation of the RPM-based Fan Speed Control algorithm used for the respective Fan Driver (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”).
#define EMC2305_REG_FAN2_CONFIG2           0x43u // Fan 2 Configuration Register 2 - The Fan Configuration 2 registers control the tachometer measurement and advanced features of the RPM based Fan Speed Control algorithm (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”)
#define EMC2305_REG_GAIN2                  0x45u // Fan 2 PID Gain Register - See Section 4.3.3.2 “Setting the PID Gains”.
#define EMC2305_REG_FAN2_SPIN              0x46u // Fan 2 Spin Up Configuration Registers - The Fan Spin Up Configuration registers control the settings of the Spin Up Routine. These registers are software locked (see Section 4.8 “Spin Up Routine”)
#define EMC2305_REG_FAN2_MAX_STEP          0x47u // Fan 2 Maximum Step Size Register - This register determines the maximum step size for the ramp rate control (see Section 4.3.3.3 “Fan Drive Max Step” and Section 4.10 “Ramp Rate Control”).
#define EMC2305_REG_FAN2_MIN_DRIVE         0x48u // Fan 2 Minimum Drive Register - The Fan Minimum Drive Register (see Section 4.3.3.4 “Minimum Drive Setting”) stores the minimum drive setting for each RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN2_VALID_TACH        0x49u // Fan 2 Valid TACH Count Register - The Valid TACH Count registers store the maximum TACH Reading register value to indicate that each fan is spinning properly (see Section 4.4.2 “Valid Tachometer Readings”).
#define EMC2305_REG_FAN2_DRVFAIL_L         0x4Au // Fan 2 Drive Fail Band Low Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN2_DRVFAIL_H         0x4Bu // Fan 2 Drive Fail Band High Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN2_TACH_TARGET_L     0x4Cu // Fan 2 Tachometer Closed Loop Target Low Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN2_TACH_TARGET_H     0x4Du // Fan 2 Tachometer Closed Loop Target High Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN2_TACH_READING_H    0x4Eu // Fan 2 Tachometer Reading High Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).
#define EMC2305_REG_FAN2_TACH_READING_L    0x4Fu // Fan 2 Tachometer Reading Low Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).

// Fan 3
#define EMC2305_REG_FAN3_SETTING           0x50u // Fan 3 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM3_DIVIDE            0x51u // Fan 3 PWM Divide Register - The PWM Divide registers determine the final frequency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
#define EMC2305_REG_FAN3_CONFIG1           0x52u // Fan 3 Configuration Register 1 - The Fan Configuration 1 registers control the general operation of the RPM-based Fan Speed Control algorithm used for the respective Fan Driver (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”).
#define EMC2305_REG_FAN3_CONFIG2           0x53u // Fan 3 Configuration Register 2 - The Fan Configuration 2 registers control the tachometer measurement and advanced features of the RPM based Fan Speed Control algorithm (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”)
#define EMC2305_REG_GAIN3                  0x55u // Fan 3 PID Gain Register - See Section 4.3.3.2 “Setting the PID Gains”.
#define EMC2305_REG_FAN3_SPIN              0x56u // Fan 3 Spin Up Configuration Registers - The Fan Spin Up Configuration registers control the settings of the Spin Up Routine. These registers are software locked (see Section 4.8 “Spin Up Routine”)
#define EMC2305_REG_FAN3_MAX_STEP          0x57u // Fan 3 Maximum Step Size Register - This register determines the maximum step size for the ramp rate control (see Section 4.3.3.3 “Fan Drive Max Step” and Section 4.10 “Ramp Rate Control”).
#define EMC2305_REG_FAN3_MIN_DRIVE         0x58u // Fan 3 Minimum Drive Register - The Fan Minimum Drive Register (see Section 4.3.3.4 “Minimum Drive Setting”) stores the minimum drive setting for each RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN3_VALID_TACH        0x59u // Fan 3 Valid TACH Count Register - The Valid TACH Count registers store the maximum TACH Reading register value to indicate that each fan is spinning properly (see Section 4.4.2 “Valid Tachometer Readings”).
#define EMC2305_REG_FAN3_DRVFAIL_L         0x5Au // Fan 3 Drive Fail Band Low Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN3_DRVFAIL_H         0x5Bu // Fan 3 Drive Fail Band High Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN3_TACH_TARGET_L     0x5Cu // Fan 3 Tachometer Closed Loop Target Low Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN3_TACH_TARGET_H     0x5Du // Fan 3 Tachometer Closed Loop Target High Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN3_TACH_READING_H    0x5Eu // Fan 3 Tachometer Reading High Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).
#define EMC2305_REG_FAN3_TACH_READING_L    0x5Fu // Fan 3 Tachometer Reading Low Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).

// Fan 4
#define EMC2305_REG_FAN4_SETTING           0x60u // Fan 4 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM4_DIVIDE            0x61u // Fan 4 PWM Divide Register - The PWM Divide registers determine the final frequency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
#define EMC2305_REG_FAN4_CONFIG1           0x62u // Fan 4 Configuration Register 1 - The Fan Configuration 1 registers control the general operation of the RPM-based Fan Speed Control algorithm used for the respective Fan Driver (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”).
#define EMC2305_REG_FAN4_CONFIG2           0x63u // Fan 4 Configuration Register 2 - The Fan Configuration 2 registers control the tachometer measurement and advanced features of the RPM based Fan Speed Control algorithm (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”)
#define EMC2305_REG_GAIN4                  0x65u // Fan 4 PID Gain Register - See Section 4.3.3.2 “Setting the PID Gains”.
#define EMC2305_REG_FAN4_SPIN              0x66u // Fan 4 Spin Up Configuration Registers - The Fan Spin Up Configuration registers control the settings of the Spin Up Routine. These registers are software locked (see Section 4.8 “Spin Up Routine”)
#define EMC2305_REG_FAN4_MAX_STEP          0x67u // Fan 4 Maximum Step Size Register - This register determines the maximum step size for the ramp rate control (see Section 4.3.3.3 “Fan Drive Max Step” and Section 4.10 “Ramp Rate Control”).
#define EMC2305_REG_FAN4_MIN_DRIVE         0x68u // Fan 4 Minimum Drive Register - The Fan Minimum Drive Register (see Section 4.3.3.4 “Minimum Drive Setting”) stores the minimum drive setting for each RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN4_VALID_TACH        0x69u // Fan 4 Valid TACH Count Register - The Valid TACH Count registers store the maximum TACH Reading register value to indicate that each fan is spinning properly (see Section 4.4.2 “Valid Tachometer Readings”).
#define EMC2305_REG_FAN4_DRVFAIL_L         0x6Au // Fan 4 Drive Fail Band Low Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN4_DRVFAIL_H         0x6Bu // Fan 4 Drive Fail Band High Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN4_TACH_TARGET_L     0x6Cu // Fan 4 Tachometer Closed Loop Target Low Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN4_TACH_TARGET_H     0x6Du // Fan 4 Tachometer Closed Loop Target High Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN4_TACH_READING_H    0x6Eu // Fan 4 Tachometer Reading High Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).
#define EMC2305_REG_FAN4_TACH_READING_L    0x6Fu // Fan 4 Tachometer Reading Low Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).

// Fan 5
#define EMC2305_REG_FAN5_SETTING           0x70u // Fan 5 Drive Setting Register - The Fan Drive Setting register always displays the current setting of the respective fan driver
#define EMC2305_REG_PWM5_DIVIDE            0x71u // Fan 5 PWM Divide Register - The PWM Divide registers determine the final frequency of the respective PWM Fan Driver. Each driver base frequency is divided by the value of the respective PWM Divide Register to determine the final frequency.
#define EMC2305_REG_FAN5_CONFIG1           0x72u // Fan 5 Configuration Register 1 - The Fan Configuration 1 registers control the general operation of the RPM-based Fan Speed Control algorithm used for the respective Fan Driver (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”).
#define EMC2305_REG_FAN5_CONFIG2           0x73u // Fan 5 Configuration Register 2 - The Fan Configuration 2 registers control the tachometer measurement and advanced features of the RPM based Fan Speed Control algorithm (see Section 4.3 “RPM-Based Fan Speed Control Algorithm”)
#define EMC2305_REG_GAIN5                  0x75u // Fan 5 PID Gain Register - See Section 4.3.3.2 “Setting the PID Gains”.
#define EMC2305_REG_FAN5_SPIN              0x76u // Fan 5 Spin Up Configuration Registers - The Fan Spin Up Configuration registers control the settings of the Spin Up Routine. These registers are software locked (see Section 4.8 “Spin Up Routine”)
#define EMC2305_REG_FAN5_MAX_STEP          0x77u // Fan 5 Maximum Step Size Register - This register determines the maximum step size for the ramp rate control (see Section 4.3.3.3 “Fan Drive Max Step” and Section 4.10 “Ramp Rate Control”).
#define EMC2305_REG_FAN5_MIN_DRIVE         0x78u // Fan 5 Minimum Drive Register - The Fan Minimum Drive Register (see Section 4.3.3.4 “Minimum Drive Setting”) stores the minimum drive setting for each RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN5_VALID_TACH        0x79u // Fan 5 Valid TACH Count Register - The Valid TACH Count registers store the maximum TACH Reading register value to indicate that each fan is spinning properly (see Section 4.4.2 “Valid Tachometer Readings”).
#define EMC2305_REG_FAN5_DRVFAIL_L         0x7Au // Fan 5 Drive Fail Band Low Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN5_DRVFAIL_H         0x7Bu // Fan 5 Drive Fail Band High Byte Register - The Drive Fail Band registers store the number of tach counts used by the Fan Drive Fail detection circuitry.
#define EMC2305_REG_FAN5_TACH_TARGET_L     0x7Cu // Fan 5 Tachometer Closed Loop Target Low Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN5_TACH_TARGET_H     0x7Du // Fan 5 Tachometer Closed Loop Target High Byte Register - The TACH Target Registers hold the target tachometer value that is maintained by the RPM-based Fan Speed Control algorithm.
#define EMC2305_REG_FAN5_TACH_READING_H    0x7Eu // Fan 5 Tachometer Reading High Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).
#define EMC2305_REG_FAN5_TACH_READING_L    0x7Fu // Fan 5 Tachometer Reading Low Byte Register - The TACH Reading Registers describe the current tachometer reading for each of the fans (see Section 4.4 “Tachometer Measurement”).

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

// Fan Stall (0x25) bits - F1..F5 stall
#define EMC2305_FAN_STALL_F1                (1u << 0)
#define EMC2305_FAN_STALL_F2                (1u << 1)
#define EMC2305_FAN_STALL_F3                (1u << 2)
#define EMC2305_FAN_STALL_F4                (1u << 3)
#define EMC2305_FAN_STALL_F5                (1u << 4)

// Fan Spin (0x26) bits - F1..F5 spin-up
#define EMC2305_FAN_SPIN_F1                 (1u << 0)
#define EMC2305_FAN_SPIN_F2                 (1u << 1)
#define EMC2305_FAN_SPIN_F3                 (1u << 2)
#define EMC2305_FAN_SPIN_F4                 (1u << 3)
#define EMC2305_FAN_SPIN_F5                 (1u << 4)

// Drive Fail (0x27) bits - F1..F5 drive fail
#define EMC2305_DRIVE_FAIL_F1               (1u << 0)
#define EMC2305_DRIVE_FAIL_F2               (1u << 1)
#define EMC2305_DRIVE_FAIL_F3               (1u << 2)
#define EMC2305_DRIVE_FAIL_F4               (1u << 3)
#define EMC2305_DRIVE_FAIL_F5               (1u << 4)

// Fan Interrupt Enable (0x29) bits
#define EMC2305_ITEN_F1                     (1u << 0)
#define EMC2305_ITEN_F2                     (1u << 1)
#define EMC2305_ITEN_F3                     (1u << 2)
#define EMC2305_ITEN_F4                     (1u << 3)
#define EMC2305_ITEN_F5                     (1u << 4)

// PWM polarity (0x2A) bits - PLRITYn: when set PWM polarity is inverted */
#define EMC2305_PLRTY_FAN1                  (1u << 0)
#define EMC2305_PLRTY_FAN2                  (1u << 1)
#define EMC2305_PLRTY_FAN3                  (1u << 2)
#define EMC2305_PLRTY_FAN4                  (1u << 3)
#define EMC2305_PLRTY_FAN5                  (1u << 4)

// PWM output type (0x2B) PMOTn: push-pull when set, open-drain when clear
#define EMC2305_PWMTYPE_FAN1                (1u << 0)
#define EMC2305_PWMTYPE_FAN2                (1u << 1)
#define EMC2305_PWMTYPE_FAN3                (1u << 2)
#define EMC2305_PWMTYPE_FAN4                (1u << 3)
#define EMC2305_PWMTYPE_FAN5                (1u << 4)

// Fan Config1 (ENAGx bit in each fan CONFIG1)
#define EMC2305_FAN_ENAG                    (1u << 7)  /* in each FANx CONFIG1: enable closed-loop FSC */

// Fan Config2 (ENRCx, GHENx, DPTx, ERGx in each fan CONFIG2)
#define EMC2305_FAN2_ENRC                   (1u << 6)  /* ENRCx ramp-rate enable when ENAGx == 0 */
#define EMC2305_FAN2_GHEN                   (1u << 5)  /* glitch filter enable (TACH pin) */
#define EMC2305_FAN2_DPT_MASK               (0x18u)    /* bits 4..3 derivative option */
#define EMC2305_FAN2_ERG_MASK               (0x06u)    /* bits 2..1 error window */

// Product ID
#define EMC2305_PRODUCT_ID_REG              0xFDu
#define EMC2305_MANUFACTURER_ID             0x5Du     /* read-only fixed per datasheet */
#define EMC2305_DEV_EMC2305_PID             0x34u     /* FD POR value for EMC2305 */

// /* Convenience constants */
// #define EMC2305_TACH_READING_H(reg_base)    (reg_base + 0x0Eu) /* e.g. FAN1: 0x3E; used for convert helpers */

// /* Notes:
//  * - Tach targets and readings are 13-bit-ish values stored across two registers (high and low).
//  * - PWM duty is 0..255 across Fan Setting registers.
//  * - Many registers are software-lockable (SWL). See datasheet for SWL behavior.
//  */

// /* Control register bits (placeholder) */
// #define EMC2305_CTRL_FAN_ENABLE (1u << 0)

// Device Management Functions

/**
 * @brief   Initializes the I2C bus and validates the chip by reading the Product ID and Manufacturer ID
 * @param   chip EMC2305 to initialize
 * @return  OK if successful, ERR otherwise
 */
EMC2305_Status EMC2305_Init(EMC2305_HandleTypeDef chip);

EMC2305_Status EMC2305_Reset(EMC2305_HandleTypeDef chip);

EMC2305_Status EMC2305_EnableSWLock(EMC2305_HandleTypeDef chip);

EMC2305_Status EMC2305_DisableSWLock(EMC2305_HandleTypeDef chip);

// TODO(rshah): should break into multiple fns
EMC2305_Status EMC2305_SetGlobalConfig(EMC2305_HandleTypeDef chip, EMC2305_Config config);

// Fan Configuration Functions

EMC2305_Status EMC2305_SetPWMBaseFrequency(EMC2305_HandleTypeDef chip, EMC2305_Fan fan, EMC2305_PWM_BaseFreq freq);

EMC2305_Status EMC2305_SetFanConfig(EMC2305_HandleTypeDef chip, EMC2305_Fan fan......

// Fan Control Functions

// Status & Measurement Functions
