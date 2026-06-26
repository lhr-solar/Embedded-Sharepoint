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

/* Upper bound on how long we wait for the ACK to drain before resetting. Way
 * more than "BOOTACK\r\n" needs even at slow baud (9 bytes @ 1200 baud ~75ms). */
#ifndef BOOTLOADER_ACK_DRAIN_MS
#define BOOTLOADER_ACK_DRAIN_MS 500U
#endif

static UART_HandleTypeDef *s_uart;

static void Task_Bootloader(void *pvParameters) {
    (void)pvParameters;

    uint8_t byte = 0U;
    for (;;) {
        if (uart_recv(s_uart, &byte, 1U, portMAX_DELAY) != UART_OK) {
            /* UART not initialised yet (or a transient error): don't busy-spin. */
            vTaskDelay(pdMS_TO_TICKS(10));
            continue;
        }
        if (bootloader_lite_feed_byte(byte)) {
            /* Acknowledge so the host knows we heard it... */
            if (uart_send(s_uart, (const uint8_t *)BOOTLOADER_LITE_ACK,
                          (uint16_t)(sizeof(BOOTLOADER_LITE_ACK) - 1U),
                          pdMS_TO_TICKS(200)) == UART_OK) {
                /* ...wait for the ACK to actually leave the wire before we reset
                 * (the reboot kills the UART). First let the driver's queued/IT
                 * transmit finish, then wait for the shift register (TC flag) --
                 * no blind delay that a low baud / busy system could outrun. The
                 * deadline keeps a chatty console from wedging us here. */
                const TickType_t start = xTaskGetTickCount();
                const TickType_t timeout = pdMS_TO_TICKS(BOOTLOADER_ACK_DRAIN_MS);
                while (s_uart->gState == HAL_UART_STATE_BUSY_TX &&
                       (xTaskGetTickCount() - start) < timeout) {
                    vTaskDelay(1);
                }
                while (!__HAL_UART_GET_FLAG(s_uart, UART_FLAG_TC) &&
                       (xTaskGetTickCount() - start) < timeout) {
                    /* last byte still shifting out; microseconds, then reset */
                }
            }
            bootloader_lite_reboot_to_rom();
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
