/* UART test

- Sets up UART4 
- Creates a receive queue
- Sends multiple bytes (some will go in TX queue)
- Receives bytes and verifies correctness
- Tests blocking and non-blocking behavior
- Flashes LED if successful
*/

#include "stm32xx_hal.h"
#include "UART.h"

// Create static task for FreeRTOS
StaticTask_t task_buffer;
StackType_t taskStack[configMINIMAL_STACK_SIZE];

// Create static RX queue
#define RX_QUEUE_SIZE 32
static StaticQueue_t rx_queue_buffer;
static uint8_t rx_queue_storage[RX_QUEUE_SIZE];
static QueueHandle_t rx_queue;

static void error_handler(void) {
    while(1) {}
}

static void success_handler(void) {
    GPIO_InitTypeDef led_init = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_5
    };
    
    __HAL_RCC_GPIOA_CLK_ENABLE();
    HAL_GPIO_Init(GPIOA, &led_init);

    while(1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5); // Toggle LED 
        HAL_Delay(500);
    }
}

static void task(void *pvParameters) {
    // Create test data
    const uint8_t tx_data[] = "Hello UART Test!";
    const uint8_t tx_len = sizeof(tx_data) - 1;  // Don't include null terminator
    uint8_t rx_data[32] = {0};
    
    // Initialize RX queue
    rx_queue = xQueueCreateStatic(RX_QUEUE_SIZE,
                                 sizeof(uint8_t),
                                 rx_queue_storage,
                                 &rx_queue_buffer);
                                 
    // Setup UART init
    huart4->Init.BaudRate = 115200;
    huart4->Init.WordLength = UART_WORDLENGTH_8B;
    huart4->Init.StopBits = UART_STOPBITS_1;
    huart4->Init.Parity = UART_PARITY_NONE;
    huart4->Init.Mode = UART_MODE_TX_RX;
    huart4->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4->Init.OverSampling = UART_OVERSAMPLING_16;
    
    // Initialize UART with RX queue
    if (uart_init(huart4, &rx_queue) != UART_OK) error_handler();

    // Test 1: Blocking send
    if (uart_send(huart4, tx_data, tx_len, true) != UART_SENT) error_handler();
    
    // Test 2: Blocking receive what we sent
    if (uart_recv(huart4, rx_data, tx_len, true) != UART_RECV) error_handler();
    
    // Verify received data
    for (int i = 0; i < tx_len; i++) {
        if (rx_data[i] != tx_data[i]) error_handler();
    }
    
    // Test 3: Non-blocking receive when empty
    uint8_t single_byte;
    if (uart_recv(huart4, &single_byte, 1, false) != UART_EMPTY) error_handler();
    
    // Test 4: Multiple rapid sends to test queueing
    const uint8_t rapid_data[] = "12345";
    const uint8_t rapid_len = sizeof(rapid_data) - 1;
    
    for (int i = 0; i < 10; i++) {  // Send enough to fill TX queue
        if (uart_send(huart4, rapid_data, rapid_len, true) != UART_SENT) error_handler();
    }
    
    // Test 5: Blocking receive multiple queued messages
    memset(rx_data, 0, sizeof(rx_data));
    for (int i = 0; i < 10; i++) {
        if (uart_recv(huart4, rx_data, rapid_len, true) != UART_RECV) error_handler();
        for (int j = 0; j < rapid_len; j++) {
            if (rx_data[j] != rapid_data[j]) error_handler();
        }
    }

    success_handler();
}

int main(void) {
    // Initialize HAL
    if (HAL_Init() != HAL_OK) error_handler();
    // SystemClock_Config();  // Uncomment if needed
    
    // Create RTOS task
    xTaskCreateStatic(task,
                     "uart_test",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     tskIDLE_PRIORITY + 2,
                     taskStack,
                     &task_buffer);
    
    vTaskStartScheduler();
    
    error_handler();
    return 0;
}