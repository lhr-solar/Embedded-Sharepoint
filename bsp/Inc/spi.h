#include "stdint.h"

void SPI1_Init(void);
void SPI1_TransmitReceive_IT(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);