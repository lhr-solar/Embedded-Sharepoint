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

enum display {
   display_0 = 0,
   display_1 = 1,
   display_off = 2
}; 

typedef struct led_pins led_t;
typedef enum display display_t;


//Initializes the LEDs for the Seven Segment LED.
void SevenSegment_Init();


/*Displays a number on the Seven Segment LEDs. Display refers to display_0 or display_1 
 and number is the number 0-99 that the LEDs will display. 
 If the number is larger than 10, display input does not matter since it will use both displays. 
 Returns 1 for success and 0 for failure.*/
uint8_t displayNum_SevenSegment(display_t display, uint8_t number);
    
    
/* This function turns off the LEDs in one or both displays. 
Input is display_0, display_1, or display_off */
void off_SevenSegment(uint8_t display);

#endif // SEVEN_SEGMENT_H
