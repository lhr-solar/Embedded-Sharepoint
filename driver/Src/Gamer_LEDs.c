/**
 * This is the driver for addressable LEDs. 
 * The VCAT lighting board uses WS2814 LEDs which read an NZR protocol to set the color for each individual LED.
 * This is done by sending an array to be sent using DMA. The STM will automatically use these values to set PWM duration.
 */

#include "Gamer_LEDs.h"


 /************** Lights and config defines ***************/


                // PWM Cycle Values //

 // For DMA; Number of cycles that need to send high to send a one or zero to the LEDs
#ifdef WS2812B // # Cycles high for a 100 cycle period. Assuming 12.5ns clock cycle (set counter to 99)
#define PWM_ONE_HIGH 64 // 800ns
#define PWM_ZERO_HIGH 32 // 400ns
#else // # Cycles high for WS2814 in a 100 cycle period using 12.5ns per cycle (set counter to 99)
#define PWM_ONE_HIGH 76 // 950ns
#define PWM_ZERO_HIGH 28 // 350ns
#endif

                // DMA handles //

 #ifdef STM32L432_PA8 // PA8 is the pin used for LEDs on the TPS testboard
 extern TIM_HandleTypeDef htim1; // Is it better to extern here and include variables elsewhere?
 DMA_HandleTypeDef hdma_tim1_ch1; 
 #else // The lighting board will have three addressable LED pins across two timers
 extern TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim1_ch4_trig_com;
DMA_HandleTypeDef hdma_tim2_ch1;
DMA_HandleTypeDef hdma_tim2_ch3;
 #endif

 // defines
 #ifdef WS2812B
 #define BYTES_PER_PIXEL 3 // Neopixel/WS2812b only has RGB
 #else
 #define BYTES_PER_PIXEL 4 // WS2814 has four colors: RGBW
 #endif

 #define NUM_PIXELS 2                             // Number of LEDs
 #define NUM_BYTES (BYTES_PER_PIXEL * NUM_PIXELS) // Total bytes per message (for all LEDs)
 #define BITS_PER_BYTE 8
 #define RESET_CYCLES 50 // Need the line low for >50 us to reset after each message. 
 #define WR_BUF_LENGTH (NUM_BYTES * BITS_PER_BYTE + RESET_CYCLES) // Include extra bits for setting PWM low (0 cycles on each period)


 /************** Function Prototypes ***************/

 static void MX_DMA_Init(void);
 static void MX_GPIO_Init(void);
 static void MX_TIM1_Init(void);
 #ifndef STM32L432_PA8
 static void MX_TIM2_Init(void);// TODO: Test including other init functions for lighting board?
 #endif


 /************** Data buffers ***************/

 // buffer of colors
 //uint8_t color_array[NUM_BYTES] = {0}; // TODO: Each LED takes in a value from 0-255 for each color (R, G, B, W if 2814)
 // Will likely need multiple since there will be mutiple LED pins per board

 // buffer to send to DMA
 uint8_t write_buf[(WR_BUF_LENGTH)] = {0}; // Hard-code size and extra reset zeroes for now


 /************** Functions ***************/

 // TODO:   Write functions that set LED colors using a color buffer that can be set via CAN
 //         Write functions that convert LED colors into binary using PWM cycles
 //         Example code is included at the bottom of this file

 /**
  * Test function; currently nothing works, but this is probably an issue at a lower level rather than one in this function
  */
 void test_set_leds_high() {
    // Hardcode send high for everything for now
    for (uint8_t i = 0; i < WR_BUF_LENGTH; i++) {
        write_buf[i] = PWM_ONE_HIGH;
    }

    // This is how we tell PWM to read the buffer of clock cycles using DMA
    #ifdef STM32L432_PA8 // The timers for the pins on the testboard are different than the lighting board timers/pins
    HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)write_buf, WR_BUF_LENGTH);
    #endif
    // TODO: Start the PWM timers for the lighting board (TIM2_CH1, TIM2_CH3, TIM1_CH4)
    // Depending on which pins you want to set high

 }

 /**
  * To initialize the LEDs, we need to call the MSP init functions
  */
 void Gamer_LEDs_Init() {
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM1_Init();
    #ifndef STM32L432_PA8
    MX_TIM2_Init(); // TODO: Test init TIM2 if on the lighting board instead of the testing board
    #endif

    

 }

 // Example code for the Neopixel running on an STM32F042 
 // from https://www.thevfdcollective.com/blog/stm32-and-sk6812-rgbw-led
 // Could follow this model to convert colors to PWM cycles for DMA transfer

/*
static inline uint8_t scale8(uint8_t x, uint8_t scale) {
  return ((uint16_t)x * scale) >> 8;
}

// Set a single color (RGB) to index
void led_set_RGB(uint8_t index, uint8_t r, uint8_t g, uint8_t b) {
#if (NUM_BPP == 4) // SK6812
  rgb_arr[4 * index] = scale8(g, 0xB0); // g;
  rgb_arr[4 * index + 1] = r;
  rgb_arr[4 * index + 2] = scale8(b, 0xF0); // b;
  rgb_arr[4 * index + 3] = 0;
#else // WS2812B
  rgb_arr[3 * index] = scale8(g, 0xB0); // g;
  rgb_arr[3 * index + 1] = r;
  rgb_arr[3 * index + 2] = scale8(b, 0xF0); // b;
#endif // End SK6812 WS2812B case differentiation
}

// Set a single color (RGBW) to index
void led_set_RGBW(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  led_set_RGB(index, r, g, b);
#if (NUM_BPP == 4) // SK6812
  rgb_arr[4 * index + 3] = w;
#endif // End SK6812 WS2812B case differentiation
}

// Set all colors to RGB
void led_set_all_RGB(uint8_t r, uint8_t g, uint8_t b) {
  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) led_set_RGB(i, r, g, b);
}

// Set all colors to RGBW
void led_set_all_RGBW(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
  for(uint_fast8_t i = 0; i < NUM_PIXELS; ++i) led_set_RGBW(i, r, g, b, w);
}

// Shuttle the data to the LEDs!
void led_render() {
  if(wr_buf_p != 0 || hdma_tim2_ch1.State != HAL_DMA_STATE_READY) {
    // Ongoing transfer, cancel!
    for(uint8_t i = 0; i < WR_BUF_LEN; ++i) wr_buf[i] = 0;
    wr_buf_p = 0;
    HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
    return;
  }
  // Ooh boi the first data buffer half (and the second!)
#if (NUM_BPP == 4) // SK6812
  for(uint_fast8_t i = 0; i < 8; ++i) {
    wr_buf[i     ] = PWM_LO << (((rgb_arr[0] << i) & 0x80) > 0);
    wr_buf[i +  8] = PWM_LO << (((rgb_arr[1] << i) & 0x80) > 0);
    wr_buf[i + 16] = PWM_LO << (((rgb_arr[2] << i) & 0x80) > 0);
    wr_buf[i + 24] = PWM_LO << (((rgb_arr[3] << i) & 0x80) > 0);
    wr_buf[i + 32] = PWM_LO << (((rgb_arr[4] << i) & 0x80) > 0);
    wr_buf[i + 40] = PWM_LO << (((rgb_arr[5] << i) & 0x80) > 0);
    wr_buf[i + 48] = PWM_LO << (((rgb_arr[6] << i) & 0x80) > 0);
    wr_buf[i + 56] = PWM_LO << (((rgb_arr[7] << i) & 0x80) > 0);
  }
#else // WS2812B
  for(uint_fast8_t i = 0; i < 8; ++i) {
    wr_buf[i     ] = PWM_LO << (((rgb_arr[0] << i) & 0x80) > 0);
    wr_buf[i +  8] = PWM_LO << (((rgb_arr[1] << i) & 0x80) > 0);
    wr_buf[i + 16] = PWM_LO << (((rgb_arr[2] << i) & 0x80) > 0);
    wr_buf[i + 24] = PWM_LO << (((rgb_arr[3] << i) & 0x80) > 0);
    wr_buf[i + 32] = PWM_LO << (((rgb_arr[4] << i) & 0x80) > 0);
    wr_buf[i + 40] = PWM_LO << (((rgb_arr[5] << i) & 0x80) > 0);
  }
#endif // End SK6812 WS2812B case differentiation

  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_1, (uint32_t *)wr_buf, WR_BUF_LEN);
  wr_buf_p = 2; // Since we're ready for the next buffer
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  // DMA buffer set from LED(wr_buf_p) to LED(wr_buf_p + 1)
  if(wr_buf_p < NUM_PIXELS) {
    // We're in. Fill the even buffer
#if (NUM_BPP == 4) // SK6812
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i     ] = PWM_LO << (((rgb_arr[4 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i +  8] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 16] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 2] << i) & 0x80) > 0);
      wr_buf[i + 24] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 3] << i) & 0x80) > 0);
    }
#else // WS2812B
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i     ] = PWM_LO << (((rgb_arr[3 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i +  8] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 16] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 2] << i) & 0x80) > 0);
    }
#endif // End SK6812 WS2812B case differentiation
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    // Last two transfers are resets. SK6812: 64 * 1.25 us = 80 us == good enough reset
  	//                               WS2812B: 48 * 1.25 us = 60 us == good enough reset
    // First half reset zero fill
    for(uint8_t i = 0; i < WR_BUF_LEN / 2; ++i) wr_buf[i] = 0;
    wr_buf_p++;
  }
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
  // DMA buffer set from LED(wr_buf_p) to LED(wr_buf_p + 1)
  if(wr_buf_p < NUM_PIXELS) {
    // We're in. Fill the odd buffer
#if (NUM_BPP == 4) // SK6812
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i + 32] = PWM_LO << (((rgb_arr[4 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i + 40] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 48] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 2] << i) & 0x80) > 0);
      wr_buf[i + 56] = PWM_LO << (((rgb_arr[4 * wr_buf_p + 3] << i) & 0x80) > 0);
    }
#else // WS2812B
    for(uint_fast8_t i = 0; i < 8; ++i) {
      wr_buf[i + 24] = PWM_LO << (((rgb_arr[3 * wr_buf_p    ] << i) & 0x80) > 0);
      wr_buf[i + 32] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 1] << i) & 0x80) > 0);
      wr_buf[i + 40] = PWM_LO << (((rgb_arr[3 * wr_buf_p + 2] << i) & 0x80) > 0);
    }
#endif // End SK6812 WS2812B case differentiation
    wr_buf_p++;
  } else if (wr_buf_p < NUM_PIXELS + 2) {
    // Second half reset zero fill
    for(uint8_t i = WR_BUF_LEN / 2; i < WR_BUF_LEN; ++i) wr_buf[i] = 0;
    ++wr_buf_p;
  } else {
    // We're done. Lean back and until next time!
    wr_buf_p = 0;
    //HAL_TIM_PWM_Stop_DMA(&htim2, TIM_CHANNEL_1);
  }
}

*/


 // /// /// /// /// /// /// /// /// /// ///
 // // // // // MSP Functions // // // // /
 /// /// /// /// /// /// /// /// /// /// //

#ifdef STM32L432_PA8 // Functions for the test board
 void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

 /**
  * @brief TIM_Base MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
        // Initialize TIM1
    if(htim_base->Instance==TIM1)
    {
        /* USER CODE BEGIN TIM1_MspInit 0 */

        /* USER CODE END TIM1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_TIM1_CLK_ENABLE();

        /* TIM1 DMA Init */
        /* TIM1_CH1 Init */
        hdma_tim1_ch1.Instance = DMA1_Channel2;
        hdma_tim1_ch1.Init.Request = DMA_REQUEST_7;
        hdma_tim1_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
        hdma_tim1_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_tim1_ch1.Init.MemInc = DMA_MINC_ENABLE;
        hdma_tim1_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
        hdma_tim1_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_tim1_ch1.Init.Mode = DMA_CIRCULAR;
        hdma_tim1_ch1.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_tim1_ch1) != HAL_OK)
        {
        Error_Handler();
        }

        __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC1],hdma_tim1_ch1);

        /* USER CODE BEGIN TIM1_MspInit 1 */

        /* USER CODE END TIM1_MspInit 1 */
    }

}


void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};


    if(htim->Instance==TIM1)
    {
    /* USER CODE BEGIN TIM1_MspPostInit 0 */

    /* USER CODE END TIM1_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM1_MspPostInit 1 */

    /* USER CODE END TIM1_MspPostInit 1 */
    }

}

/**
  * @brief TIM_Base MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
    
    if(htim_base->Instance==TIM1)
    {
    /* USER CODE BEGIN TIM1_MspDeInit 0 */

    /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 DMA DeInit */
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC1]);
    /* USER CODE BEGIN TIM1_MspDeInit 1 */

    /* USER CODE END TIM1_MspDeInit 1 */
    }
    

}

#else // Functions for the lighting board

/**
  * @brief TIM_Base MSP Initialization
  * This function configures the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    /* USER CODE BEGIN TIM1_MspInit 0 */

    /* USER CODE END TIM1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    /* TIM1 DMA Init */
    /* TIM1_CH4_TRIG_COM Init */
    hdma_tim1_ch4_trig_com.Instance = DMA1_Channel4;
    hdma_tim1_ch4_trig_com.Init.Request = DMA_REQUEST_7;
    hdma_tim1_ch4_trig_com.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim1_ch4_trig_com.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim1_ch4_trig_com.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim1_ch4_trig_com.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tim1_ch4_trig_com.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tim1_ch4_trig_com.Init.Mode = DMA_CIRCULAR;
    hdma_tim1_ch4_trig_com.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_tim1_ch4_trig_com) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one channel to perform all the requested DMAs. */
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC4],hdma_tim1_ch4_trig_com);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_TRIGGER],hdma_tim1_ch4_trig_com);
    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_COMMUTATION],hdma_tim1_ch4_trig_com);

    /* USER CODE BEGIN TIM1_MspInit 1 */

    /* USER CODE END TIM1_MspInit 1 */
  }
  else if(htim_base->Instance==TIM2)
  {
    /* USER CODE BEGIN TIM2_MspInit 0 */

    /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* TIM2 DMA Init */
    /* TIM2_CH1 Init */
    hdma_tim2_ch1.Instance = DMA1_Channel5;
    hdma_tim2_ch1.Init.Request = DMA_REQUEST_4;
    hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tim2_ch1.Init.Mode = DMA_CIRCULAR;
    hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC1],hdma_tim2_ch1);

    /* TIM2_CH3 Init */
    hdma_tim2_ch3.Instance = DMA1_Channel1;
    hdma_tim2_ch3.Init.Request = DMA_REQUEST_4;
    hdma_tim2_ch3.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim2_ch3.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim2_ch3.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim2_ch3.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_tim2_ch3.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_tim2_ch3.Init.Mode = DMA_CIRCULAR;
    hdma_tim2_ch3.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_tim2_ch3) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(htim_base,hdma[TIM_DMA_ID_CC3],hdma_tim2_ch3);

    /* USER CODE BEGIN TIM2_MspInit 1 */

    /* USER CODE END TIM2_MspInit 1 */
  }

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(htim->Instance==TIM1)
  {
    /* USER CODE BEGIN TIM1_MspPostInit 0 */

    /* USER CODE END TIM1_MspPostInit 0 */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA11     ------> TIM1_CH4
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM1_MspPostInit 1 */

    /* USER CODE END TIM1_MspPostInit 1 */
  }
  else if(htim->Instance==TIM2)
  {
    /* USER CODE BEGIN TIM2_MspPostInit 0 */

    /* USER CODE END TIM2_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    PA2     ------> TIM2_CH3
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM2_MspPostInit 1 */

    /* USER CODE END TIM2_MspPostInit 1 */
  }

}
/**
  * @brief TIM_Base MSP De-Initialization
  * This function freeze the hardware resources used in this example
  * @param htim_base: TIM_Base handle pointer
  * @retval None
  */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {
    /* USER CODE BEGIN TIM1_MspDeInit 0 */

    /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /* TIM1 DMA DeInit */
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC4]);
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_TRIGGER]);
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_COMMUTATION]);
    /* USER CODE BEGIN TIM1_MspDeInit 1 */

    /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(htim_base->Instance==TIM2)
  {
    /* USER CODE BEGIN TIM2_MspDeInit 0 */

    /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 DMA DeInit */
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC1]);
    HAL_DMA_DeInit(htim_base->hdma[TIM_DMA_ID_CC3]);
    /* USER CODE BEGIN TIM2_MspDeInit 1 */

    /* USER CODE END TIM2_MspDeInit 1 */
  }

}
#endif

 // /// /// /// /// /// /// /// /// /// ///
 // // // Initialization Functions // // //
 /// /// /// /// /// /// /// /// /// /// //

#ifdef STM32L432_PA8
/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;

  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 99;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}
#else

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;

  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 99;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4 != HAL_OK))
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }

  HAL_TIM_MspPostInit(&htim2);

}
#endif



/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
    #ifdef STM32L432_PA8

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel2_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);
    
    
    #else // For lighting board (uses more leds/pins/DMA channels)

    /* DMA controller clock enable */
    __HAL_RCC_DMA1_CLK_ENABLE();

    /* DMA interrupt init */
    /* DMA1_Channel1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    /* DMA1_Channel4_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
    /* DMA1_Channel5_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);
    #endif
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
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
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */



// I don't think we need these(?), but they're here for now

// /**
//   * @brief This function handles DMA1 channel1 global interrupt.
//   */
// void DMA1_Channel1_IRQHandler(void)
// {
//   /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

//   /* USER CODE END DMA1_Channel1_IRQn 0 */
//   HAL_DMA_IRQHandler(&hdma_tim2_ch3);
//   /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

//   /* USER CODE END DMA1_Channel1_IRQn 1 */
// }

// /**
//   * @brief This function handles DMA1 channel4 global interrupt.
//   */
// void DMA1_Channel4_IRQHandler(void)
// {
//   /* USER CODE BEGIN DMA1_Channel4_IRQn 0 */

//   /* USER CODE END DMA1_Channel4_IRQn 0 */
//   HAL_DMA_IRQHandler(&hdma_tim1_ch4_trig_com);
//   /* USER CODE BEGIN DMA1_Channel4_IRQn 1 */

//   /* USER CODE END DMA1_Channel4_IRQn 1 */
// }

// /**
//   * @brief This function handles DMA1 channel5 global interrupt.
//   */
// void DMA1_Channel5_IRQHandler(void)
// {
//   /* USER CODE BEGIN DMA1_Channel5_IRQn 0 */

//   /* USER CODE END DMA1_Channel5_IRQn 0 */
//   HAL_DMA_IRQHandler(&hdma_tim2_ch1);
//   /* USER CODE BEGIN DMA1_Channel5_IRQn 1 */

//   /* USER CODE END DMA1_Channel5_IRQn 1 */
// }
