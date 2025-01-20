#ifndef STM32XX_HAL_DEF_H
#define STM32XX_HAL_DEF_H

#include <stdint.h>
#include <stdbool.h>

/* HAL Status definitions */
typedef enum {
    HAL_OK       = 0x00U,
    HAL_ERROR    = 0x01U,
    HAL_BUSY     = 0x02U,
    HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/* HAL Lock definitions */
typedef enum {
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;

/* Placeholder structure for GPIO initialization settings */
typedef struct {
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
} GPIO_InitTypeDef;

/* SPI Handle Structure Definition */
typedef struct {
    struct {
        uint32_t Mode;
        uint32_t Direction;
        uint32_t DataSize;
        uint32_t CLKPolarity;
        uint32_t CLKPhase;
        uint32_t NSS;
        uint32_t BaudRatePrescaler;
        uint32_t FirstBit;
        uint32_t TIMode;
        uint32_t CRCCalculation;
        uint32_t CRCPolynomial;
    } Init;
    SPI_TypeDef *Instance;
    HAL_LockTypeDef Lock;
    HAL_StatusTypeDef State;
} SPI_HandleTypeDef;

/* Dummy SPI Register Definition */
typedef struct {
    uint32_t CR1;
    uint32_t CR2;
    uint32_t SR;
    uint32_t DR;
    uint32_t CRCPR;
    uint32_t RXCRCR;
    uint32_t TXCRCR;
    uint32_t I2SCFGR;
    uint32_t I2SPR;
} SPI_TypeDef;

/* Define a dummy SPI1 instance */
SPI_TypeDef SPI1_instance;
#define SPI1 (&SPI1_instance)

/* GPIO mode definitions */
#define GPIO_MODE_AF_PP    0x02U
#define GPIO_MODE_AF_OD    0x03U

/* GPIO speed definitions */
#define GPIO_SPEED_FREQ_LOW     0x00U
#define GPIO_SPEED_FREQ_MEDIUM  0x01U
#define GPIO_SPEED_FREQ_HIGH    0x02U

/* GPIO pull definitions */
#define GPIO_NOPULL             0x00U
#define GPIO_PULLUP             0x01U
#define GPIO_PULLDOWN           0x02U

/* SPI Initialization Definitions */
#define SPI_MODE_MASTER         0x01U
#define SPI_DIRECTION_2LINES    0x00U
#define SPI_DATASIZE_8BIT       0x07U
#define SPI_POLARITY_LOW        0x00U
#define SPI_PHASE_1EDGE         0x00U
#define SPI_NSS_SOFT            0x01U
#define SPI_BAUDRATEPRESCALER_16 0x18U
#define SPI_FIRSTBIT_MSB        0x00U
#define SPI_TIMODE_DISABLE      0x00U
#define SPI_CRCCALCULATION_DISABLE 0x00U

/* GPIO Alternate function definitions */
#define GPIO_AF5_SPI1           0x05U

/* Clock Enable Macros */
#define __SPI1_CLK_ENABLE()     /* Placeholder for enabling SPI1 clock */
#define __GPIOA_CLK_ENABLE()    /* Placeholder for enabling GPIOA clock */

typedef struct {
    uint32_t MODER;
    uint32_t OTYPER;
    uint32_t OSPEEDR;
    uint32_t PUPDR;
    uint32_t IDR;
    uint32_t ODR;
    uint32_t BSRR;
    uint32_t LCKR;
    uint32_t AFR[2];
} GPIO_TypeDef;

/* Function Prototypes */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
HAL_StatusTypeDef HAL_SPI_Init(SPI_HandleTypeDef *hspi);
HAL_StatusTypeDef HAL_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_SPI_GetState(SPI_HandleTypeDef *hspi);

#endif /* STM32XX_HAL_DEF_H */
