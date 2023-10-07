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

The following preprocessor defines are designed for you to choose the BSP
hardware selections and configuration at compile-time. Below this block of
defines are global variables used in parameters for the actual HAL function calls
 */

/*
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
*/



#ifndef PA0_ALTERNATE_FUNCTION
	#define PA0_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA1_ALTERNATE_FUNCTION
	#define PA1_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA2_ALTERNATE_FUNCTION
	#define PA2_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA3_ALTERNATE_FUNCTION
	#define PA3_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA4_ALTERNATE_FUNCTION
	#define PA4_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA5_ALTERNATE_FUNCTION
	#define PA5_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA6_ALTERNATE_FUNCTION
	#define PA6_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA7_ALTERNATE_FUNCTION
	#define PA7_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA8_ALTERNATE_FUNCTION
	#define PA8_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA9_ALTERNATE_FUNCTION
	#define PA9_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA10_ALTERNATE_FUNCTION
	#define PA10_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA11_ALTERNATE_FUNCTION
	#define PA11_ALTERNATE_FUNCTION (0)
#endif
#ifndef PA12_ALTERNATE_FUNCTION
	#define PA12_ALTERNATE_FUNCTION (0)
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
	#define PB3_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB4_ALTERNATE_FUNCTION
	#define PB4_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB5_ALTERNATE_FUNCTION
	#define PB5_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB6_ALTERNATE_FUNCTION
	#define PB6_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB7_ALTERNATE_FUNCTION
	#define PB7_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB8_ALTERNATE_FUNCTION
	#define PB8_ALTERNATE_FUNCTION (0)
#endif
#ifndef PB9_ALTERNATE_FUNCTION
	#define PB9_ALTERNATE_FUNCTION (0)
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
	#define PC9_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC10_ALTERNATE_FUNCTION
	#define PC10_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC11_ALTERNATE_FUNCTION
	#define PC11_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC12_ALTERNATE_FUNCTION
	#define PC12_ALTERNATE_FUNCTION (0)
#endif
#ifndef PC13_ALTERNATE_FUNCTION
	#define PC13_ALTERNATE_FUNCTION (0)
#endif
#ifndef PD2_ALTERNATE_FUNCTION
	#define PD2_ALTERNATE_FUNCTION (0)
#endif

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

/*
  _____   ______  ______       _    _  _    _______  _____ 
 |  __ \ |  ____||  ____|/\   | |  | || |  |__   __|/ ____|
 | |  | || |__   | |__  /  \  | |  | || |     | |  | (___  
 | |  | ||  __|  |  __|/ /\ \ | |  | || |     | |   \___ \ 
 | |__| || |____ | |  / ____ \| |__| || |____ | |   ____) |
 |_____/ |______||_| /_/    \_\\____/ |______||_|  |_____/ 
*/
// (wip)
#define I2C_DEFAULT 

#endif
