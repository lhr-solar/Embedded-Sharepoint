#include "BSP.h"
#include "bsp_config.h"
#include <stdbool.h>

#define MAX_PIN_AF 8

// see bsp_config.c
extern const uint8_t* BSP_GPIO_MAPPING;

// LUT for all valid AF pin mappings

// https://www.st.com/resource/en/datasheet/stm32f413cg.pdf page 67
// SPI1, SPI2, USART1/2, I2C1/2
// https://utexas.sharepoint.com/:x:/r/sites/ENGR-LonghornRacing/_layouts/15/Doc.aspx?sourcedoc=%7B1CE08B10-B376-4BEB-83E4-BC72F106B36F%7D&file=LeaderSOM%20MCU%20pin%20assignment.xlsx&action=default&mobileredirect=true
static const uint8_t VALID_AF_MAP[NUM_BSP_PINS][MAX_PIN_AF] = {
    [BSP_GPIO_PA0] = {GPIO_AF8_UART4}, //uart4
    [BSP_GPIO_PA1] = {GPIO_AF8_UART4}, //uart4
    [BSP_GPIO_PA2] = {GPIO_AF1_TIM2, GPIO_AF3_TIM9}, //gpio/adc
    [BSP_GPIO_PA3] = {GPIO_AF1_TIM2, GPIO_AF3_TIM9},
    [BSP_GPIO_PA4] = {}, //gpio
    [BSP_GPIO_PA5] = {}, //gpio
    [BSP_GPIO_PA6] = {}, //gpio
    [BSP_GPIO_PA7] = {GPIO_AF2_TIM3},
    [BSP_GPIO_PA8] = {GPIO_AF1_TIM1, GPIO_AF4_I2C3}, 
    [BSP_GPIO_PA9] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2},
    [BSP_GPIO_PA10] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2},
    [BSP_GPIO_PA11] = {GPIO_AF1_TIM1, GPIO_AF5_SPI2},
    [BSP_GPIO_PA12] = {GPIO_AF5_SPI2},
    [BSP_GPIO_PA13] = {}, //gpio/unused?
    [BSP_GPIO_PA14] = {}, //gpio
    [BSP_GPIO_PA15] = {}, //gpio
    [BSP_GPIO_PB0] = {}, //disp7
    [BSP_GPIO_PB1] = {}, //disp8
    [BSP_GPIO_PB2] = {}, //disp9
    [BSP_GPIO_PB3] = {GPIO_AF11_CAN3},
    [BSP_GPIO_PB4] = {GPIO_AF11_CAN3},
    [BSP_GPIO_PB5] = {GPIO_AF9_CAN2},
    [BSP_GPIO_PB6] = {GPIO_AF9_CAN2},
    [BSP_GPIO_PB7] = {}, //gpio
    [BSP_GPIO_PB8] = {GPIO_AF4_I2C1},
    [BSP_GPIO_PB9] = {GPIO_AF4_I2C1},
    [BSP_GPIO_PB10] = {}, //disp10
    [BSP_GPIO_PB11] = {},
    [BSP_GPIO_PB12] = {GPIO_AF11_UART5},
    [BSP_GPIO_PB13] = {GPIO_AF11_UART5},
    [BSP_GPIO_PB14] = {},  //gpio
    [BSP_GPIO_PB15] = {}, //gpio
    [BSP_GPIO_PC0] = {}, //disp1
    [BSP_GPIO_PC1] = {}, //disp2
    [BSP_GPIO_PC2] = {}, //disp3
    [BSP_GPIO_PC3] = {}, //disp4
    [BSP_GPIO_PC4] = {}, //disp5
    [BSP_GPIO_PC5] = {}, //disp6
    [BSP_GPIO_PC6] = {}, //disp13
    [BSP_GPIO_PC7] = {}, //disp14
    [BSP_GPIO_PC8] = {}, //gpio
    [BSP_GPIO_PC9] = {GPIO_AF4_I2C3}, 
    [BSP_GPIO_PC10] = {GPIO_AF6_SPI3},
    [BSP_GPIO_PC11] = {GPIO_AF6_SPI3},
    [BSP_GPIO_PC12] = {GPIO_AF6_SPI3},
    [BSP_GPIO_PC13] = {},
    [BSP_GPIO_PD2] = {}
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

void bsp_init_gpio() {
    GPIO_InitTypeDef init_struct;

    for (uint8_t i = 0; i < NUM_BSP_PINS; i++) {
        GPIO_TypeDef* port = GET_GPIO(i);
    }
}