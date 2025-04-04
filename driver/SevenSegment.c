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
    .Mode = GPIO_MODE_OUTPUT_PP, // figure thing out
    .Pull = GPIO_NOPULL,
    .Pin = 0
}; 

typedef struct led_pins led_t;


//led_config.Mode = GPIO_MODE_OUTPUT_PP;
//led_config.Pull = GPIO_NOPULL;
//led_config.Pin = GPIO_PIN_10;
    
//led_pins.Pin = Pin;
//led_config.Port = GPIO_TypeDef *Port; 

led_t led[NUMBER_SEVEN_SEGMENTS][NUMBER_SEGMENTS] = {{{GPIO_PIN_10, GPIOB}, {GPIO_PIN_1, GPIOB}, {GPIO_PIN_14, GPIOB}, {GPIO_PIN_2, GPIOB}, {GPIO_PIN_0, GPIOB}, {GPIO_PIN_5, GPIOC}, {GPIO_PIN_4, GPIOC}},
   {{GPIO_PIN_6, GPIOC}, {GPIO_PIN_7, GPIOC}, {GPIO_PIN_15, GPIOB}, {GPIO_PIN_15, GPIOA}, {GPIO_PIN_10, GPIOC}, {GPIO_PIN_11, GPIOC}, {GPIO_PIN_12, GPIOC}}};

//Initializes the LEDs for the Seven Segment LED.
void SevenSegment_Init(){ //IOMUX -> SECCFG.PINCM[PB10INDEX]=(uint32_t) 0x00000081;
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable clock for GPIOB
    __HAL_RCC_GPIOC_CLK_ENABLE(); // enable clock for GPIOC

    for (uint8_t i = 0; i < 2; i++) {
        for (uint8_t j = 0; j < 6; j++){
            if (led[i][j].Port == GPIOA){
                led_configA.Pin = (led[i][j].Pin | led_configA.Pin);   
            }
            else if (led[i][j].Port == GPIOB){
                led_configB.Pin = (led[i][j].Pin | led_configB.Pin);   
            }
            else if (led[i][j].Port == GPIOC){
                led_configC.Pin = (led[i][j].Pin | led_configC.Pin);   
            }
        }
    } 
    HAL_GPIO_Init(GPIOA, &led_configA); // initialize GPIOA with led_configA // Does this initalize all pins at the same time?
    HAL_GPIO_Init(GPIOB, &led_configB); // initialize GPIOB with led_configB
    HAL_GPIO_Init(GPIOC, &led_configC); // initialize GPIOC with led_configC

    //DISPLAY1 LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
    //DISPLAY2 LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
}


//Displays a number on the Seven Segment LEDs. Display refers to display 0 or 1 and number is the number 0-9 that the LEDs will display.
void displayNum_SevenSegment(uint32_t display, uint32_t number){ //display is 0 or 1
    
    switch (number){
        case 0:
            for (uint8_t i = 0; i <= 5; i++){
               HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
            }

            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11
            break;
        case 1:
            for (uint8_t i = 1; i <= 2; i++){
                HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
            }

            //On: LED2 PB1, LED3 PB14
            //On: LED9 PC7, LED10 PB15
            break;
        case 2:
            for (uint8_t i = 0; i <= 6; i++){
                if ((i != 2) && (i != 5)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On:LED1 PB10, LED2 PB1, LED4 PB2, LED5 PB0, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED11 PA15, LED12 PC10, LED14 PC12
            break;
        case 3:
            for (uint8_t i = 0; i <= 6; i++){ 
                if ((i <= 3) || (i == 6)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }
    
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED14 PC12
            break;
        case 4:
            for (uint8_t i = 1; i <= 6; i++){ 
                if ((i <= 2) || (i >=5)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }
           
            //On: LED2 PB1, LED3 PB14, LED6 PC5, LED7 PC4
            //On: LED9 PC7, LED10 PB15, LED13 PC11, LED14 PC12
            break;
        case 5:
            for (uint8_t i = 0; i <= 6; i++){ 
                if ((i != 1) && (i != 4)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }
            
            //On:LED1 PB10, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
        case 6:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i != 1) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }
    
            //On:LED1 PB10, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
            break;
        case 7:
            for (uint8_t i = 0; i <= 2; i++){ 
                HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
            }
            
            //On:LED1 PB10, LED2 PB1, LED3 PB14
            //On:LED8 PC6, LED9 PC7, LED10 PB15
            break;
        case 8:
            for (uint8_t i = 0; i <= 6; i++){ 
                HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
            }
            
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
            break;
        case 9:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i != 4){
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }
            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
    }
}

// This function turns off the LEDs in one or both displays. Display 1 input = 0, Display 2 input = 1, Both = 2
void off_SevenSegment(uint32_t display){
    if (display == 0 || display == 2){
        //HAL
    }
    if (display == 1 || display == 2){

    }

}
