#include "bsp_config.h"

/*
   _____  _       ____   ____            _      
  / ____|| |     / __ \ |  _ \    /\    | |     
 | |  __ | |    | |  | || |_) |  /  \   | |     
 | | |_ || |    | |  | ||  _ <  / /\ \  | |     
 | |__| || |____| |__| || |_) |/ ____ \ | |____ 
  \_____||______|\____/ |____//_/    \_\|______|
                                                
                                                
           _____  _____  _   _                  
          |  __ \|_   _|| \ | |                 
          | |__) | | |  |  \| |                 
          |  ___/  | |  | . ` |                 
          | |     _| |_ | |\  |                 
          |_|    |_____||_| \_|                 
                                                
                                                
     __      __       _        _____            
     \ \    / //\    | |      / ____|           
      \ \  / //  \   | |     | (___             
       \ \/ // /\ \  | |      \___ \            
        \  // ____ \ | |____  ____) |           
         \//_/    \_\|______||_____/            
*/
#include "BSP.h"
const uint8_t BSP_GPIO_MAPPING[NUM_BSP_PINS] = {
    [BSP_GPIO_PA0] = (PA0_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA1] = (PA1_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA2] = (PA2_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA3] = (PA3_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA4] = (PA4_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA5] = (PA5_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA6] = (PA6_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA7] = (PA7_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA8] = (PA8_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA9] = (PA9_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA10] = (PA10_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA11] = (PA11_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA12] = (PA12_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA13] = (PA13_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA14] = (PA14_ALTERNATE_FUNCTION),
    [BSP_GPIO_PA15] = (PA15_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB0] = (PB0_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB1] = (PB1_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB2] = (PB2_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB3] = (PB3_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB4] = (PB4_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB5] = (PB5_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB6] = (PB6_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB7] = (PB7_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB8] = (PB8_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB9] = (PB9_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB10] = (PB10_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB12] = (PB12_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB13] = (PB13_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB14] = (PB14_ALTERNATE_FUNCTION),
    [BSP_GPIO_PB15] = (PB15_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC0] = (PC0_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC1] = (PC1_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC2] = (PC2_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC3] = (PC3_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC4] = (PC4_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC5] = (PC5_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC6] = (PC6_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC7] = (PC7_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC8] = (PC8_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC9] = (PC9_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC10] = (PC10_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC11] = (PC11_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC12] = (PC12_ALTERNATE_FUNCTION),
    [BSP_GPIO_PC13] = (PC13_ALTERNATE_FUNCTION),
    [BSP_GPIO_PD2] = (PD2_ALTERNATE_FUNCTION),
};
