// This is all autogenerated code (except for the 2 functions at the end)
    // https://github.com/STMicroelectronics/STM32CubeL4/blob/master/Projects/NUCLEO-L432KC/Examples/ADC/ADC_DMA_Transfer/Src/main.c
    // First is the main.h
    // Second is the main.c 

/* High level overview 
    * ADC is configured to continuously convert ADC_CHANNEL_9
    * Resolution is 12 bits
    * Data is continuously being transfered between Data Register and aADCxConvertedData buffer (by DMA)
    * Input: GPIO_PIN_4 on GPIOA
    * LED3 (on board) turns ON when error occurs

    * Main.h is easy to change so that you can run it on different pins 
    * Using SysTick - be Careful with timer
    * Have to have certain stuff in certain directories to build properly
    * Runs on our device
    * Been tested (by STM?) w/ NUCLEO-L432KC board

    * How to use?
        - Rebuild all files and flash into memory
        - Run the example
        - watch aADCxConvertedData varaible in debugger live watch

    * From Main.c, basically warnings so you dont mess up 
        /* STM32L4xx HAL library initialization:
            - Configure the Flash prefetch
            - Systick timer is configured by default as source of time base, but user 
                can eventually implement his proper time base source (a general purpose 
                timer for example or other time source), keeping in mind that Time base 
                duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
                handled in milliseconds basis.
            - Set NVIC Group Priority to 4
            - Low Level Initialization
            */ 
    */


// Main.h stuff:
/**
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l4xx_nucleo_32.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor ADCx instance used and associated
   resources */
   
/* Definition for ADCx clock resources */
#define ADCx                            ADC1                        // dont really know whats going on here 
#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC_CLK_ENABLE()
#define ADCx_CHANNEL_GPIO_CLK_ENABLE()  __HAL_RCC_GPIOA_CLK_ENABLE()

#define DMAx_CHANNELx_CLK_ENABLE()      __HAL_RCC_DMA1_CLK_ENABLE()

#define ADCx_FORCE_RESET()              __HAL_RCC_ADC_FORCE_RESET()
#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC_RELEASE_RESET()

/* Definition for ADCx Channel Pin */
#define ADCx_CHANNEL_PIN_CLK_ENABLE()   __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADCx_CHANNEL_PIN                GPIO_PIN_4                    // What pin we're using 
#define ADCx_CHANNEL_GPIO_PORT          GPIOA                         // What port its on 

/* Definition for ADCx's Channel */
#define ADCx_CHANNEL                    ADC_CHANNEL_9                 // What channel it is 

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */



// Main.c stuff (w/ some notes):
/**
  ******************************************************************************
  * @file    ADC/ADC_DMA_Transfer/Src/main.c
  * @author  MCD Application Team
  * @brief   This example describes how to use the DMA to convert continuously data.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32L4xx_HAL_Examples
  * @{
  */

/** @addtogroup ADC_DMA_Transfer
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Definitions of data related to this example */
  /* Definition of ADCx conversions data table size */
  #define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t)  32)   /* Size of array aADCxConvertedData[] */
  // This is the SIZE of the BUFFER that the DMA is filling up after it is "read" by the ADC

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* ADC handle declaration */
ADC_HandleTypeDef             AdcHandle;
// this is some struct that will be used the Handle ADC

/* ADC channel configuration structure declaration */
ADC_ChannelConfTypeDef        sConfig;
// they're making some struct that will be used for channel configureation 
    // This is includes the pins 

/* Variable containing ADC conversions data */
static uint16_t   volatile aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE];
    // included volatile - may not actully need it 
        // dont want complier to free style 
    // This is the varaible (buffer) that the DMA is filling up after it is "read" by the ADC
    // DOES this need to be "varaible" so it can continuously update 

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
 /* This sample code shows how to convert an analog input and read the converted
    data using DMA transfer.
    To proceed, 4 steps are required: */
   
  /* STM32L4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();
  // initlizes HAL 

  /* Configure the system clock to 80 MHz */
  SystemClock_Config();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* ### - 1 - Initialize ADC peripheral #################################### */
  AdcHandle.Instance          = ADCx;             
    // AdcHandler is a struct, and we are setting the instance member to be ADCx
    // ADCx is defined in the main.h and its kinda inteneded that the user modifies main.h
    // ADCx equals ADC1 in the main.h file
      // There are a few other things you can change in the main.h file 
  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
  {
    /* ADC de-initialization Error */
    Error_Handler();
  }

  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV2;      /* Synchronous clock mode, input ADC clock divided by 2*/
  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;            /* 12-bit resolution for converted data */
  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;           /* Right-alignment for converted data */
  AdcHandle.Init.ScanConvMode          = DISABLE;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;           /* EOC flag picked-up to indicate conversion end */
  AdcHandle.Init.LowPowerAutoWait      = DISABLE;                       /* Auto-delayed conversion feature disabled */
  AdcHandle.Init.ContinuousConvMode    = ENABLE;                        /* Continuous mode enabled (automatic conversion restart after each conversion) */
  AdcHandle.Init.NbrOfConversion       = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.DiscontinuousConvMode = DISABLE;                       /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.NbrOfDiscConversion   = 1;                             /* Parameter discarded because sequencer is disabled */
  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because software trigger chosen */
  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC DMA continuous request to match with DMA circular mode */
  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;      /* DR register is overwritten with the last conversion result in case of overrun */
  AdcHandle.Init.OversamplingMode      = DISABLE;                       /* No oversampling */
  /* Initialize ADC peripheral according to the passed parameters */
  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
  {
    Error_Handler();
  }
  
  
  /* ### - 2 - Start calibration ############################################ */
  if (HAL_ADCEx_Calibration_Start(&AdcHandle, ADC_SINGLE_ENDED) !=  HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 3 - Channel configuration ######################################## */
  sConfig.Channel      = ADCx_CHANNEL;                /* Sampled channel number */   
      // ADCx_CHANNEL is another thing defined in main.h 
  sConfig.Rank         = ADC_REGULAR_RANK_1;          /* Rank of sampled channel number ADCx_CHANNEL */
  sConfig.SamplingTime = ADC_SAMPLETIME_6CYCLES_5;   /* Sampling time (number of clock cycles unit) */
  sConfig.SingleDiff   = ADC_SINGLE_ENDED;            /* Single-ended input channel */
  sConfig.OffsetNumber = ADC_OFFSET_NONE;             /* No offset subtraction */ 
  sConfig.Offset = 0;                                 /* Parameter discarded because offset correction is disabled */
  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* ### - 4 - Start conversion in DMA mode ################################# */
  if (HAL_ADC_Start_DMA(&AdcHandle,
                        (uint32_t *)aADCxConvertedData,
                        ADC_CONVERTED_DATA_BUFFER_SIZE
                       ) != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Infinite Loop */
  while (1)
  { 
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follows :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on*/
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */




// Testing code (not part of main.c or main.h from STM)
/**
 * @brief   Gets converted ADC value - not sure what units 
 * @param   None
 * @return  ADC value - mystery units 
 */
int16_t BspAdcGetRawValue(void) {
    // Get ADC raw data at index 0 of the Buffer 
    uint16_t data = aADCxConvertedData[0];

    return (int16_t)data;
}


int16_t BspAdcGetVoltageValue(void) {
    // Get ADC raw data at index 0 of the Buffer 
    uint16_t data = aADCxConvertedData[0];
    data = (data * 3.3) >> 12; 
        // 12 b/c 12 bits of resolution. We're assuming that the values
        // are 0 to 2^12 
    return (int16_t)data;
}



int16_t BspAdcGetMiliVoltageValue(void) {
    // Get ADC raw data at index 0 of the Buffer 
    uint16_t data = aADCxConvertedData[0];
    data = (data * 3.3 * 1000) >> 12;
        // 12 b/c 12 bits of resolution. We're assuming that the values
        // are 0 to 2^12 
    return (int16_t)data;
}