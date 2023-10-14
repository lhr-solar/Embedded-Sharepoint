#ifndef BSP_CONFIG_H
#define BSP_CONFIG_H

#include <stdint.h>

#define PREPROC_COMMA , // preprocessor black magic for default config stuff
#define STR(x) #x // see below
#define ENQUOTE(x) STR(x) // preprocessor black magic to print string equivalent of #defines

/**
  _____  _              ___          _____             _         _                        _ 
 |  __ \(_)            |__ \        |  __ \           (_)       | |                      | |
 | |__) |_  _ __  ______  ) |______ | |__) |___  _ __  _  _ __  | |__    ___  _ __  __ _ | |
 |  ___/| || '_ \|______|/ /|______||  ___// _ \| '__|| || '_ \ | '_ \  / _ \| '__|/ _` || |
 | |    | || | | |      / /_        | |   |  __/| |   | || |_) || | | ||  __/| |  | (_| || |
 |_|    |_||_| |_|     |____|       |_|    \___||_|   |_|| .__/ |_| |_| \___||_|   \__,_||_|
                                                         | |                                
                                                         |_|                                
                              __  __                                                        
                             |  \/  |                                                       
                             | \  / |  __ _  _ __                                           
                             | |\/| | / _` || '_ \                                          
                             | |  | || (_| || |_) |                                         
                             |_|  |_| \__,_|| .__/                                          
                                            | |                                             
                                            |_|                                             
            _    _______  ______  _____   _   _         _______  ______ 
     /\    | |  |__   __||  ____||  __ \ | \ | |    /\ |__   __||  ____|
    /  \   | |     | |   | |__   | |__) ||  \| |   /  \   | |   | |__   
   / /\ \  | |     | |   |  __|  |  _  / | . ` |  / /\ \  | |   |  __|  
  / ____ \ | |____ | |   | |____ | | \ \ | |\  | / ____ \ | |   | |____ 
 /_/    \_\|______||_|   |______||_|  \_\|_| \_|/_/    \_\|_|   |______|
                                                                        
                                                                        
  ______  _    _  _   _   _____  _______  _____  ____   _   _   _____   
 |  ____|| |  | || \ | | / ____||__   __||_   _|/ __ \ | \ | | / ____|  
 | |__   | |  | ||  \| || |        | |     | | | |  | ||  \| || (___    
 |  __|  | |  | || . ` || |        | |     | | | |  | || . ` | \___ \   
 | |     | |__| || |\  || |____    | |    _| |_| |__| || |\  | ____) |  
 |_|      \____/ |_| \_| \_____|   |_|   |_____|\____/ |_| \_||_____/   

 https://www.st.com/resource/en/datasheet/stm32f413cg.pdf page 67
 
* @details The following preprocessor defines are designed for you to choose the BSP
hardware selections and configuration at compile-time.
*/

// (0) is also GPIO

#ifndef PA0_ALTERNATE_FUNCTION
	#define PA0_ALTERNATE_FUNCTION (BSP_UART4)
#endif
#ifndef PA1_ALTERNATE_FUNCTION
	#define PA1_ALTERNATE_FUNCTION (BSP_UART4)
#endif
#ifndef PA2_ALTERNATE_FUNCTION
	#define PA2_ALTERNATE_FUNCTION (BSP_TIM2)
#endif
#ifndef PA3_ALTERNATE_FUNCTION
	#define PA3_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA4_ALTERNATE_FUNCTION
	#define PA4_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA5_ALTERNATE_FUNCTION
	#define PA5_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA6_ALTERNATE_FUNCTION
	#define PA6_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA7_ALTERNATE_FUNCTION
	#define PA7_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA8_ALTERNATE_FUNCTION
	#define PA8_ALTERNATE_FUNCTION (BSP_GPIO)
#endif
#ifndef PA9_ALTERNATE_FUNCTION
	#define PA9_ALTERNATE_FUNCTION (BSP_SPI2)
#endif
#ifndef PA10_ALTERNATE_FUNCTION
	#define PA10_ALTERNATE_FUNCTION (BSP_SPI2)
#endif
#ifndef PA11_ALTERNATE_FUNCTION
	#define PA11_ALTERNATE_FUNCTION (BSP_SPI2)
#endif
#ifndef PA12_ALTERNATE_FUNCTION
	#define PA12_ALTERNATE_FUNCTION (BSP_SPI2)
#endif
#ifndef PA13_ALTERNATE_FUNCTION
	#define PA13_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA14_ALTERNATE_FUNCTION
	#define PA14_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA15_ALTERNATE_FUNCTION
	#define PA15_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB0_ALTERNATE_FUNCTION
	#define PB0_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB1_ALTERNATE_FUNCTION
	#define PB1_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB2_ALTERNATE_FUNCTION
	#define PB2_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB3_ALTERNATE_FUNCTION
	#define PB3_ALTERNATE_FUNCTION (BSP_CAN3)
#endif
#ifndef PB4_ALTERNATE_FUNCTION
	#define PB4_ALTERNATE_FUNCTION (BSP_CAN3)
#endif
#ifndef PB5_ALTERNATE_FUNCTION
	#define PB5_ALTERNATE_FUNCTION (BSP_CAN2)
#endif
#ifndef PB6_ALTERNATE_FUNCTION
	#define PB6_ALTERNATE_FUNCTION (BSP_CAN2)
#endif
#ifndef PB7_ALTERNATE_FUNCTION
	#define PB7_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB8_ALTERNATE_FUNCTION
	#define PB8_ALTERNATE_FUNCTION (BSP_I2C1)
#endif
#ifndef PB9_ALTERNATE_FUNCTION
	#define PB9_ALTERNATE_FUNCTION (BSP_I2C1)
#endif
#ifndef PB10_ALTERNATE_FUNCTION
	#define PB10_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB12_ALTERNATE_FUNCTION
	#define PB12_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB13_ALTERNATE_FUNCTION
	#define PB13_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB14_ALTERNATE_FUNCTION
	#define PB14_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB15_ALTERNATE_FUNCTION
	#define PB15_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC0_ALTERNATE_FUNCTION
	#define PC0_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC1_ALTERNATE_FUNCTION
	#define PC1_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC2_ALTERNATE_FUNCTION
	#define PC2_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC3_ALTERNATE_FUNCTION
	#define PC3_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC4_ALTERNATE_FUNCTION
	#define PC4_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC5_ALTERNATE_FUNCTION
	#define PC5_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC6_ALTERNATE_FUNCTION
	#define PC6_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC7_ALTERNATE_FUNCTION
	#define PC7_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC8_ALTERNATE_FUNCTION
	#define PC8_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC9_ALTERNATE_FUNCTION
	#define PC9_ALTERNATE_FUNCTION (BSP_SPI3)
#endif
#ifndef PC10_ALTERNATE_FUNCTION
	#define PC10_ALTERNATE_FUNCTION (BSP_SPI3)
#endif
#ifndef PC11_ALTERNATE_FUNCTION
	#define PC11_ALTERNATE_FUNCTION (BSP_SPI3)
#endif
#ifndef PC12_ALTERNATE_FUNCTION
	#define PC12_ALTERNATE_FUNCTION (BSP_SPI3)
#endif
#ifndef PC13_ALTERNATE_FUNCTION
	#define PC13_ALTERNATE_FUNCTION (0)
#endif
#ifndef PD2_ALTERNATE_FUNCTION
	#define PD2_ALTERNATE_FUNCTION (0)
#endif

// a remapping enum for programmatic use
typedef enum {
	BSP_GPIO, 
	BSP_UART4, BSP_UART5,
	BSP_TIM1, BSP_TIM2, BSP_TIM3, BSP_TIM9,
	BSP_I2C1, BSP_I2C3, 
	BSP_SPI2, BSP_SPI3,
	BSP_CAN2, BSP_CAN3, 
	NUM_PERIPHS
} AF_ALIAS;

#endif
