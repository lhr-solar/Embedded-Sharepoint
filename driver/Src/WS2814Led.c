#include "WS2814Led.h"

// Driver for controlling an addressable LED strip

// Pins for PWM:
// For STM32L431CBT6 Lighting PSOM
// OPAMP1_VINP (7) GPIO_BPS_Strobe: PA0
// ADC1_IN7 for GPIO_Addre: PA2
// SPI1_MISO for GPIO_Normal: PA11

#define NUM_LEDS 100 // Not the correct number
#define BYTES_PER_LED 4

ws2814_color_t ledCommandBuf[NUM_LEDS] = {0};

uint8_t dmaTransferBuffer[NUM_LEDS * BYTES_PER_LED];





// Init function
// Initialize chosen pins for addressable LED (maybe just 1 for now- not strobe or normal)
void WS2814_Init() {
    // Config stuff
}

/**
 * Fill a next-color buffer with the next color/light command. Note that there will be multiple light outputs
 */
void WS2814_SetLights(color) {

}


/**
 * dma callback
 * Have a separate transfer buffer of size 2
 * When the transfer is half completed, replace the other half of the buffer with new data
 */
// Idk about dealing with conditions of trying to set and access buffer at the same time?



/**
 * Override this if needed - timer callback for reloading timer
 */
//weak uint8_t HAL_Callback () {

}

/**
 * When the timer expires, switch the reload value to the next one (and set based on 1 or zero)
 * 
 */
Lights_Driver_Callback () {
    // Look at the buffer (defined outside of here) and set reload value to the next one
}


// Set color to a hardcoded value
// Tell some number of leds to turn a certain color using timer and dma




