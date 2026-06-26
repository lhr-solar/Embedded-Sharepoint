/**
 * @file BootloaderTask.c
 * @brief Drop-in FreeRTOS UART listener for bootloader_lite (see header).
 * @copyright Copyright (c) 2026 UT Longhorn Racing Solar
 */

#include "BootloaderTask.h"
#include "bootloader_lite.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdint.h>

#ifndef BOOTLOADER_TASK_STACK_SIZE
#define BOOTLOADER_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE * 2U)
#endif

#ifndef BOOTLOADER_TASK_PRIORITY
#define BOOTLOADER_TASK_PRIORITY (tskIDLE_PRIORITY + 1U)
#endif

static UART_HandleTypeDef *s_uart;

static void Task_Bootloader(void *pvParameters) {
    (void)pvParameters;

    uint8_t byte = 0U;
    for (;;) {
        if (uart_recv(s_uart, &byte, 1U, portMAX_DELAY) != UART_OK) {
            continue;
        }
        if (bootloader_lite_feed_byte(byte)) {
            /* Acknowledge so the host knows we heard it... */
            (void)uart_send(s_uart, (const uint8_t *)BOOTLOADER_LITE_ACK,
                            (uint16_t)(sizeof(BOOTLOADER_LITE_ACK) - 1U), pdMS_TO_TICKS(200));
            /* ...let the ACK drain, then jump into the ROM bootloader. */
            vTaskDelay(pdMS_TO_TICKS(100));
            bootloader_lite_enter_rom();
        }
    }
}

#if (configSUPPORT_STATIC_ALLOCATION == 1)
static StaticTask_t s_bootloader_task_buf;
static StackType_t s_bootloader_task_stack[BOOTLOADER_TASK_STACK_SIZE];
#endif

void BootloaderTask_Init(UART_HandleTypeDef *uart) {
    s_uart = uart;
#if (configSUPPORT_STATIC_ALLOCATION == 1)
    (void)xTaskCreateStatic(
        Task_Bootloader, "Bootloader", BOOTLOADER_TASK_STACK_SIZE,
        NULL, BOOTLOADER_TASK_PRIORITY, s_bootloader_task_stack, &s_bootloader_task_buf);
#else
    (void)xTaskCreate(
        Task_Bootloader, "Bootloader", BOOTLOADER_TASK_STACK_SIZE,
        NULL, BOOTLOADER_TASK_PRIORITY, NULL);
#endif
}
