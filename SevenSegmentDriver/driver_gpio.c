#include "stm32xx_hal.h"

GPIO_InitTypeDef led_config3;

struct led_pins { 
    uint32_t Pin;
    GPIO_TypeDef *Port;
}; 

GPIO_InitTypeDef led_config = {
    .Mode = GPIO_MODE_OUTPUT_PP,
    .Pull = GPIO_NOPULL,
    .Pin = GPIO_PIN_5
}; 


typedef struct led_pins led_t;


//led_config.Mode = GPIO_MODE_OUTPUT_PP;
//led_config.Pull = GPIO_NOPULL;
//led_config.Pin = GPIO_PIN_10;
    
//led_pins.Pin = Pin;
//led_config.Port = GPIO_TypeDef *Port; 

led_t led[2][7] = {{{GPIO_PIN_10, GPIOB}, {GPIO_PIN_1, GPIOB}, {GPIO_PIN_14, GPIOB}, {GPIO_PIN_2, GPIOB}, {GPIO_PIN_0, GPIOB}, {GPIO_PIN_5, GPIOC}, {GPIO_PIN_4, GPIOC}},
   {{GPIO_PIN_6, GPIOC}, {GPIO_PIN_7, GPIOC}, {GPIO_PIN_15, GPIOB}, {GPIO_PIN_15, GPIOA}, {GPIO_PIN_10, GPIOC}, {GPIO_PIN_11, GPIOC}, {GPIO_PIN_12, GPIOC}}};


void LED_Init(){ //IOMUX -> SECCFG.PINCM[PB10INDEX]=(uint32_t) 0x00000081;

    

    for (uint8_t i = 0; i < 14; i++) {
        if (i <= 6){
            led_config.Pin = led[0][i].Pin;  
            HAL_GPIO_Init(led[0][i].Port, &led_config); 
        }
        else {
            led_config.Pin = led[1][i].Pin;  
            HAL_GPIO_Init(led[1][i].Port, &led_config);
        }
    } 

    //DISPLAY1 LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED5 PB0, LED6 PC5, LED7 PC4
    //DISPLAY2 LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED12 PC10, LED13 PC11, LED14 PC12
}

void num(uint32_t display, uint32_t number){ //display is 0 or 1
    
    switch (number){
        case 0:
            for (uint8_t i = 0; i <= 4; i++){
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
            for (uint8_t i = 0; i < 3; i++){
                if ((i != 2) && (i != 5)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On:LED1 PB10, LED2 PB1, LED4 PB2, LED5 PB0, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED11 PA15, LED12 PC10, LED14 PC12
            break;
        case 3:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i <= 3 || i == 6) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED14 PC12
            break;
        case 4:
            for (uint8_t i = 1; i <= 6; i++){ 
                if ((i <= 2) || (i >=5 && i <= 6)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On: LED2 PB1, LED3 PB14, LED6 PC5, LED7 PC4
            //On: LED9 PC7, LED10 PB15, LED13 PC11, LED14 PC12
            break;
        case 5:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i == 1 || (i >=2 && i <= 3) || (i >=5 && i <= 6)) {
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On:LED1 PB10, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
        case 6:
            for (uint8_t i = 0; i <= 6; i++){ 
                if (i == 1 || (i >=2 && i <= 7)) {
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
                if ( i != 5){
                    HAL_GPIO_TogglePin(led[display][i].Port, led[display][i].Pin);
                }
            }

            //On:LED1 PB10, LED2 PB1, LED3 PB14, LED4 PB2, LED6 PC5, LED7 PC4
            //On:LED8 PC6, LED9 PC7, LED10 PB15, LED11 PA15, LED13 PC11, LED14 PC12
            break;
    }
}
//function for turning them off

int main2(){
    HAL_Init();  //initializes HAL
    LED_Init();
    
    while(1){
   
    }

    return 0;
} 

int main(){
    HAL_Init();

    struct led_pins2 { 
        uint32_t Pin;
        GPIO_TypeDef *Port;
    }; 

    typedef struct led_pins led2_t;

    led2_t arr[1] = {{GPIO_PIN_10, GPIOB}};

    GPIO_InitTypeDef led_config4 = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_10
    };
    
    __HAL_RCC_GPIOB_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(GPIOB, &led_config4); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(GPIOB, arr[0].Pin);
        HAL_Delay(1000);
    }

    return 0;
   
}

int mainblinky(){
    HAL_Init();

    GPIO_InitTypeDef led_config3 = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
    __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
    HAL_GPIO_Init(GPIOA, &led_config3); // initialize GPIOA with led_config

    while(1){
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        HAL_Delay(1000);
    }

    return 0;
}
 