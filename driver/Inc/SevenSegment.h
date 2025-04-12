#ifndef SEVENSEGMENTHEADER_H  
#define SEVENSEGMENTHEADER_H  

#include "stm32xx_hal.h"

#define NUMBER_SEVEN_SEGMENTS 2
#define NUMBER_SEGMENTS 7
#define MAX_NUM 99

struct led_pins { 
   uint32_t Pin;
   GPIO_TypeDef *Port;
}; 

struct display {
   display_0 = 0,
   display_1 = 1
} 

typedef struct led_pins led_t;
typedef struct display display_t;


led_t led[NUMBER_SEVEN_SEGMENTS][NUMBER_SEGMENTS];
//Initializes the LEDs for the Seven Segment LED.
void SevenSegment_Init();


//Displays a number on the Seven Segment LEDs. Display refers to display 0 or 1 and number is the number 0-99 that the LEDs will display. If the number is larger than 10, display input does not matter.
void displayNum_SevenSegment(display_t display, uint8_t number);
    
    
// This function turns off the LEDs in one or both displays. Display 1 input = 0, Display 2 input = 1, Both = 2
void off_SevenSegment(uint8_t display);

#endif // SEVEN_SEGMENT_H
