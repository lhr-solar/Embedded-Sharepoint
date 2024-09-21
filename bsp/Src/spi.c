#include "stm32l4xx_hal.h"
#include "spi.h"

// Define the SPI handle as a global variable
SPI_HandleTypeDef hspi1;

// Buffer for SPI transmission and reception
uint8_t TxBuffer[32];
uint8_t RxBuffer[32];

// SPI initialization function
void SPI1_Init(void)
{
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
    hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
    hspi1.Init.NSS = SPI_NSS_SOFT;
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    
    if (HAL_SPI_Init(&hspi1) != HAL_OK)
    {
        // Error handling
        Error_Handler();
    }
}

// Function to start an interrupt-based SPI transfer
void SPI1_TransmitReceive_IT(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
    if (HAL_SPI_TransmitReceive_IT(&hspi1, pTxData, pRxData, Size) != HAL_OK)
    {
        // Error handling
        Error_Handler();
    }
}

// SPI transfer complete callback
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        // Handle transfer complete
        // For example, set a flag or trigger another action
    }
}

// SPI error callback
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI1)
    {
        // Handle error
        // For example, reset the SPI peripheral or attempt to reinitialize
    }
}