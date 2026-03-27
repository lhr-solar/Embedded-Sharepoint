// A simple echo application to test input and output over serial
#include <string.h>
#include "stm32xx_hal.h"
#include "printf.h"

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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
void HardFault_Handler(){
    // Configurable Fault Status Register
    // Consists of MMSR, BFSR and UFSR
    volatile uint32_t _CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;

    // Hard Fault Status Register
    volatile uint32_t _HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

    // Debug Fault Status Register
    volatile uint32_t _DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

    // Auxiliary Fault Status Register
    volatile uint32_t _AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

    // Read the Fault Address Registers. These may not contain valid values.
    // Check BFARVALID/MMARVALID to see if they are valid values
    // MemManage Fault Address Register
    volatile uint32_t _MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
    // Bus Fault Address Register
    volatile uint32_t _BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

    __asm("BKPT #0\n") ; // Break into the debugger
}
#pragma GCC diagnostic pop

#define THREAD_MULTIPLIER 5
#define DUMP_SIZE 100

static char *messages[] = {
    "This is a message I expect to never interleave!\n\r",
    "The quick brown fox jumped over the lazy dog!\n\r",
    "5.5\n\r",
    "3.777\n\r",
    "I'm going to write a really long message to test long transmission lengths to confirm that interleaving is not a problem with these sorts of messages.\n\r"
};

StaticTask_t txTaskBuffer[sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER];
StackType_t txTaskStack[sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER][configMINIMAL_STACK_SIZE];

static uint64_t time_dump[sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER][DUMP_SIZE] = {0};
static volatile uint64_t time_max = 0;
static bool tx_done[sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER] = {};

void TxTask(void *argument){
    uint8_t time_ind = 0;

    for(int i=0; i<100; i++){
        volatile int time_before = DWT->CYCCNT;
        //printf(messages[((int)argument % (sizeof(messages)/sizeof(char*)))]);
        uart_send(husart2, (const uint8_t *)messages[((int)argument % (sizeof(messages)/sizeof(char*)))], strlen(messages[((int)argument % (sizeof(messages)/sizeof(char*)))]), portMAX_DELAY);
        time_dump[(int)argument][time_ind++] = (time_before - DWT->CYCCNT);

        if(time_max < time_dump[(int)argument][time_ind-1]) time_max = time_dump[(int)argument][time_ind-1];

        time_ind = time_ind % DUMP_SIZE;

        vTaskDelay(pdMS_TO_TICKS(0));
    }

    tx_done[(int)argument] = true;
    while(1){vTaskDelay(1000);}
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

    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; // Enable trace
    }

    DWT->CYCCNT = 0;                // Reset counter
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; // Enable cycle counter
    
    for(int i=0; i<sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER; i++){
        tx_done[i] = false;
        for(int j=0; j<DUMP_SIZE; j++) time_dump[i][j] = 0;
        xTaskCreateStatic(TxTask, 
                        "TX",
                        configMINIMAL_STACK_SIZE,
                        ((void*)i),
                        tskIDLE_PRIORITY + 2,
                        txTaskStack[i],
                        &txTaskBuffer[i]);
    }

    while(1){
        bool all_done = true;
        for(int i=0; i<sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER; i++){
            all_done &= tx_done[i];
        }

        if(all_done){
            for(int i=0; i<sizeof(messages)/sizeof(char*) * THREAD_MULTIPLIER; i++){
                printf("Times for TID %d: ", i);
                for(int j=0; j<DUMP_SIZE; j++){
                    printf("%d ", time_dump[i][j]);
                }
                printf("\n\r");
            }

            printf("Max time: %d\n\r", time_max);

            break;
        }
    }

    while(1){}
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    xTaskCreateStatic(InitTask, 
                      "Init", 
                      configMINIMAL_STACK_SIZE, 
                      NULL, 
                      tskIDLE_PRIORITY + 1, 
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
