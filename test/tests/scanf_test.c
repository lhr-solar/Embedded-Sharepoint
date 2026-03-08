// A simple echo application to test input and output over serial
#include "stm32xx_hal.h"
#include "printf.h"

StaticTask_t txTaskBuffer;
#define TX_STACK_SIZE (configMINIMAL_STACK_SIZE*4)
StackType_t txTaskStack[TX_STACK_SIZE];

StaticTask_t rxTaskBuffer;
#define RX_STACK_SIZE (configMINIMAL_STACK_SIZE*4)
StackType_t rxTaskStack[RX_STACK_SIZE];

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];

void HAL_UART_MspGPIOInit(UART_HandleTypeDef *huart){
    GPIO_InitTypeDef init = {0};
    UNUSED(init);

#ifdef STM32F4xx
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* enable port A USART2 gpio
    PA2 -> USART2_TX
    PA3 -> USART2_RX
    */
    init.Pin = GPIO_PIN_2|GPIO_PIN_3;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &init);
#endif

#ifdef STM32G4xx
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /* enable port C USART3 gpio
    PC10 -> USART3_TX
    PC11 -> USART3_RX
    */
    init.Pin = GPIO_PIN_10|GPIO_PIN_11;
    init.Mode = GPIO_MODE_AF_PP;
    init.Pull = GPIO_NOPULL;
    init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    init.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOC, &init);
#endif
}

// Semaphore for the buffer
StaticSemaphore_t buf_sem4_buffer;
SemaphoreHandle_t buf_sem4;
#define BUF_LEN 128
char buf[BUF_LEN];

void RxTask(void *argument){
    while(1){
        // 127: read max 127 characters + null terminator (buffer sized to 128)
        // [^\r\n]: read until next newline or CR character (include any form of whitespace in the string)
        // the leading space is crucial: In scanf format strings, any whitespace in the format string means 
        // “skip all consecutive whitespace characters in the input.”
        // without the leading space, scanf early returns because there's constantly a leading whitespace (either a \r or \n)
        // in the buffer.

        // an argument can be made to just not use scanf at all and instead use fgets. it consumes less stack space
        // and is more convenient for parsing rather than this format string BS.
        // TODO: add fgets to system
        
        // you can also use %s to detect strings; issue is, it will ignore whitespace and only give you the last string
        // typed with no whitespace.

        int ret = scanf(" %127[^\r\n]", buf);
        if(ret == 1) xSemaphoreGive(buf_sem4);
        else{ printf("ERROR ERROR ERROR!\r\n");}
    }   
}

// #pragma GCC diagnostic push
// #pragma GCC diagnostic ignored "-Wunused-variable"
// void HardFault_Handler(){
//     // Configurable Fault Status Register
//     // Consists of MMSR, BFSR and UFSR
//     volatile uint32_t _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;
//
//     // Hard Fault Status Register
//     volatile uint32_t _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;
//
//     // Debug Fault Status Register
//     volatile uint32_t _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;
//
//     // Auxiliary Fault Status Register
//     volatile uint32_t _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;
//
//     // Read the Fault Address Registers. These may not contain valid values.
//     // Check BFARVALID/MMARVALID to see if they are valid values
//     // MemManage Fault Address Register
//     volatile uint32_t _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
//     // Bus Fault Address Register
//     volatile uint32_t _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;
//
//     __asm("BKPT #0\n") ; // Break into the debugger
// }
// #pragma GCC diagnostic pop

void TxTask(void *argument){
    while(1){
        xSemaphoreTake(buf_sem4, portMAX_DELAY);
        printf("You said: %s\r\n", buf);
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void InitTask(void *argument){

#ifdef STM32F4xx
    husart2->Init.BaudRate = 115200;
    husart2->Init.WordLength = UART_WORDLENGTH_8B;
    husart2->Init.StopBits = UART_STOPBITS_1;
    husart2->Init.Parity = UART_PARITY_NONE;
    husart2->Init.Mode = UART_MODE_TX_RX;
    husart2->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart2->Init.OverSampling = UART_OVERSAMPLING_16;
    printf_init(husart2);
#endif

#ifdef STM32G4xx
    husart3->Init.BaudRate = 115200;
    husart3->Init.WordLength = UART_WORDLENGTH_8B;
    husart3->Init.StopBits = UART_STOPBITS_1;
    husart3->Init.Parity = UART_PARITY_NONE;
    husart3->Init.Mode = UART_MODE_TX_RX;
    husart3->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart3->Init.OverSampling = UART_OVERSAMPLING_16;
    printf_init(husart3);
#endif
    
    buf_sem4 = xSemaphoreCreateBinaryStatic(&buf_sem4_buffer);

    xTaskCreateStatic(TxTask, 
                     "TX",
                     TX_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     txTaskStack,
                     &txTaskBuffer);

    xTaskCreateStatic(RxTask, 
                     "RX",
                     RX_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     rxTaskStack,
                     &rxTaskBuffer);

    while(1){}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    xTaskCreateStatic(InitTask, 
                     "Init",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     initTaskStack,
                     &initTaskBuffer);

    vTaskStartScheduler();

    while (1) {
    }
}

// autogenerated
#ifdef STM32G4xx
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}
#endif
