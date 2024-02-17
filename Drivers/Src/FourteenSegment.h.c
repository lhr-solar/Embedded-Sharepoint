#include "FourteenSegment.h"
#include "BSP.h"

/*
 7 Segment Display pins for the for first Display
*/
#define disp1SegA BSP_GPIO_PB10
#define disp1SegB BSP_GPIO_PB1
#define disp1SegC BSP_GPIO_PB14
#define disp1SegD BSP_GPIO_PB2
#define disp1SegE BSP_GPIO_PB0
#define disp1SegF BSP_GPIO_PC5
#define disp1SegG BSP_GPIO_PC6


/*
 7 Segment Display pins for the for second Display
*/
#define disp2SegA BSP_GPIO_PC7
#define disp2SegB BSP_GPIO_PB15
#define disp2SegC BSP_GPIO_PA15
#define disp2SegD BSP_GPIO_PC10
#define disp2SegE BSP_GPIO_PC11
#define disp2SegF BSP_GPIO_PC12
#define disp2SegG BSP_GPIO_PC5 
//FIX PINS :sob:


// To do: make not int
const int sevenSegmentLookup[16] = {
    0x3F, /* 1 */
    0x5B, /* 2 */
    0x4F, /* 3 */
    0x66, /* 4 */
    0x6D, /* 5 */
    0x7D, /* 6 */
    0x07, /* 7 */
    0x7F, /* 8 */
    0x6F, /* 9 */
    0x77, /* A */
    0x7C, /* B */
    0x39, /* C */
    0x5E, /* D */
    0x79, /* E */
    0x71  /* F */
};

void Fourteen_Segment_Init(void){
    //HAL_GPIO_Init();
    // assume pins already set as output
    // assume timers are set
}

void Display_Top(int digit){

    int segments = sevenSegmentLookup[digit];

    HAL_GPIO_WritePin(disp1SegA, (segments & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegB, (segments & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegC, (segments & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegD, (segments & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegE, (segments & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegF, (segments & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(disp1SegG, (segments & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);

}

 

