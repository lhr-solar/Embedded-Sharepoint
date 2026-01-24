#include "WS2812B.h"
#include "stm32xx_hal.h"

#define STACK_SIZE 200

StaticTask_t xTaskBuffer;
StackType_t xStack[ STACK_SIZE ];

static status_t curr_stat;

uint8_t* conv2rgbarray(char* hstring);

void EightReds(void *pvParameters) {
    /* 8 red leds */
    char* red_hstring="FF0000";
    uint8_t leds[24];
    uint8_t* rgb_arr = conv2rgbarray(red_hstring);
    for (int i=0;i<24;i++) leds[i]=rgb_arr[i]; // copy over

    WS2812B_Send(leds, 1);
}

uint8_t* conv2rgbarray(char* hstring) {
    static uint8_t arr[24];
    uint8_t idx=0;
    for (int i=0;i<3;i++) {
        for (int j=0;j<8;j++) {
            uint8_t decimal=hstring[i*24+j]-'0';
            if (decimal>=8) { arr[idx++]=1;decimal-=8; }
            else { arr[idx++]=0; }
            if (decimal>=4) { arr[idx++]=1;decimal-=4; }
            else { arr[idx++]=0; }
            if (decimal>=2) { arr[idx++]=1;decimal-=2; }
            else { arr[idx++]=0; }
            if (decimal>=1) { arr[idx++]=1;decimal-=1; }
            else { arr[idx++]=0; }
        }
    }
    return arr;
}

int main(void) {
    #define LSOM_PORT GPIOB
    #define LSOM_PIN  GPIO_PIN_6

    curr_stat = WS2812B_Init(LSOM_PORT, LSOM_PIN);

    xTaskCreateStatic(EightReds,
                    "Display 8 Red LEDs",
                    configMINIMAL_STACK_SIZE,
                    (void*) 1,
                    tskIDLE_PRIORITY+4,
                    xStack,
                    &xTaskBuffer);

    vTaskStartScheduler();
    
    // should never reach

    return 0;
}