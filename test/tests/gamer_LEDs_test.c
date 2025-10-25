/**
 * This test currently does not seem to work
 */
#include "Gamer_LEDs.h"
#include "stm32xx_hal.h"

int main(){
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    Gamer_LEDs_Init();
    
    while (1)
    {
        test_set_leds_high();
        HAL_Delay(10);
       
    }
   

    return 0;
}


