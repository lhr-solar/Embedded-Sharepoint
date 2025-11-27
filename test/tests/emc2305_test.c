#include "stm32xx_hal.h"
#include "EMC2305.h"
#include "UART.h"

#include <stdio.h>

#define DEFAULT_DEV_ADDR 0x4D
#define STATUS_LED_PORT GPIOB
#define STATUS_LED_PIN GPIO_PIN_11

I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
EMC2305_HandleTypeDef chip;


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
#endif

int main(void) {
    // initialize the HAL and system clock
    if (HAL_Init() != HAL_OK) Error_Handler();
    SystemClock_Config();
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Peripheral clock enable */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // LED init
    GPIO_InitTypeDef led_init = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = STATUS_LED_PIN
    };
    HAL_GPIO_Init(STATUS_LED_PORT, &led_init);

    // UART init
    GPIO_InitTypeDef InitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };
    /* USER CODE BEGIN USART1_MspInit 0 */

    /* USER CODE END USART1_MspInit 0 */

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

    /* USER CODE BEGIN USART1_MspInit 1 */

    /* USER CODE END USART1_MspInit 1 */

    huart1.Instance = USART1;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }

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

    uint8_t data[] = "EMC2305 Fan Controller Test\r\n";
    uint8_t msgLen = sizeof(data) - 1;
    HAL_UART_Transmit(&huart1, data, msgLen, 1000);

    // Initialize EMC2305
    if (EMC2305_Init(&chip, &hi2c1, DEFAULT_DEV_ADDR << 1) != EMC2305_OK) {
        Error_Handler();
    }

    uint8_t data1[] = "Chip Initialized\r\n";
    msgLen = sizeof(data1) - 1;
    HAL_UART_Transmit(&huart1, data1, msgLen, 1000);

    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = false;
    EMC2305_Fan_Config2 config2 = { 0 };
    if (EMC2305_SetFanConfig(&chip, EMC2305_FAN2, &config1, &config2) != EMC2305_OK) {
        Error_Handler();
    };

    uint8_t data2[] = "Chip Configured\r\n";
    msgLen = sizeof(data2) - 1;
    HAL_UART_Transmit(&huart1, data2, msgLen, 1000);

    while (1) {

        // uint8_t product_id = 69;
        // EMC2305_ReadReg(&chip, EMC2305_REG_PRODUCT_ID, &product_id);

        // char buffer[10]; // Adjust size as needed
        // sprintf(buffer, "%d", product_id);
        // uint8_t msgLen = sizeof(buffer) - 1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, msgLen, 1000);

        // uint8_t data2[] = "\r\n";
        // msgLen = sizeof(data2) - 1;
        // HAL_UART_Transmit(&huart1, data2, msgLen, 1000);



        // uint8_t mfg_id = 69;
        // EMC2305_ReadReg(&chip, EMC2305_REG_PRODUCT_ID, &mfg_id);

        // sprintf(buffer, "%d", mfg_id);
        // msgLen = sizeof(buffer) - 1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, msgLen, 1000);

        // msgLen = sizeof(data2) - 1;
        // HAL_UART_Transmit(&huart1, data2, msgLen, 1000);




        // // Send I2C
        // HAL_I2C_Master_Transmit(&hi2c1, 0x4D << 1, &reg, 1, 100);
        // HAL_Delay(1);
        // HAL_I2C_Master_Receive(&hi2c1, 0x4D << 1, &val, 1, 100);



        // char buffer[22];
        // char text[] = "Configuration: ";
        // snprintf(buffer, sizeof(buffer), "%s: %d\n", text, val);
        // msgLen = sizeof(buffer) - 1;
        // HAL_UART_Transmit(&huart1, (uint8_t*)buffer, msgLen, 1000);

        // // 1. Disable RPM/FSC for Fan2 (0x42 → 0x0B)
        // reg = EMC2305_REG_FAN2_CONFIG1; val = 0x0B;
        // HAL_I2C_Master_Transmit(&hi2c1, 0x4D << 1, (uint8_t[]) { reg, val }, 2, 100);

        // // 2. OPTIONAL: Set PWM2 to push-pull instead of open drain (0x2B → bit1 = 1)
        // // reg = EMC2305_REG_PWM_OUTPUT_CONFIG; val = 0x02;
        // // HAL_I2C_Master_Transmit(&hi2c1, 0x4D << 1, (uint8_t[]) { reg, val }, 2, 100);

        // // 3. Set PWM2 duty cycle to 50% (0x40 → 0x80)
        // uint8_t reg = 0x40;
        // uint8_t reg = EMC2305_FAN_REG_ADDR(EMC2305_FAN2, EMC2305_REG_FAN1_SETTING);
        // uint8_t val = (50 * 255) / 100;
        // HAL_I2C_Master_Transmit(&hi2c1, 0x4D << 1, (uint8_t[]) { reg, val }, 2, 100);




        // Set PWM2 duty cycle to 25%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
            Error_Handler();
        };

        HAL_Delay(1000);

        // Set PWM2 duty cycle to 50%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 50) != EMC2305_OK) {
            Error_Handler();
        };

        HAL_Delay(1000);

        // Set PWM2 duty cycle to 75%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 75) != EMC2305_OK) {
            Error_Handler();
        };

        HAL_Delay(1000);

        // Set PWM2 duty cycle to 100%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 100) != EMC2305_OK) {
            Error_Handler();
        };

        HAL_Delay(1000);

        // Set PWM2 duty cycle to 0%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 0) != EMC2305_OK) {
            Error_Handler();
        };

        uint8_t data3[] = "Set PWM\r\n";
        msgLen = sizeof(data3) - 1;
        HAL_UART_Transmit(&huart1, data3, msgLen, 1000);

        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN);
        HAL_Delay(1000);
    }

    return 0;
}
