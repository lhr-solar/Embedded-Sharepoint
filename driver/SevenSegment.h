#ifndef SEVENSEGMENTHEADER_H  
#define SEVENSEGMENTHEADER_H  

#include "stm32xx_hal.h"

#define NUMBER_SEVEN_SEGMENTS 2
#define NUMBER_SEGMENTS 7

struct led_pins { 
    uint32_t Pin;
    GPIO_TypeDef *Port;
}; 

GPIO_InitTypeDef led_configA = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0 
}; 
GPIO_InitTypeDef led_configB = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0
}; 

GPIO_InitTypeDef led_configC = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0
}; 

typedef struct led_pins led_t;

led_t led[NUMBER_SEVEN_SEGMENTS][NUMBER_SEGMENTS];
//Initializes the LEDs for the Seven Segment LED.
void SevenSegment_Init();


//Displays a number on the Seven Segment LEDs. Display refers to display 0 or 1 and number is the number 0-9 that the LEDs will display.
void displayNum_SevenSegment(uint32_t display, uint32_t number);
    
    
// This function turns off the LEDs in one or both displays. Display 1 input = 0, Display 2 input = 1, Both = 2
void off_SevenSegment(uint32_t display);

#endif // SEVEN_SEGMENT_H