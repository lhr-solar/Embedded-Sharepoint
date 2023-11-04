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

#pragma message "Pin PA0_AF set to " ENQUOTE(PA0_ALTERNATE_FUNCTION)
#pragma message "Pin PA1_AF set to " ENQUOTE(PA1_ALTERNATE_FUNCTION)
#pragma message "Pin PA2_AF set to " ENQUOTE(PA2_ALTERNATE_FUNCTION)
#pragma message "Pin PA3_AF set to " ENQUOTE(PA3_ALTERNATE_FUNCTION)
#pragma message "Pin PA4_AF set to " ENQUOTE(PA4_ALTERNATE_FUNCTION)
#pragma message "Pin PA5_AF set to " ENQUOTE(PA5_ALTERNATE_FUNCTION)
#pragma message "Pin PA6_AF set to " ENQUOTE(PA6_ALTERNATE_FUNCTION)
#pragma message "Pin PA7_AF set to " ENQUOTE(PA7_ALTERNATE_FUNCTION)
#pragma message "Pin PA8_AF set to " ENQUOTE(PA8_ALTERNATE_FUNCTION)
#pragma message "Pin PA9_AF set to " ENQUOTE(PA9_ALTERNATE_FUNCTION)
#pragma message "Pin PA10_AF set to " ENQUOTE(PA10_ALTERNATE_FUNCTION)
#pragma message "Pin PA11_AF set to " ENQUOTE(PA11_ALTERNATE_FUNCTION)
#pragma message "Pin PA12_AF set to " ENQUOTE(PA12_ALTERNATE_FUNCTION)
#pragma message "Pin PA13_AF set to " ENQUOTE(PA13_ALTERNATE_FUNCTION)
#pragma message "Pin PA14_AF set to " ENQUOTE(PA14_ALTERNATE_FUNCTION)
#pragma message "Pin PA15_AF set to " ENQUOTE(PA15_ALTERNATE_FUNCTION)
#pragma message "Pin PB0_AF set to " ENQUOTE(PB0_ALTERNATE_FUNCTION)
#pragma message "Pin PB1_AF set to " ENQUOTE(PB1_ALTERNATE_FUNCTION)
#pragma message "Pin PB2_AF set to " ENQUOTE(PB2_ALTERNATE_FUNCTION)
#pragma message "Pin PB3_AF set to " ENQUOTE(PB3_ALTERNATE_FUNCTION)
#pragma message "Pin PB4_AF set to " ENQUOTE(PB4_ALTERNATE_FUNCTION)
#pragma message "Pin PB5_AF set to " ENQUOTE(PB5_ALTERNATE_FUNCTION)
#pragma message "Pin PB6_AF set to " ENQUOTE(PB6_ALTERNATE_FUNCTION)
#pragma message "Pin PB7_AF set to " ENQUOTE(PB7_ALTERNATE_FUNCTION)
#pragma message "Pin PB8_AF set to " ENQUOTE(PB8_ALTERNATE_FUNCTION)
#pragma message "Pin PB9_AF set to " ENQUOTE(PB9_ALTERNATE_FUNCTION)
#pragma message "Pin PB10_AF set to " ENQUOTE(PB10_ALTERNATE_FUNCTION)
#pragma message "Pin PB12_AF set to " ENQUOTE(PB12_ALTERNATE_FUNCTION)
#pragma message "Pin PB13_AF set to " ENQUOTE(PB13_ALTERNATE_FUNCTION)
#pragma message "Pin PB14_AF set to " ENQUOTE(PB14_ALTERNATE_FUNCTION)
#pragma message "Pin PB15_AF set to " ENQUOTE(PB15_ALTERNATE_FUNCTION)
#pragma message "Pin PC0_AF set to " ENQUOTE(PC0_ALTERNATE_FUNCTION)
#pragma message "Pin PC1_AF set to " ENQUOTE(PC1_ALTERNATE_FUNCTION)
#pragma message "Pin PC2_AF set to " ENQUOTE(PC2_ALTERNATE_FUNCTION)
#pragma message "Pin PC3_AF set to " ENQUOTE(PC3_ALTERNATE_FUNCTION)
#pragma message "Pin PC4_AF set to " ENQUOTE(PC4_ALTERNATE_FUNCTION)
#pragma message "Pin PC5_AF set to " ENQUOTE(PC5_ALTERNATE_FUNCTION)
#pragma message "Pin PC6_AF set to " ENQUOTE(PC6_ALTERNATE_FUNCTION)
#pragma message "Pin PC7_AF set to " ENQUOTE(PC7_ALTERNATE_FUNCTION)
#pragma message "Pin PC8_AF set to " ENQUOTE(PC8_ALTERNATE_FUNCTION)
#pragma message "Pin PC9_AF set to " ENQUOTE(PC9_ALTERNATE_FUNCTION)
#pragma message "Pin PC10_AF set to " ENQUOTE(PC10_ALTERNATE_FUNCTION)
#pragma message "Pin PC11_AF set to " ENQUOTE(PC11_ALTERNATE_FUNCTION)
#pragma message "Pin PC12_AF set to " ENQUOTE(PC12_ALTERNATE_FUNCTION)
#pragma message "Pin PC13_AF set to " ENQUOTE(PC13_ALTERNATE_FUNCTION)
#pragma message "Pin PD2_AF set to " ENQUOTE(PD2_ALTERNATE_FUNCTION)

/**
 * @brief Pin <-> AF mapping lookup table
 * 
 */
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

