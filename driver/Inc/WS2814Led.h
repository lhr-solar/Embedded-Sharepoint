#ifndef WS2814_H  
#define WS2814_H  

#include "stm32xx_hal.h"

// Each LED takes 4 bytes with MSbit first
typedef struct _ws2814_color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t w;
} ws2814_color_t;

// Init function
void WS2814_Init();


// uint8_t SetLights(color struct, number)
void WS2814_SetLights(color);

// De-init function
void WS2814_Deinit(); // probably should return an error code

#endif