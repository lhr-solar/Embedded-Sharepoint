#include "stm32xx_hal.h"
#include "SevenSegment.h"

static GPIO_InitTypeDef led_configA = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0 
}; 
static GPIO_InitTypeDef led_configB = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0
}; 

static GPIO_InitTypeDef led_configC = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = 0
}; 

const led_t seven_segment_led[NUMBER_SEVEN_SEGMENTS][NUMBER_SEGMENTS] = {{{GPIO_PIN_10, GPIOB}, {GPIO_PIN_1, GPIOB}, {GPIO_PIN_14, GPIOB}, {GPIO_PIN_2, GPIOB}, {GPIO_PIN_0, GPIOB}, {GPIO_PIN_5, GPIOC}, {GPIO_PIN_4, GPIOC}},
   {{GPIO_PIN_6, GPIOC}, {GPIO_PIN_7, GPIOC}, {GPIO_PIN_15, GPIOB}, {GPIO_PIN_15, GPIOA}, {GPIO_PIN_10, GPIOC}, {GPIO_PIN_11, GPIOC}, {GPIO_PIN_12, GPIOC}}};

//Initializes the LEDs for the Seven Segment LED.
void SevenSegment_Init(){ 
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable clock for GPIOB
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable clock for GPIOC

    for (uint8_t i = 0; i < NUMBER_SEVEN_SEGMENTS; i++) {
        for (uint8_t j = 0; j < NUMBER_SEGMENTS; j++){
            switch ((uint32_t)seven_segment_led[i][j].Port){
                case (uint32_t)GPIOA:
                    led_configA.Pin = (seven_segment_led[i][j].Pin | led_configA.Pin);  
                    break; 
                case (uint32_t)GPIOB:
                    led_configB.Pin = (seven_segment_led[i][j].Pin | led_configB.Pin);   
                    break;
                case (uint32_t)GPIOC:
                    led_configC.Pin = (seven_segment_led[i][j].Pin | led_configC.Pin);   
                    break;
            }
        } 
    }
    HAL_GPIO_Init(GPIOA, &led_configA); // initialize GPIOA with led_configA 
    HAL_GPIO_Init(GPIOB, &led_configB); // initialize GPIOB with led_configB
    HAL_GPIO_Init(GPIOC, &led_configC); // initialize GPIOC with led_configC

    //DISPLAY1 LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
    //DISPLAY2 LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
}


/*Displays a number on the Seven Segment LEDs. Display refers to display_0 or display_1 
 and number is the number 0-99 that the LEDs will display. 
 If the number is larger than 10, display input does not matter since it will use both displays. 
 Returns 1 for success and 0 for failure.*/

 uint8_t displayNum_SevenSegment(display_t display, uint8_t number){ //display is 0 or 1
    uint8_t digit = 0;
    if (number > MAX_NUM){
        return 0;
    }
   for (uint8_t i = 0; i < NUMBER_SEVEN_SEGMENTS; i++){
        if ((number/10) == 0){
            if (display > display_1){
                return 0;
            }
            digit = number;
        }
        else if ((number/10) != 0 && i == 0) {
            digit = number / 10;
            display = display_1;
        }
        else if ((number/10) != 0 && i == 1){
            digit = number % 10;
            display = display_0;
        }
    switch (digit){
         case 0:
            HAL_GPIO_WritePin(seven_segment_led[display][6].Port, seven_segment_led[display][6].Pin, 1);
            for (uint8_t i = 0; i < 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
            }
           
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11
            break;
         case 1:
             for (uint8_t i = 0; i <= 6; i++){
                if (i !=4 && i !=5){
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
             }
             //On: LED2 PB1, LED3 PB14
             //On: LED9 PC7, LED10 PB15
             break;
        case 2:
            for (uint8_t i = 0; i <= 6; i++){
                if ((i == 2) || (i == 5)) {
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }

            //On:LED1 PB10, LED2 PB1, LED4 PB2, LED5 PB0, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED11 PA15, LED12 PC10, LED14 PC12
            break;
        case 3:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i == 2 || i == 1) {
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
    
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED14 PC12
            break;
        case 4:
            for (uint8_t i = 0; i <= 6; i++){ 
                if ((i <= 1) || (i == 3)) {
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
           
            //On: LED2 PB1, LED3 PB14, LED6 PC5, LED7 PC4
            //On: LED9 PC7, LED10 PB15, LED13 PC11, LED14 PC12
            break;
        case 5:
            for (uint8_t i = 0; i <= 6; i++){ 
                if ((i == 1) || (i == 4)) {
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
            
            //On:LED1 PB10, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
        case 6:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i == 4) {
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
    
            //On:LED1 PB10, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
            break;
        case 7:
            for (uint8_t i = 0; i <= 6; i++){ 
                if ((i <= 2) || i == 6){
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
            
            //On:LED1 PB10, LED2 PB1, LED3 PB14
            //On:LED8 PC6, LED9 PC7, LED10 PB15
            break;
        case 8:
            for (uint8_t i = 0; i <= 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
            } 
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
            break;
        case 9:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i == 1){
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
                }
                else{
                    HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 0);
                }
            }
            
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
    }
    }
    return 1;
}

//Displays a 2 number input and returns 0 for failure and 1 for success
uint8_t displayTwoNum_SevenSegment(uint8_t number){
    return displayNum_SevenSegment(0, number);
 }

/* This function turns off the LEDs in one or both displays. 
Input is display_0, display_1, or display_off */
void off_SevenSegment(uint8_t display){
    switch (display){
        case display_0:
            for (uint8_t i = 0; i <= 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
            }
            break;
        case display_1:
            for (uint8_t i = 0; i <= 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[display][i].Port, seven_segment_led[display][i].Pin, 1);
            }   
            break; 
        case display_off:
            for (uint8_t i = 0; i <= 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[0][i].Port, seven_segment_led[0][i].Pin, 1);
            }  
            for (uint8_t i = 0; i <= 6; i++){
                HAL_GPIO_WritePin(seven_segment_led[1][i].Port, seven_segment_led[1][i].Pin, 1);
            } 
            break;
    }
}