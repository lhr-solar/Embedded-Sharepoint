/*
 * File: FanChip.h
 * Simple EMC2305 I2C driver for STM32L4 using HAL.
 */

#ifndef FANCHIP_H_
#define FANCHIP_H_

#include "stm32l4xx_hal.h"
#include <stdint.h>


/* Public handle (opaque-friendly but concrete) */
typedef struct {
    I2C_HandleTypeDef *hi2c;   /* STM32 HAL I2C handle */
    uint16_t dev_addr;         /* HAL convention: 7-bit address << 1 */
} EMC2305_HandleTypeDef;

/* Lifecycle */
HAL_StatusTypeDef EMC2305_Open(EMC2305_HandleTypeDef *h, I2C_HandleTypeDef *hi2c, uint16_t dev_addr);
void             EMC2305_Close(EMC2305_HandleTypeDef *h);

/* Basic register helpers */
HAL_StatusTypeDef EMC2305_ReadReg(EMC2305_HandleTypeDef *h, uint8_t reg, uint8_t *value);
HAL_StatusTypeDef EMC2305_WriteReg(EMC2305_HandleTypeDef *h, uint8_t reg, uint8_t value);

/* Fan control (fan index 1..5) */
HAL_StatusTypeDef EMC2305_SetDutyPercentFan(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t percent);
HAL_StatusTypeDef EMC2305_ReadDriveFan(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t *out_val);
HAL_StatusTypeDef EMC2305_SetFscEnabled(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t enable);

/* Tachometer */
HAL_StatusTypeDef EMC2305_ReadTachRawFan(EMC2305_HandleTypeDef *h, unsigned fan, uint16_t *out_count);
/* Convert raw tach count to RPM.
   range_multiplier = 1,2,4,8 depending on RNGx bits (see datasheet) */
uint32_t          EMC2305_TachCountToRpm(uint16_t tach_count, uint8_t range_multiplier);

/* Misc */
HAL_StatusTypeDef EMC2305_MaskAlert(EMC2305_HandleTypeDef *h, uint8_t mask);


/* Config */
#ifndef EMC2305_I2C_TIMEOUT
#define EMC2305_I2C_TIMEOUT 100u
#endif

/* Register map -- replace with values from the EMC2305 datasheet */
/* FanChip_regs.h  -- register map / bit masks for EMC2305 (EMC2301/2/3/5 family) */

/* Registers (hex) */
#define EMC2305_REG_CONFIGURATION           0x20u
#define EMC2305_REG_FAN_STATUS              0x24u
#define EMC2305_REG_FAN_STALL_STATUS        0x25u
#define EMC2305_REG_FAN_SPIN_STATUS         0x26u
#define EMC2305_REG_DRIVE_FAIL_STATUS       0x27u
#define EMC2305_REG_FAN_INTERRUPT_ENABLE    0x29u
#define EMC2305_REG_PWM_POLARITY            0x2Au
#define EMC2305_REG_PWM_OUTPUT_CONFIG       0x2Bu
#define EMC2305_REG_PWM_BASEF45             0x2Cu
#define EMC2305_REG_PWM_BASEF123            0x2Du

/* Fan drive setting registers (read/write in Direct mode, read-only in FSC) */
#define EMC2305_REG_FAN1_SETTING            0x30u
#define EMC2305_REG_PWM1_DIVIDE             0x31u
#define EMC2305_REG_FAN1_CONFIG1            0x32u
#define EMC2305_REG_FAN1_CONFIG2            0x33u
#define EMC2305_REG_GAIN1                   0x35u
#define EMC2305_REG_FAN1_SPIN               0x36u
#define EMC2305_REG_FAN1_MAX_STEP           0x37u
#define EMC2305_REG_FAN1_MIN_DRIVE          0x38u
#define EMC2305_REG_FAN1_VALID_TACH         0x39u
#define EMC2305_REG_FAN1_DRVFAIL_L          0x3Au
#define EMC2305_REG_FAN1_DRVFAIL_H          0x3Bu
#define EMC2305_REG_FAN1_TACH_TARGET_L      0x3Cu
#define EMC2305_REG_FAN1_TACH_TARGET_H      0x3Du
#define EMC2305_REG_FAN1_TACH_READING_H     0x3Eu
#define EMC2305_REG_FAN1_TACH_READING_L     0x3Fu

#define EMC2305_REG_FAN2_SETTING            0x40u
#define EMC2305_REG_PWM2_DIVIDE             0x41u
#define EMC2305_REG_FAN2_CONFIG1            0x42u
#define EMC2305_REG_FAN2_CONFIG2            0x43u
#define EMC2305_REG_GAIN2                   0x45u
#define EMC2305_REG_FAN2_SPIN               0x46u
#define EMC2305_REG_FAN2_MAX_STEP           0x47u
#define EMC2305_REG_FAN2_MIN_DRIVE          0x48u
#define EMC2305_REG_FAN2_VALID_TACH         0x49u
#define EMC2305_REG_FAN2_DRVFAIL_L          0x4Au
#define EMC2305_REG_FAN2_DRVFAIL_H          0x4Bu
#define EMC2305_REG_FAN2_TACH_TARGET_L      0x4Cu
#define EMC2305_REG_FAN2_TACH_TARGET_H      0x4Du
#define EMC2305_REG_FAN2_TACH_READING_H     0x4Eu
#define EMC2305_REG_FAN2_TACH_READING_L     0x4Fu

#define EMC2305_REG_FAN3_SETTING            0x50u
#define EMC2305_REG_PWM3_DIVIDE             0x51u
#define EMC2305_REG_FAN3_CONFIG1            0x52u
#define EMC2305_REG_FAN3_CONFIG2            0x53u
#define EMC2305_REG_GAIN3                   0x55u
#define EMC2305_REG_FAN3_SPIN               0x56u
#define EMC2305_REG_FAN3_MAX_STEP           0x57u
#define EMC2305_REG_FAN3_MIN_DRIVE          0x58u
#define EMC2305_REG_FAN3_VALID_TACH         0x59u
#define EMC2305_REG_FAN3_DRVFAIL_L          0x5Au
#define EMC2305_REG_FAN3_DRVFAIL_H          0x5Bu
#define EMC2305_REG_FAN3_TACH_TARGET_L      0x5Cu
#define EMC2305_REG_FAN3_TACH_TARGET_H      0x5Du
#define EMC2305_REG_FAN3_TACH_READING_H     0x5Eu
#define EMC2305_REG_FAN3_TACH_READING_L     0x5Fu

#define EMC2305_REG_FAN4_SETTING            0x60u
#define EMC2305_REG_PWM4_DIVIDE             0x61u
#define EMC2305_REG_FAN4_CONFIG1            0x62u
#define EMC2305_REG_FAN4_CONFIG2            0x63u
#define EMC2305_REG_GAIN4                   0x65u
#define EMC2305_REG_FAN4_SPIN               0x66u
#define EMC2305_REG_FAN4_MAX_STEP           0x67u
#define EMC2305_REG_FAN4_MIN_DRIVE          0x68u
#define EMC2305_REG_FAN4_VALID_TACH         0x69u
#define EMC2305_REG_FAN4_DRVFAIL_L          0x6Au
#define EMC2305_REG_FAN4_DRVFAIL_H          0x6Bu
#define EMC2305_REG_FAN4_TACH_TARGET_L      0x6Cu
#define EMC2305_REG_FAN4_TACH_TARGET_H      0x6Du
#define EMC2305_REG_FAN4_TACH_READING_H     0x6Eu
#define EMC2305_REG_FAN4_TACH_READING_L     0x6Fu

#define EMC2305_REG_FAN5_SETTING            0x70u
#define EMC2305_REG_PWM5_DIVIDE             0x71u
#define EMC2305_REG_FAN5_CONFIG1            0x72u
#define EMC2305_REG_FAN5_CONFIG2            0x73u
#define EMC2305_REG_GAIN5                   0x75u
#define EMC2305_REG_FAN5_SPIN               0x76u
#define EMC2305_REG_FAN5_MAX_STEP           0x77u
#define EMC2305_REG_FAN5_MIN_DRIVE          0x78u
#define EMC2305_REG_FAN5_VALID_TACH         0x79u
#define EMC2305_REG_FAN5_DRVFAIL_L          0x7Au
#define EMC2305_REG_FAN5_DRVFAIL_H          0x7Bu
#define EMC2305_REG_FAN5_TACH_TARGET_L      0x7Cu
#define EMC2305_REG_FAN5_TACH_TARGET_H      0x7Du
#define EMC2305_REG_FAN5_TACH_READING_H     0x7Eu
#define EMC2305_REG_FAN5_TACH_READING_L     0x7Fu

/* Software lock, product features and identification */
#define EMC2305_REG_SW_LOCK                 0xEFu
#define EMC2305_REG_PRODUCT_FEAT            0xFCu
#define EMC2305_REG_PRODUCT_ID              0xFDu
#define EMC2305_REG_MANUFACTURER_ID         0xFEu
#define EMC2305_REG_REVISION                0xFFu

/* ---------------------- Useful bit masks ---------------------- */
/* CONFIG (0x20) bits */
#define EMC2305_CFG_MASK_ALERT              (1u << 7)  /* MASK - mask ALERT pin when set */
#define EMC2305_CFG_DIS_TO                  (1u << 6)  /* DIS_TO - SMBus timeout disable (I2C compat) */
#define EMC2305_CFG_WD_EN                   (1u << 5)  /* WD_EN - enable watchdog continuous mode */
#define EMC2305_CFG_DRECK                   (1u << 1)  /* DRECK - CLK pin drives internal clock when set (output) */
#define EMC2305_CFG_USECK                   (1u << 0)  /* USECK - use external CLK pin as tach clock when set */

/* FAN STATUS (0x24) bits */
#define EMC2305_STAT_WATCH                  (1u << 7)  /* WATCH - watchdog fired (read-to-clear) */
#define EMC2305_STAT_DRVFAIL                (1u << 2)  /* Drive fail summary */
#define EMC2305_STAT_FNSPIN                 (1u << 1)  /* spin-up failure summary */
#define EMC2305_STAT_FNSTL                  (1u << 0)  /* stall summary */

/* FAN STALL (0x25) bits - F1..F5 stall */
#define EMC2305_FAN_STALL_F1                (1u << 0)
#define EMC2305_FAN_STALL_F2                (1u << 1)
#define EMC2305_FAN_STALL_F3                (1u << 2)
#define EMC2305_FAN_STALL_F4                (1u << 3)
#define EMC2305_FAN_STALL_F5                (1u << 4)

/* FAN SPIN (0x26) bits - F1..F5 spin-up */
#define EMC2305_FAN_SPIN_F1                 (1u << 0)
#define EMC2305_FAN_SPIN_F2                 (1u << 1)
#define EMC2305_FAN_SPIN_F3                 (1u << 2)
#define EMC2305_FAN_SPIN_F4                 (1u << 3)
#define EMC2305_FAN_SPIN_F5                 (1u << 4)

/* DRIVE FAIL (0x27) bits - F1..F5 drive fail */
#define EMC2305_DRIVE_FAIL_F1               (1u << 0)
#define EMC2305_DRIVE_FAIL_F2               (1u << 1)
#define EMC2305_DRIVE_FAIL_F3               (1u << 2)
#define EMC2305_DRIVE_FAIL_F4               (1u << 3)
#define EMC2305_DRIVE_FAIL_F5               (1u << 4)

/* FAN INTERRUPT ENABLE (0x29) bits */
#define EMC2305_ITEN_F1                     (1u << 0)
#define EMC2305_ITEN_F2                     (1u << 1)
#define EMC2305_ITEN_F3                     (1u << 2)
#define EMC2305_ITEN_F4                     (1u << 3)
#define EMC2305_ITEN_F5                     (1u << 4)

/* PWM polarity (0x2A) bits - PLRITYn: when set polarity inverted */
#define EMC2305_PLRTY_FAN1                  (1u << 0)
#define EMC2305_PLRTY_FAN2                  (1u << 1)
#define EMC2305_PLRTY_FAN3                  (1u << 2)
#define EMC2305_PLRTY_FAN4                  (1u << 3)
#define EMC2305_PLRTY_FAN5                  (1u << 4)

/* PWM output type (0x2B) PMOTn: push-pull when set, open-drain when clear */
#define EMC2305_PWMTYPE_FAN1                (1u << 0)
#define EMC2305_PWMTYPE_FAN2                (1u << 1)
#define EMC2305_PWMTYPE_FAN3                (1u << 2)
#define EMC2305_PWMTYPE_FAN4                (1u << 3)
#define EMC2305_PWMTYPE_FAN5                (1u << 4)

/* Fan Config1 (ENAGx bit in each fan CONFIG1) */
#define EMC2305_FAN_ENAG                    (1u << 7)  /* in each FANx CONFIG1: enable closed-loop FSC */

/* Fan Config2 (ENRCx, GHENx, DPTx, ERGx) helpers */
#define EMC2305_FAN2_ENRC                   (1u << 6)  /* ENRCx ramp-rate enable when ENAGx == 0 */
#define EMC2305_FAN2_GHEN                   (1u << 5)  /* glitch filter enable (TACH pin) */
#define EMC2305_FAN2_DPT_MASK               (0x18u)    /* bits 4..3 derivative option */
#define EMC2305_FAN2_ERG_MASK               (0x06u)    /* bits 2..1 error window */

/* Product / ID */
#define EMC2305_PRODUCT_ID_REG              0xFDu
#define EMC2305_MANUFACTURER_ID             0x5Du     /* read-only fixed per datasheet */
#define EMC2305_DEV_EMC2305_PID             0x34u     /* FD POR value for EMC2305 */

/* Convenience constants */
#define EMC2305_TACH_READING_H(reg_base)    (reg_base + 0x0Eu) /* e.g. FAN1: 0x3E; used for convert helpers */

/* Notes:
 * - Tach targets and readings are 13-bit-ish values stored across two registers (high and low).
 * - PWM duty is 0..255 across Fan Setting registers.
 * - Many registers are software-lockable (SWL). See datasheet for SWL behavior.
 */



/* Control register bits (placeholder) */
#define EMC2305_CTRL_FAN_ENABLE (1u << 0)

#endif /* FANCHIP_H_ */