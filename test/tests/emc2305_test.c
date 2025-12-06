#include "stm32xx_hal.h"
#include "EMC2305.h"
#include "UART.h"
#include "printf.h"

#include <stdio.h>

#define DEFAULT_DEV_ADDR 0x4D
#define STATUS_LED_PORT GPIOA
#define STATUS_LED_PIN_1 GPIO_PIN_7
#define STATUS_LED_PIN_2 GPIO_PIN_8
#define STATUS_LED_PIN_3 GPIO_PIN_15

I2C_HandleTypeDef hi2c1;
EMC2305_HandleTypeDef chip;

StaticTask_t emc2305TaskBuffer_1;
StackType_t emc2305TaskStack_1[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_2;
StackType_t emc2305TaskStack_2[configMINIMAL_STACK_SIZE];

#ifdef STM32L431xx
/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

    /** Configure the main internal regulator output voltage
    */
    if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 1;
    RCC_OscInitStruct.PLL.PLLN = 20;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
    RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
    RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
        | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief This function handles I2C1 event interrupt.
  */
void I2C1_EV_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_EV_IRQn 0 */

    /* USER CODE END I2C1_EV_IRQn 0 */
    HAL_I2C_EV_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_EV_IRQn 1 */

    /* USER CODE END I2C1_EV_IRQn 1 */
}

/**
  * @brief This function handles I2C1 error interrupt.
  */
void I2C1_ER_IRQHandler(void) {
    /* USER CODE BEGIN I2C1_ER_IRQn 0 */

    /* USER CODE END I2C1_ER_IRQn 0 */
    HAL_I2C_ER_IRQHandler(&hi2c1);
    /* USER CODE BEGIN I2C1_ER_IRQn 1 */

    /* USER CODE END I2C1_ER_IRQn 1 */
}
#endif

void mx_uart_init(void) {
    // UART init
    GPIO_InitTypeDef InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

    /** Initializes the peripherals clock
    */
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    /* Peripheral clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
    InitStruct.Mode = GPIO_MODE_AF_PP;
    InitStruct.Pull = GPIO_NOPULL;
    InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &InitStruct);
}

void mx_i2c_init(void) {
    // initialize I2C pins on PSOM
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef ClkInit = { 0 };
    /* USER CODE BEGIN I2C1_MspInit 0 */

    /* USER CODE END I2C1_MspInit 0 */

    /** Initializes the peripherals clock
    */
    ClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
    ClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_PCLK1;
    if (HAL_RCCEx_PeriphCLKConfig(&ClkInit) != HAL_OK)
    {
        Error_Handler();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // I2C Interrupt Init
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

    // MX I2C Init
    hi2c1.Instance = I2C1;
    hi2c1.Init.Timing = 0x10D19CE4;
    hi2c1.Init.OwnAddress1 = 0;
    hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
    hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
    hi2c1.Init.OwnAddress2 = 0;
    hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
    hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
    hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
    if (HAL_I2C_Init(&hi2c1) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Analogue filter
    */
    if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
    {
        Error_Handler();
    }

    /** Configure Digital filter
    */
    if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
    {
        Error_Handler();
    }
}

void mx_led_init(void) {
    /* Peripheral clock enable */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // LED init
    GPIO_InitTypeDef led_init = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_15,
    };
    HAL_GPIO_Init(GPIOA, &led_init);

    // LED init
    GPIO_InitTypeDef hb_init = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_11,
    };
    HAL_GPIO_Init(GPIOB, &hb_init);
}

void EMC2305_Task_1(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    // Init UART printf
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);

    vTaskDelay(pdMS_TO_TICKS(250));

    // Initialize EMC2305
    if (EMC2305_Init(&chip, &hi2c1, DEFAULT_DEV_ADDR << 1) != EMC2305_OK) {
        Error_Handler();
    }

    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_3);

    printf("Task 1: EMC2305 Initialized\r\n");

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;
    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }

    printf("Task 1: Global Config Set\r\n");

    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = true; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
    config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
    config1.range = EMC2305_RNG_2000;
    EMC2305_Fan_Config2 config2 = { 0 };
    config2.enable_ramp_rate_ctl = true;
    config2.enable_glitch_filter = true;
    config2.error_window = EMC2305_ERG_200RPM;
    config2.derivative_options = EMC2305_DPT_BOTH;
    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Fan 2 Config Set\r\n");

    // Depends on the fan lol
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN2, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: PWM Frequency set to 19.53 kHz\r\n");

    // Set minimum drive to 0%
    // FUCK MICROCHIP THEY NEED TO KILL THEMSELVES
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Minimum Drive set to 0\r\n");

    // Set PID Gain to lowest (1x)
    // I HATE THESE BOZOS WHY IS THIS NOT THE DEFAULT
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: PID Gain set to 1x\r\n");

    // Uncomment to set outputs to push-pull
    // if (EMC2305_WriteReg(&chip, EMC2305_REG_PWM_OUTPUT_CONFIG, 0x1F) != EMC2305_OK) {
    //     Error_Handler();
    // };

    while (1) {
        // // Testing Direct PWM Drive Mode
        // // Set PWM2 duty cycle to 25%
        // if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
        //     Error_Handler();
        // };

        // Testing FSC Mode
        // Set RPM to 3000
        if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 1: RPM target set to 3000\r\n");

        // Get current rpm
        // uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
        // printf("Measured RPM: %u\r\n", rpm);

        // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink Heartbeat LED
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void EMC2305_Task_2(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    // Init UART printf
    husart1->Init.BaudRate = 115200;
    husart1->Init.WordLength = UART_WORDLENGTH_8B;
    husart1->Init.StopBits = UART_STOPBITS_1;
    husart1->Init.Parity = UART_PARITY_NONE;
    husart1->Init.Mode = UART_MODE_TX_RX;
    husart1->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    husart1->Init.OverSampling = UART_OVERSAMPLING_16;

    printf_init(husart1);

    // Initialize EMC2305
    if (EMC2305_Init(&chip, &hi2c1, DEFAULT_DEV_ADDR << 1) != EMC2305_OK) {
        Error_Handler();
    }

    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_2);

    printf("Task 2: EMC2305 Initialized\r\n");

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;
    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }

    printf("Task 2: Global Config Set\r\n");

    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = true; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
    config1.edges = EMC2305_EDG_5; // 5 edges is default for 2 pole fans
    config1.range = EMC2305_RNG_2000;
    EMC2305_Fan_Config2 config2 = { 0 };
    config2.enable_ramp_rate_ctl = true;
    config2.enable_glitch_filter = true;
    config2.error_window = EMC2305_ERG_200RPM;
    config2.derivative_options = EMC2305_DPT_BOTH;
    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 2: Fan 2 Config Set\r\n");

    // Depends on the fan lol
    if (EMC2305_SetPWMBaseFrequency(&chip, EMC2305_FAN2, EMC2305_PWM_19k53) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 2: PWM Frequency set to 19.53 kHz\r\n");

    // Set minimum drive to 0%
    // FUCK MICROCHIP THEY NEED TO KILL THEMSELVES
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_MIN_DRIVE), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 2: Minimum Drive set to 0\r\n");

    // Set PID Gain to lowest (1x)
    // I HATE THESE BOZOS WHY IS THIS NOT THE DEFAULT
    if (EMC2305_WriteReg(&chip, EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_GAIN1), 0x00) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 2: PID Gain set to 1x\r\n");

    while (1) {
        // Set RPM to 3000
        if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 2: RPM target set to 3000\r\n");

        // Get current rpm
        // uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
        // printf("Measured RPM: %u\r\n", rpm);

        // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink Heartbeat LED
        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_1);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

int main(void) {
#ifdef STM32L431xx
    // initialize the HAL and system clock
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    // Init peripherals
    mx_uart_init();
    mx_i2c_init();
    mx_led_init();
#endif

    xTaskCreateStatic(EMC2305_Task_1,
        "EMC2305 Task 1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        emc2305TaskStack_1,
        &emc2305TaskBuffer_1);

    xTaskCreateStatic(EMC2305_Task_2,
        "EMC2305 Task 2",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        emc2305TaskStack_2,
        &emc2305TaskBuffer_2);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}
