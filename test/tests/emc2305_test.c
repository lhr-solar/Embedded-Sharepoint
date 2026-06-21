// Test for the EMC2305 Fan Chip Driver
// Spawns 2 tasks to test thread safe control of underlying I2C bus

#include "stm32xx_hal.h"
#include "emc2305.h"
#include "uart.h"
#include "printf.h"

// From datasheet
#define DEFAULT_DEV_ADDR 0x4D

// LED pins for PSOM
#define STATUS_LED_PORT GPIOA
#define STATUS_LED_PIN_1 GPIO_PIN_7
#define STATUS_LED_PIN_2 GPIO_PIN_8
#define STATUS_LED_PIN_3 GPIO_PIN_15

// USART pins for PSOM
#define USART_PORT GPIOA
#define USART_TX_PIN GPIO_PIN_9
#define USART_RX_PIN GPIO_PIN_10

// I2C pins for PSOM
#define I2C_PORT GPIOB
#define I2C_SCL_PIN GPIO_PIN_6
#define I2C_SDA_PIN GPIO_PIN_7

I2C_HandleTypeDef hi2c1;
EMC2305_HandleTypeDef chip;

StaticTask_t initTaskBuffer;
StackType_t initTaskStack[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_1;
StackType_t emc2305TaskStack_1[configMINIMAL_STACK_SIZE];
StaticTask_t emc2305TaskBuffer_2;
StackType_t emc2305TaskStack_2[configMINIMAL_STACK_SIZE];

void mx_uart_init(void) {
}

void mx_i2c_init(void) {
}

void mx_led_init(void) {
}

// Initialize UART and EMC2305
void Init_Task(void* argument) {
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
    // Only call from ONE task!
    if (EMC2305_Init(&chip, &hi2c1, DEFAULT_DEV_ADDR) != EMC2305_OK) {
        Error_Handler();
    }

    // Task kills itself
    vTaskDelete(NULL);
}

void EMC2305_Task_1(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

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
    config1.enable_closed_loop = false; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
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

    // Set PWM output mode to open-drain
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN2, true) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 1: Fan 2 output set to open-drain\r\n");

    while (1) {
        // Testing Direct PWM Drive Mode
        // Set PWM2 duty cycle to 25%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 25) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 1: PWM drive set to 25%%\r\n");

        // Testing FSC Mode
        // Set RPM to 3000
        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // printf("Task 1: RPM target set to 3000\r\n");

        // Get current rpm
        // uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
        // printf("Measured RPM: %u\r\n", rpm);

        // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink Heartbeat LED
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);

        // vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void EMC2305_Task_2(void* argument) {
    // Allow chip to power on
    vTaskDelay(pdMS_TO_TICKS(250));

    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_2);

    // Set global config
    EMC2305_Global_Config config = { 0 };
    config.watchdog_enable = true;
    if (EMC2305_SetGlobalConfig(&chip, &config) != EMC2305_OK) {
        Error_Handler();
    }

    printf("Task 2: Global Config Set\r\n");

    // Set config1 and config2
    EMC2305_Fan_Config1 config1 = { 0 };
    config1.enable_closed_loop = false; // Set this to true if using FSC (Closed Loop RPM Control). False for using PWM directly
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

    // Set PWM output mode to open-drain
    if (EMC2305_SetPWMOutputMode(&chip, EMC2305_FAN2, true) != EMC2305_OK) {
        Error_Handler();
    };

    printf("Task 2: Fan 2 output set to open-drain\r\n");

    while (1) {
        // Testing Direct PWM Drive Mode
        // Set PWM2 duty cycle to 100%
        if (EMC2305_SetFanPWM(&chip, EMC2305_FAN2, 100) != EMC2305_OK) {
            Error_Handler();
        };
        printf("Task 2: PWM drive set to 100%%\r\n");

        // Testing FSC mode
        // Set RPM to 3000
        // if (EMC2305_SetFanRPM(&chip, EMC2305_FAN2, 3000) != EMC2305_OK) {
        //     Error_Handler();
        // };
        // printf("Task 2: RPM target set to 3000\r\n");

        // Get current rpm
        // uint16_t rpm = EMC2305_GetFanRPM(&chip, EMC2305_FAN2);
        // printf("Measured RPM: %u\r\n", rpm);

        // Get current pwm
        // uint8_t pwm = EMC2305_GetFanPWM(&chip, EMC2305_FAN2);
        // printf("Drive PWM: %u\r\n", pwm);

        // Blink Heartbeat LED
        HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_1);

        // vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    HAL_GPIO_TogglePin(STATUS_LED_PORT, STATUS_LED_PIN_1);
    printf("Task 2: EMC2305 Initialized\r\n");

    xTaskCreateStatic(Init_Task,
        "Init Task",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 1,
        initTaskStack,
        &initTaskBuffer);

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
        tskIDLE_PRIORITY + 5,
        emc2305TaskStack_2,
        &emc2305TaskBuffer_2);

    vTaskStartScheduler();

    while (1) {
    }

    return 0;
}

// I2C Transmit Interrupt Callback
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterTxCpltCallback(hi2c);
}

// I2C Receive Interrupt Callback
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
    EMC2305_I2C_MasterRxCpltCallback(hi2c);
}

// I2C Error Interrupt Callback
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef* hi2c) {
    // TODO: properly recover bus....
}
