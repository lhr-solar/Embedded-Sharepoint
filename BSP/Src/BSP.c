#include "BSP.h"
#include "bsp_config.h"
#include <stdbool.h>

#define MAX_PIN_AF 8

// see bsp_config.c
extern const uint8_t* BSP_GPIO_MAPPING;

// LUT for all valid AF pin mappings

// https://www.st.com/resource/en/datasheet/stm32f413cg.pdf page 67
static const uint8_t VALID_AF_MAP[NUM_BSP_PINS][MAX_PIN_AF] = {
    [BSP_GPIO_PA0] = {GPIO_AF1_TIM2, GPIO_AF2_TIM5, GPIO_AF3_TIM8, GPIO_AF7_USART2, GPIO_AF8_UART4},
    [BSP_GPIO_PA1] = {GPIO_AF1_TIM2, GPIO_AF2_TIM5, GPIO_AF5_SPI4, GPIO_AF7_USART2, GPIO_AF8_UART4},
    [BSP_GPIO_PA2] = {GPIO_AF1_TIM2, GPIO_AF2_TIM5, GPIO_AF3_TIM9, GPIO_AF7_USART2, NULL}, // we dont use i2s or fsmc
    [BSP_GPIO_PA3] = {GPIO_AF1_TIM2, GPIO_AF2_TIM5, GPIO_AF3_TIM9, GPIO_AF7_USART2, NULL}, // same as above
    [BSP_GPIO_PA4] = {GPIO_AF5_SPI1, GPIO_AF5_SPI3, GPIO_AF6_SPI3, GPIO_AF7_USART2, NULL},
    [BSP_GPIO_PA5] = {GPIO_AF1_TIM2, GPIO_AF3_TIM8, GPIO_AF5_SPI1, NULL, NULL},
    [BSP_GPIO_PA6] = {GPIO_AF1_TIM1, GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF5_SPI1, GPIO_AF9_TIM13},
    [BSP_GPIO_PA7] = {GPIO_AF1_TIM1, GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF5_SPI1, GPIO_AF9_TIM14},
    [BSP_GPIO_PA8] = {GPIO_AF1_TIM1, GPIO_AF4_I2C3, GPIO_AF7_USART1, GPIO_AF8_UART7, GPIO_AF11_CAN3}, // won't be using MCO
    [BSP_GPIO_PA9] = {GPIO_AF1_TIM1, GPIO_AF4_I2C3, GPIO_AF5_SPI2, GPIO_AF7_USART1, NULL},
    [BSP_GPIO_PA10] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2, GPIO_AF6_SPI5, GPIO_AF7_USART1, NULL},
    [BSP_GPIO_PA11] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2, GPIO_AF6_SPI4, GPIO_AF7_USART1, GPIO_AF8_USART6, GPIO_AF9_CAN1, GPIO_AF11_UART4},
    [BSP_GPIO_PA12] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2, GPIO_AF6_SPI5, GPIO_AF7_USART1, GPIO_AF8_USART6, GPIO_AF9_CAN1, GPIO_AF11_UART4},
    [BSP_GPIO_PA13] = {},
    [BSP_GPIO_PA14] = {},
    [BSP_GPIO_PA15] = {GPIO_AF1_TIM2, GPIO_AF5_SPI1, GPIO_AF6_SPI3, GPIO_AF7_USART1, GPIO_AF8_UART7, GPIO_AF11_CAN3},
    [BSP_GPIO_PB0] = {GPIO_AF1_TIM1, GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF6_SPI5},
    [BSP_GPIO_PB1] = {GPIO_AF1_TIM1, GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF6_SPI5},
    [BSP_GPIO_PB2] = {},
    [BSP_GPIO_PB3] = {GPIO_AF1_TIM2, GPIO_AF5_SPI1, GPIO_AF6_SPI3, GPIO_AF7_USART1, GPIO_AF8_UART7, GPIO_AF9_I2C2, GPIO_AF11_CAN3},
    [BSP_GPIO_PB4] = {GPIO_AF2_TIM3, GPIO_AF5_SPI1, GPIO_AF6_SPI3, GPIO_AF8_UART7, GPIO_AF9_I2C3, GPIO_AF11_CAN3},
    [BSP_GPIO_PB5] = {GPIO_AF2_TIM3, GPIO_AF4_I2C1, GPIO_AF5_SPI1, GPIO_AF6_SPI3, GPIO_AF9_CAN2, GPIO_AF11_UART5},
    [BSP_GPIO_PB6] = {GPIO_AF2_TIM4, GPIO_AF4_I2C1, GPIO_AF7_USART1, GPIO_AF9_CAN2, GPIO_AF11_UART5},
    [BSP_GPIO_PB7] = {GPIO_AF2_TIM4, GPIO_AF4_I2C1, GPIO_AF7_USART1},
    [BSP_GPIO_PB8] = {GPIO_AF2_TIM4, GPIO_AF3_TIM10, GPIO_AF4_I2C1, GPIO_AF6_SPI5, GPIO_AF8_CAN1, GPIO_AF9_I2C3, GPIO_AF11_UART5},
    [BSP_GPIO_PB9] = {GPIO_AF2_TIM4, GPIO_AF3_TIM11, GPIO_AF4_I2C1, GPIO_AF5_SPI2, GPIO_AF8_CAN1, GPIO_AF9_I2C2, GPIO_AF11_UART5},
    [BSP_GPIO_PB10] = {GPIO_AF1_TIM2, GPIO_AF4_I2C2, GPIO_AF5_SPI2, GPIO_AF7_USART3},
    // PB11 not on our chip
    [BSP_GPIO_PB12] = {GPIO_AF1_TIM1, GPIO_AF4_I2C2, GPIO_AF5_SPI2, GPIO_AF6_SPI4, GPIO_AF7_SPI3, GPIO_AF8_USART3, GPIO_AF9_CAN2, GPIO_AF11_UART5},
    [BSP_GPIO_PB13] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2, GPIO_AF6_SPI4, GPIO_AF8_USART3, GPIO_AF9_CAN2, GPIO_AF11_UART5},
    [BSP_GPIO_PB14] = {GPIO_AF1_TIM1, GPIO_AF3_TIM8, GPIO_AF5_SPI2, GPIO_AF7_USART3, GPIO_AF9_TIM12},
    [BSP_GPIO_PB15] = {GPIO_AF1_TIM1, GPIO_AF3_TIM8, GPIO_AF5_SPI2, GPIO_AF9_TIM12},
    [BSP_GPIO_PC0] = {},
    [BSP_GPIO_PC1] = {},
    [BSP_GPIO_PC2] = {GPIO_AF5_SPI2},
    [BSP_GPIO_PC3] = {GPIO_AF5_SPI2},
    [BSP_GPIO_PC4] = {},
    [BSP_GPIO_PC5] = {GPIO_AF7_USART3},
    [BSP_GPIO_PC6] = {GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF5_SPI2, GPIO_AF8_USART6},
    [BSP_GPIO_PC7] = {GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF5_SPI2, GPIO_AF8_USART6},
    [BSP_GPIO_PC8] = {GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF8_USART6},
    [BSP_GPIO_PC9] = {GPIO_AF2_TIM3, GPIO_AF3_TIM8, GPIO_AF4_I2C3},
    [BSP_GPIO_PC10] = {GPIO_AF6_SPI3, GPIO_AF7_USART3},
    [BSP_GPIO_PC11] = {GPIO_AF6_SPI3, GPIO_AF7_USART3, GPIO_AF8_UART4},
    [BSP_GPIO_PC12] = {GPIO_AF6_SPI3, GPIO_AF7_USART3, GPIO_AF8_UART5},
    [BSP_GPIO_PC13] = {},
    [BSP_GPIO_PD2] = {GPIO_AF2_TIM3, GPIO_AF8_UART5}
};

// Get HAL GPIO_PIN_XX from an enumerated pin
uint8_t PinFromEnum(BSP_PINS pin) {
    GPIO_TypeDef* pin_port = GET_GPIO(pin);
    uint8_t final_pin_value;
    if (pin_port == GPIOA) {
        final_pin_value = (uint8_t)pin;
    } else if (pin_port == GPIOB) {
        // we don't have PB11 on our chip so do some edge case handling
        if (pin <= BSP_GPIO_PB10) {
            final_pin_value = (uint8_t)pin - BSP_GPIO_PB0;
        }
        else {
            final_pin_value = ((uint8_t)pin - BSP_GPIO_PB0) + 1;
            // PB12 -> (27) - (16) + 1 = 12, good
        }
    } else if (pin_port == GPIOC) {
        final_pin_value = (uint8_t)pin - BSP_GPIO_PC0;
    } else if (pin_port == GPIOD || pin == BSP_GPIO_PD2) {
        final_pin_value = 2;
    }
    return final_pin_value;
}

// Determines (runtime) whether or not a pin AF mapping is valid
bool isValidPinMapping(BSP_PINS pin, uint8_t mapped_function) {
    for (uint8_t i = 0; i < MAX_PIN_AF; i++) {
        if (VALID_AF_MAP[pin][i] == mapped_function) return true;
    }
    return false; // didn't match on any of the possible AFs for that pin, so bad mapping
}

//for pin in pin2periph:
//    if pin.af == i2c_shit:
//        i2c_init(pin)
// per Tianda

/**
 * TODO: We need a better way of mapping AFs to pins
 *       besides using the builtin HAL #defines, as
 *       each AF(X) for 0 to 15 is the same value (i.e., AF4_UART = 4 = AF4_SPI)
 *       this is fine for GPIO init, but NOT fine for actual periphery initialization
 *       
 *       Perhaps (another) custom #define or enum that specifies.
 *       Edit: yes we DEFINITELY need a custom enum that specifies the AF type instead of just the #defines.
 *             CAN/Timer AFs share some values, but CAN and I2C need OD while others need PP mode.
 */

void BSP_Pin_AF_Init() {
    GPIO_InitTypeDef pinInitStruct;

    for (uint8_t i = 0; i < NUM_BSP_PINS; i++) {
        // If not valid mapping, crash (crash? how do we notify bad mapping if on board?)
        if (!isValidPinMapping((BSP_PINS)i, BSP_GPIO_MAPPING[i])) {
            return;
        }
        // I2C has to be open drain
        // Lucky for us, all the valid I2C peripherals
        // we can use are on AF4, so check for "4" value
        pinInitStruct.Mode = (BSP_GPIO_MAPPING[i] != 0) ? GPIO_MODE_AF_PP : GPIO_MODE_OUTPUT_PP;
        if (BSP_GPIO_MAPPING[i] == 0x4U) {
            pinInitStruct.Mode = GPIO_MODE_AF_OD;
        }
    }
}