#include "BSP.h"
#include "bsp_config.h"
#include <stdbool.h>

#define MAX_PIN_AF 3

// see bsp_config.c
extern const uint8_t* BSP_GPIO_MAPPING;
// remapping
static const uint8_t remap[NUM_PERIPHS] = {
	[BSP_GPIO] = 0,
	[BSP_UART4] = GPIO_AF8_UART4,
    [BSP_UART5] = GPIO_AF11_UART5,
	[BSP_TIM1] = GPIO_AF1_TIM1,
	[BSP_TIM2] = GPIO_AF1_TIM2,
	[BSP_TIM3] = GPIO_AF2_TIM3,
	[BSP_TIM9] = GPIO_AF3_TIM9,
	[BSP_I2C1] = GPIO_AF4_I2C1,
	[BSP_I2C3] = GPIO_AF4_I2C3,
	[BSP_SPI2] = GPIO_AF5_SPI2,
	[BSP_SPI3] = GPIO_AF6_SPI3,
	[BSP_CAN2] = GPIO_AF9_CAN2,
	[BSP_CAN3] = GPIO_AF11_CAN3
};

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
        final_pin_value = (uint8_t)pin - BSP_GPIO_PB0;
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


/**
 * @brief Configures the BSP layer according to the configuration specified in bsp_config.h
 * 
 * @details This function configures the BSP peripheral functions according to the alternate function
 * mapping specified in bsp_config.h
 * 
 * The validity of your configuration is checked at runtime. Each pin's alternate function value is mapped
 * into an array in memory, indexed by the pin, with a value of BSP_XXX alternate function value. This returned
 * value is used as a secondary index into a LUT that translates BSP_XXX alternate function enum value into the 
 * actual HAL alternate function value, allowing us to loop over the entire array of pins to configure their
 * GPIO/AF properly, while only requiring modifications to the bsp_config.h file.
 * 
 */
void BSP_Init() {

    // Set up the basic parameters for global handles
    // These should be referred to when possible and configured
    // accordingly in the peripheral functions
    uart4_handle.Instance = UART4;
    uart5_handle.Instance = UART5;
    tim1_handle.Instance = TIM1;
    tim2_handle.Instance = TIM2;
    tim3_handle.Instance = TIM3;
    tim9_handle.Instance = TIM9;
    i2c1_handle.Instance = I2C1;
    i2c3_handle.Instance = I2C3;
    spi2_handle.Instance = SPI2;
    spi3_handle.Instance = SPI3;
    carcan_handle.Instance = CAN2;
    localcan_handle.Instance = CAN3;


    GPIO_InitTypeDef gpio;
    gpio.Speed = GPIO_SPEED_FAST;
    gpio.Mode = GPIO_MODE_AF_PP;

    for (uint8_t i = 0; i < NUM_BSP_PINS; i++) {
        GPIO_TypeDef* port = GET_GPIO(i);
        uint8_t pin_af = BSP_GPIO_MAPPING[i];
        // invalid mapping, todo: handle
        if (!isValidPinMapping((BSP_PINS)i, pin_af)) {
            return -1; 
        }
        gpio.Alternate = remap[pin_af];
        gpio.Pin = PinFromEnum((BSP_PINS)i);
        // CAN and I2C use open drain, not push pull
        if (pin_af == BSP_CAN2 || pin_af == BSP_CAN3 || pin_af == BSP_I2C1 || pin_af == BSP_I2C3) {
            gpio.Mode = GPIO_MODE_AF_OD;
        }
        // init the GPIO pin
        HAL_GPIO_Init(port, &gpio);
        // custom peripheral initialization here
        switch (BSP_GPIO_MAPPING[i]) {
            case BSP_UART4:
                break;
            case BSP_UART5:
                break;
            case BSP_TIM1:
                break;
            case BSP_TIM2:
                break;
            case BSP_TIM3:
                break;
            case BSP_TIM9:
                break;
            case BSP_I2C1:
                break;
            case BSP_I2C3:
                break;
            case BSP_SPI2:
                break;
            case BSP_SPI3:
                break;
            case BSP_CAN2:
                break;
            case BSP_CAN3:
                break;
        }
        // reset to push pull
        gpio.Mode = GPIO_MODE_AF_PP;
    }
}