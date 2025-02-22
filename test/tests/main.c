<<<<<<< HEAD:Test/Tests/main.c
#include "stm32xx_hal.h"

int main(void) {

    HAL_MspInit();
    SystemClock_Config();

    int count = 0;
    
    count = count + 1;
    
    return 0;
}
=======
#include "stm32xx_hal.h"

int main(void) {

    HAL_MspInit();

    int count = 0;
    
    count = count + 1;
    
    return 0;
}
>>>>>>> 8d064395c8b6bd25adc30862eef81b43163b4ba7:test/tests/main.c
