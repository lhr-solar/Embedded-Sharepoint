/**
  ******************************************************************************
  * @file    CAN/CAN_LoopBack/Src/main.c 
  * @author  MCD Application Team
  * @brief   This example provides a description of how to set a communication 
  *          with the CAN in loopback mode.
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
// #include "main.h"
#include "stm32xx_hal.h"
#include "stm32f4xx.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup CAN_LoopBack
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef     CanHandle;
CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint8_t               TxData[8];
uint8_t               RxData[8];
uint32_t              TxMailbox;

SemaphoreHandle_t can1_send_semaphore = NULL;
StaticSemaphore_t can1_send_semaphore_buffer;

/* Private function prototypes -----------------------------------------------*/
// static void SystemClock_Config(void);
static void Error_Handler(void);
static HAL_StatusTypeDef CAN_Polling(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization: global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  // SystemClock_Config();

  GPIO_InitTypeDef led_config = {
      .Mode = GPIO_MODE_OUTPUT_PP,
      .Pull = GPIO_NOPULL,
      .Pin = GPIO_PIN_5
  };
  
  __HAL_RCC_GPIOA_CLK_ENABLE(); // enable clock for GPIOA
  HAL_GPIO_Init(GPIOA, &led_config); // initialize GPIOA with led_config
  
  /* Configure LED1, LED2 and LED3 */
  // BSP_LED_Init(LED1);
  // BSP_LED_Init(LED2);
  // BSP_LED_Init(LED3);

  // can1_send_semaphore = xSemaphoreCreateBinaryStatic(
  //           &can1_send_semaphore_buffer);

  CAN_Polling();

  /* Infinite loop */
  while (1)
  {
    HAL_Delay(500);
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
  } 
  
}

/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */
HAL_StatusTypeDef CAN_Polling(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  
  /*##-1- Configure the CAN peripheral #######################################*/
  CanHandle.Instance = CAN1;
    
  CanHandle.Init.TimeTriggeredMode = DISABLE;
  CanHandle.Init.AutoBusOff = DISABLE;
  CanHandle.Init.AutoWakeUp = DISABLE;
  CanHandle.Init.AutoRetransmission = ENABLE;
  CanHandle.Init.ReceiveFifoLocked = DISABLE;
  CanHandle.Init.TransmitFifoPriority = DISABLE;
  CanHandle.Init.Mode = CAN_MODE_LOOPBACK;
  CanHandle.Init.SyncJumpWidth = CAN_SJW_1TQ;
  CanHandle.Init.TimeSeg1 = CAN_BS1_6TQ;
  CanHandle.Init.TimeSeg2 = CAN_BS2_2TQ;
  CanHandle.Init.Prescaler = 5;
  
  if(HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }


  /*##-2- Configure the CAN Filter ###########################################*/
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = 0x0000;
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&CanHandle, &sFilterConfig) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }

  HAL_CAN_ActivateNotification(&CanHandle, CAN_IT_TX_MAILBOX_EMPTY);

  /*##-3- Start the CAN peripheral ###########################################*/
  if (HAL_CAN_Start(&CanHandle) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }

  /*##-4- Start the Transmission process #####################################*/
  TxHeader.StdId = 0x11;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;
  TxData[0] = 0xCA;
  TxData[1] = 0xFE;
  
  /* Request transmission */
  if(HAL_CAN_AddTxMessage(&CanHandle, &TxHeader, TxData, &TxMailbox) != HAL_OK)
  {
    /* Transmission request Error */
    Error_Handler();
  }
  
  // /* Wait transmission complete */
  // while(HAL_CAN_GetTxMailboxesFreeLevel(&CanHandle) != 3) {}

  /*##-5- Start the Reception process ########################################*/
  // if(HAL_CAN_GetRxFifoFillLevel(&CanHandle, CAN_RX_FIFO0) != 1)
  // {
  //   /* Reception Missing */
  //   Error_Handler();
  // }

  // if(HAL_CAN_GetRxMessage(&CanHandle, CAN_RX_FIFO0, &RxHeader, RxData) != HAL_OK)
  // {
  //   /* Reception Error */
  //   Error_Handler();
  // }

  // if((RxHeader.StdId != 0x11)                     ||
  //    (RxHeader.RTR != CAN_RTR_DATA)               ||
  //    (RxHeader.IDE != CAN_ID_STD)                 ||
  //    (RxHeader.DLC != 2)                          ||
  //    ((RxData[0]<<8 | RxData[1]) != 0xCAFE))
  // {
  //   /* Rx message Error */
  //   return HAL_ERROR;
  // }

  return HAL_OK; /* Test Passed */
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
// static void SystemClock_Config(void)
// {
//   RCC_ClkInitTypeDef RCC_ClkInitStruct;
//   RCC_OscInitTypeDef RCC_OscInitStruct;
//   HAL_StatusTypeDef ret = HAL_OK;

//   /* Enable Power Control clock */
//   __HAL_RCC_PWR_CLK_ENABLE();

//   /* The voltage scaling allows optimizing the power consumption when the device is 
//      clocked below the maximum system frequency, to update the voltage scaling value 
//      regarding system frequency refer to product datasheet.  */
//   __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

//   /* Enable HSE Oscillator and activate PLL with HSE as source */
//   RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
//   RCC_OscInitStruct.HSEState = RCC_HSE_ON;
//   RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
//   RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
//   RCC_OscInitStruct.PLL.PLLM = 25;
//   RCC_OscInitStruct.PLL.PLLN = 360;
//   RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
//   RCC_OscInitStruct.PLL.PLLQ = 7;
//   RCC_OscInitStruct.PLL.PLLR = 6;
  
//   ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
//   if(ret != HAL_OK)
//   {
//     while(1) { ; }
//   }
  
//   /* Activate the OverDrive to reach the 180 MHz Frequency */  
//   ret = HAL_PWREx_EnableOverDrive();
//   if(ret != HAL_OK)
//   {
//     while(1) { ; }
//   }
  
//   /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
//   RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
//   RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
//   RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
//   RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
//   RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
//   ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
//   if(ret != HAL_OK)
//   {
//     while(1) { ; }
//   }
// }


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  /* Turn LED3 on */
  // BSP_LED_On(LED3);
  while(1)
  {
  }
}

// void Default_Handler(void){
//   volatile uint32_t active_irq = SCB->ICSR & 0x1FF;
//   (void)active_irq;
// }

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
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
