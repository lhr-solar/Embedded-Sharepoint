#include "stm32xx_hal.h"
#include "SevenSegment.h"

int main1(){
//count 0-10
    HAL_Init();
    SevenSegment_Init(); 
    while(1){

    for (uint8_t i = 0; i <= 10; i++){
        if (i == 10){
            displayTwoNum_SevenSegment(i);
        }
        else{
            displayNum_SevenSegment(0, i);
            displayNum_SevenSegment(1, i);
        }

        HAL_Delay(500);
    }
        
    }
    return 0;
}

int main(){
//count 0-99

    HAL_Init();
    SevenSegment_Init(); 
    while(1){

        for (uint8_t i = 0; i <= 99; i++){
            if (i < 10){
                displayNum_SevenSegment(0, i);
                displayNum_SevenSegment(1, i);           
            }
            else{
                displayTwoNum_SevenSegment(i);
            }
    
            HAL_Delay(200);
        }
        
    }
    return 0;
}