// /*
//  * FanChip.c
//  * EMC2305 I2C (SMBus-style) driver for STM32L4 using HAL.


// #include "../Inc/FanChip.h"
// /* register map + masks defined there */
// #include <string.h>

// /* ---------- internal helpers (file-local) ---------- */

// static inline uint8_t fan_setting_reg(unsigned fan)
// {
//     static const uint8_t tbl[] = { 0x30u, 0x40u, 0x50u, 0x60u, 0x70u };
//     return (fan >= 1 && fan <= 5) ? tbl[fan - 1] : 0xFFu;
// }

// static inline uint8_t fan_tach_reading_h_reg(unsigned fan)
// {
//     static const uint8_t tbl[] = { 0x3Eu, 0x4Eu, 0x5Eu, 0x6Eu, 0x7Eu };
//     return (fan >= 1 && fan <= 5) ? tbl[fan - 1] : 0xFFu;
// }

// static inline uint8_t fan_tach_reading_l_reg(unsigned fan)
// {
//     static const uint8_t tbl[] = { 0x3Fu, 0x4Fu, 0x5Fu, 0x6Fu, 0x7Fu };
//     return (fan >= 1 && fan <= 5) ? tbl[fan - 1] : 0xFFu;
// }

// /* ---------- lifecycle ---------- */

// HAL_StatusTypeDef EMC2305_Open(EMC2305_HandleTypeDef *h, I2C_HandleTypeDef *hi2c, uint16_t dev_addr)
// {
//     if (h == NULL || hi2c == NULL) return HAL_ERROR;
//     h->hi2c = hi2c;
//     h->dev_addr = dev_addr; /* expect 7-bit address << 1 */
//     return HAL_OK;
// }

// void EMC2305_Close(EMC2305_HandleTypeDef *h)
// {
//     if (h == NULL) return;
//     memset(h, 0, sizeof(*h));
// }

// /* ---------- SMBus-style register access (HAL_I2C_Mem_*) ---------- */

// HAL_StatusTypeDef EMC2305_ReadReg(EMC2305_HandleTypeDef *h, uint8_t reg, uint8_t *value)
// {
//     if (h == NULL || h->hi2c == NULL || value == NULL) return HAL_ERROR;
//     return HAL_I2C_Mem_Read(h->hi2c, h->dev_addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, value, 1, EMC2305_I2C_TIMEOUT);
// }

// HAL_StatusTypeDef EMC2305_WriteReg(EMC2305_HandleTypeDef *h, uint8_t reg, uint8_t value)
// {
//     if (h == NULL || h->hi2c == NULL) return HAL_ERROR;
//     return HAL_I2C_Mem_Write(h->hi2c, h->dev_addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, &value, 1, EMC2305_I2C_TIMEOUT);
// }

// /* ---------- multi-byte helpers ---------- */

// static HAL_StatusTypeDef EMC2305_ReadRegs(EMC2305_HandleTypeDef *h, uint8_t reg, uint8_t *buf, uint16_t len)
// {
//     if (h == NULL || h->hi2c == NULL || buf == NULL || len == 0) return HAL_ERROR;
//     return HAL_I2C_Mem_Read(h->hi2c, h->dev_addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, buf, len, EMC2305_I2C_TIMEOUT);
// }

// static HAL_StatusTypeDef EMC2305_WriteRegs(EMC2305_HandleTypeDef *h, uint8_t reg, const uint8_t *buf, uint16_t len)
// {
//     if (h == NULL || h->hi2c == NULL || buf == NULL || len == 0) return HAL_ERROR;
//     return HAL_I2C_Mem_Write(h->hi2c, h->dev_addr, (uint16_t)reg, I2C_MEMADD_SIZE_8BIT, (uint8_t*)buf, len, EMC2305_I2C_TIMEOUT);
// }

// /* ---------- fan control API ---------- */

// /* Set PWM duty percent for a specific fan (1..5) */
// HAL_StatusTypeDef EMC2305_SetDutyPercentFan(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t percent)
// {
//     uint8_t reg = fan_setting_reg(fan);
//     if (reg == 0xFFu) return HAL_ERROR;
//     if (percent > 100u) percent = 100u;
//     uint8_t val = (uint8_t)((uint32_t)percent * 255u / 100u);
//     return EMC2305_WriteReg(h, reg, val);
// }

// /* Read current drive (0..255) */
// HAL_StatusTypeDef EMC2305_ReadDriveFan(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t *out_val)
// {
//     uint8_t reg = fan_setting_reg(fan);
//     if (reg == 0xFFu || out_val == NULL) return HAL_ERROR;
//     return EMC2305_ReadReg(h, reg, out_val);
// }

// /* Enable or disable closed-loop Fan Speed Control (ENAGx bit at FANx_CONFIG1) */
// HAL_StatusTypeDef EMC2305_SetFscEnabled(EMC2305_HandleTypeDef *h, unsigned fan, uint8_t enable)
// {
//     uint8_t cfg1_reg;
//     switch (fan) {
//         case 1: cfg1_reg = 0x32u; break;
//         case 2: cfg1_reg = 0x42u; break;
//         case 3: cfg1_reg = 0x52u; break;
//         case 4: cfg1_reg = 0x62u; break;
//         case 5: cfg1_reg = 0x72u; break;
//         default: return HAL_ERROR;
//     }
//     uint8_t v;
//     HAL_StatusTypeDef st = EMC2305_ReadReg(h, cfg1_reg, &v);
//     if (st != HAL_OK) return st;
//     if (enable) v |= (1u << 7); else v &= ~(1u << 7);
//     return EMC2305_WriteReg(h, cfg1_reg, v);
// }

// /* Mask or unmask ALERT (global config) */
// HAL_StatusTypeDef EMC2305_MaskAlert(EMC2305_HandleTypeDef *h, uint8_t mask)
// {
//     uint8_t v;
//     HAL_StatusTypeDef st = EMC2305_ReadReg(h, EMC2305_REG_CONFIGURATION, &v);
//     if (st != HAL_OK) return st;
//     if (mask) v |= EMC2305_CFG_MASK_ALERT; else v &= ~EMC2305_CFG_MASK_ALERT;
//     return EMC2305_WriteReg(h, EMC2305_REG_CONFIGURATION, v);
// }

// /* ---------- tachometer reading & conversion ---------- */

// /* Read combined tachometer raw count for fan (two-byte read). Returns masked 13-bit value. */
// HAL_StatusTypeDef EMC2305_ReadTachRawFan(EMC2305_HandleTypeDef *h, unsigned fan, uint16_t *out_count)
// {
//     if (out_count == NULL) return HAL_ERROR;
//     uint8_t reg_h = fan_tach_reading_h_reg(fan);
//     uint8_t reg_l = fan_tach_reading_l_reg(fan);
//     if (reg_h == 0xFFu || reg_l == 0xFFu) return HAL_ERROR;

//     uint8_t buf[2];
//     HAL_StatusTypeDef st = EMC2305_ReadRegs(h, reg_h, buf, 2);
//     if (st != HAL_OK) return st;

//     /* Combine the two bytes. Datasheet maps high: bits12..5 and low: bits4..0.
//        Combine then mask to 13 bits to be robust across variants. */
//     uint16_t combined = ((uint16_t)buf[0] << 8) | buf[1];
//     combined &= 0x1FFFu;
//     *out_count = combined;
//     return HAL_OK;
// }

// /* Convert raw tachometer count to RPM using datasheet constant for 32.768 kHz clock and two-pole, five-edge example.
//    Caller must supply range_multiplier (1,2,4,8) depending on RNGx setting for the fan.
//    If tach_count == 0 returns 0. */
// uint32_t EMC2305_TachCountToRpm(uint16_t tach_count, uint8_t range_multiplier)
// {
//     if (tach_count == 0) return 0;
//     if (range_multiplier == 0) range_multiplier = 1;
//     /* Using datasheet Equation 4-3: RPM = 3932160 / (m * COUNT) */
//     uint32_t numer = 3932160u / (uint32_t)range_multiplier;
//     return numer / (uint32_t)tach_count;
// }
