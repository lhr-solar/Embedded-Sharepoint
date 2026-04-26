#include "bootloader_config.h"
#include "bootloader_indicator.h"
#include "bootloader_runtime.h"
#include "bootloader_hal.h"
#include "uart_bootloader.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void SystemClock_Config(void);

#define BL_SOF ((uint8_t)0x5AU)
#define BL_ACK ((uint8_t)0x79U)
#define BL_NACK ((uint8_t)0x1FU)

/* AN3155 commands (subset needed for STM32CubeProgrammer UART flashing). */
#define BL_CMD_GET ((uint8_t)0x00U)
#define BL_CMD_GET_VERSION ((uint8_t)0x01U)
#define BL_CMD_GET_ID ((uint8_t)0x02U)
#define BL_CMD_READ_MEMORY ((uint8_t)0x11U)
#define BL_CMD_GO ((uint8_t)0x21U)
#define BL_CMD_WRITE_MEMORY ((uint8_t)0x31U)
#define BL_CMD_ERASE ((uint8_t)0x43U)
#define BL_CMD_EXT_ERASE ((uint8_t)0x44U)

#define BL_VERSION ((uint8_t)0x10U)
#define BL_DEVICE_ID ((uint16_t)0x0469U)

#define BL_FLASH_BASE ((uint32_t)0x08000000UL)
#define BL_FLASH_PAGE_SIZE ((uint32_t)0x800UL) /* 2KB pages on STM32G473 */
#define BL_OPTION_BYTES_BASE ((uint32_t)0x1FFF7800UL)
#define BL_OPTION_BYTES_SIZE ((uint32_t)0x80UL)

static const uint8_t s_supported_cmds[] = {
    BL_CMD_GET,
    BL_CMD_GET_VERSION,
    BL_CMD_GET_ID,
    BL_CMD_READ_MEMORY,
    BL_CMD_GO,
    BL_CMD_WRITE_MEMORY,
    BL_CMD_ERASE,
    BL_CMD_EXT_ERASE,
};

static bool s_app_write_seen = false;
static uint32_t s_last_command_tick = 0U;

static uint32_t bl_app_start(void) {
    return BOOTLOADER_APP_BASE;
}

static uint32_t bl_app_end_exclusive(void) {
    return BOOTLOADER_APP_BASE + BOOTLOADER_APP_MAX_SIZE;
}

static bool bl_addr_in_app(uint32_t addr) {
    return addr >= bl_app_start() && addr < bl_app_end_exclusive();
}

static bool bl_range_in_app(uint32_t addr, uint32_t len) {
    if (len == 0U) {
        return false;
    }
    uint32_t end = addr + len;
    if (end < addr) {
        return false;
    }
    return addr >= bl_app_start() && end <= bl_app_end_exclusive();
}

static bool bl_range_in_option_bytes(uint32_t addr, uint32_t len) {
    if (len == 0U) {
        return false;
    }
    uint32_t end = addr + len;
    if (end < addr) {
        return false;
    }
    return addr >= BL_OPTION_BYTES_BASE && end <= (BL_OPTION_BYTES_BASE + BL_OPTION_BYTES_SIZE);
}

static bool bl_xor_ok(const uint8_t *data, size_t len, uint8_t expected) {
    uint8_t crc = 0U;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
    }
    return crc == expected;
}

static void bl_send_ack(void) {
    uint8_t b = BL_ACK;
    (void)bootloader_runtime_send_bytes(&b, 1U);
}

static void bl_send_nack(void) {
    uint8_t b = BL_NACK;
    (void)bootloader_runtime_send_bytes(&b, 1U);
}

static bool bl_read_cmd(uint8_t *cmd) {
    uint8_t raw[2];
    if (!bootloader_runtime_read_bytes(raw, sizeof(raw), 1000U)) {
        return false;
    }
    if ((uint8_t)(raw[0] ^ raw[1]) != 0xFFU) {
        bl_send_nack();
        return false;
    }
    *cmd = raw[0];
    return true;
}

static bool bl_read_addr(uint32_t *addr) {
    uint8_t raw[5];
    if (!bootloader_runtime_read_bytes(raw, sizeof(raw), 1000U)) {
        return false;
    }
    if (!bl_xor_ok(raw, 4U, raw[4])) {
        bl_send_nack();
        return false;
    }
    *addr = ((uint32_t)raw[0] << 24) | ((uint32_t)raw[1] << 16) |
            ((uint32_t)raw[2] << 8) | ((uint32_t)raw[3]);
    return true;
}

static bool bl_handle_get(void) {
    bl_send_ack();
    uint8_t out[2 + sizeof(s_supported_cmds)];
    out[0] = (uint8_t)(sizeof(s_supported_cmds));
    out[1] = BL_VERSION;
    for (size_t i = 0; i < sizeof(s_supported_cmds); i++) {
        out[2 + i] = s_supported_cmds[i];
    }
    if (!bootloader_runtime_send_bytes(out, sizeof(out))) {
        return false;
    }
    bl_send_ack();
    return true;
}

static bool bl_handle_get_version(void) {
    bl_send_ack();
    uint8_t out[3] = {BL_VERSION, 0x00U, 0x00U};
    if (!bootloader_runtime_send_bytes(out, sizeof(out))) {
        return false;
    }
    bl_send_ack();
    return true;
}

static bool bl_handle_get_id(void) {
    bl_send_ack();
    uint8_t out[3] = {0x01U, (uint8_t)(BL_DEVICE_ID >> 8), (uint8_t)BL_DEVICE_ID};
    if (!bootloader_runtime_send_bytes(out, sizeof(out))) {
        return false;
    }
    bl_send_ack();
    return true;
}

static bool bl_handle_read_memory(void) {
    uint32_t addr = 0U;
    bl_send_ack();
    if (!bl_read_addr(&addr)) {
        return false;
    }
    bl_send_ack();

    uint8_t count_pkt[2];
    if (!bootloader_runtime_read_bytes(count_pkt, sizeof(count_pkt), 1000U)) {
        return false;
    }
    if ((uint8_t)(count_pkt[0] ^ count_pkt[1]) != 0xFFU) {
        bl_send_nack();
        return false;
    }

    uint32_t read_len = (uint32_t)count_pkt[0] + 1U;
    uint32_t end = addr + read_len;
    if (end < addr) {
        bl_send_nack();
        return false;
    }

    bl_send_ack();
    if (bl_range_in_app(addr, read_len)) {
        return bootloader_runtime_send_bytes((const uint8_t *)addr, (uint16_t)read_len);
    }
    if (bl_range_in_option_bytes(addr, read_len)) {
        return bootloader_runtime_send_bytes((const uint8_t *)addr, (uint16_t)read_len);
    }

    /*
     * Keep the session alive for other out-of-app probes while still denying
     * access to arbitrary bootloader memory.
     */
    uint8_t filler[256];
    for (uint32_t i = 0U; i < read_len; i++) {
        filler[i] = 0xFFU;
    }
    return bootloader_runtime_send_bytes(filler, (uint16_t)read_len);
}

static bool bl_handle_go(void) {
    uint32_t addr = 0U;
    bl_send_ack();
    if (!bl_read_addr(&addr)) {
        return false;
    }

    /*
     * CubeProgrammer may issue GO with probe addresses; acknowledge the command
     * to keep the session alive, and only actually jump when target is app base.
     */
    bl_send_ack();
    if (addr == bl_app_start() && bootloader_runtime_is_app_valid()) {
        HAL_Delay(20U);
        bootloader_runtime_jump_to_app();
    }
    return true;
}

static bool bl_handle_write_memory(void) {
    uint32_t addr = 0U;
    bl_send_ack();
    if (!bl_read_addr(&addr)) {
        return false;
    }
    if (!bl_addr_in_app(addr)) {
        bl_send_nack();
        return false;
    }
    bl_send_ack();

    uint8_t len_byte = 0U;
    if (!bootloader_runtime_read_bytes(&len_byte, 1U, 1000U)) {
        return false;
    }
    uint32_t data_len = (uint32_t)len_byte + 1U;
    uint8_t data[256];
    if (!bootloader_runtime_read_bytes(data, (uint16_t)data_len, 1000U)) {
        return false;
    }
    uint8_t checksum = 0U;
    if (!bootloader_runtime_read_bytes(&checksum, 1U, 1000U)) {
        return false;
    }

    uint8_t crc = len_byte;
    for (uint32_t i = 0; i < data_len; i++) {
        crc ^= data[i];
    }
    if (crc != checksum) {
        bl_send_nack();
        return false;
    }
    if (!bl_range_in_app(addr, data_len)) {
        bl_send_nack();
        return false;
    }

    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
    bootloader_indicator_update(HAL_GetTick());
    bool ok = bootloader_runtime_write_app(addr - bl_app_start(), data, data_len);
    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
    if (!ok) {
        bl_send_nack();
        return false;
    }
    s_app_write_seen = true;
    bl_send_ack();
    return true;
}

static bool bl_handle_erase(void) {
    bl_send_ack();
    uint8_t n = 0U;
    if (!bootloader_runtime_read_bytes(&n, 1U, 1000U)) {
        return false;
    }

    /* Only support global erase for app region. */
    if (n == 0xFFU) {
        uint8_t chk = 0U;
        if (!bootloader_runtime_read_bytes(&chk, 1U, 1000U)) {
            return false;
        }
        if ((uint8_t)(n ^ chk) != 0x00U) {
            bl_send_nack();
            return false;
        }
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
        bootloader_indicator_update(HAL_GetTick());
        bool ok = bootloader_runtime_erase_app();
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
        if (!ok) {
            bl_send_nack();
            return false;
        }
        bl_send_ack();
        return true;
    }

    /* Read and validate page list then perform app erase. */
    uint16_t count = (uint16_t)n + 1U;
    uint8_t page_buf[256];
    if (count > sizeof(page_buf)) {
        bl_send_nack();
        return false;
    }
    if (!bootloader_runtime_read_bytes(page_buf, count, 1000U)) {
        return false;
    }
    uint8_t chk = 0U;
    if (!bootloader_runtime_read_bytes(&chk, 1U, 1000U)) {
        return false;
    }
    uint8_t crc = n;
    for (uint16_t i = 0; i < count; i++) {
        crc ^= page_buf[i];
    }
    if (crc != chk) {
        bl_send_nack();
        return false;
    }

    uint32_t app_first_page = (bl_app_start() - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    uint32_t app_last_page = (bl_app_end_exclusive() - 1U - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    for (uint16_t i = 0; i < count; i++) {
        uint32_t p = page_buf[i];
        if (p < app_first_page || p > app_last_page) {
            bl_send_nack();
            return false;
        }
    }

    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
    bootloader_indicator_update(HAL_GetTick());
    bool ok = bootloader_runtime_erase_app();
    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
    if (!ok) {
        bl_send_nack();
        return false;
    }
    bl_send_ack();
    return true;
}

static bool bl_handle_ext_erase(void) {
    bl_send_ack();
    uint8_t hdr[2];
    if (!bootloader_runtime_read_bytes(hdr, sizeof(hdr), 1000U)) {
        return false;
    }
    uint16_t n = ((uint16_t)hdr[0] << 8) | hdr[1];

    if ((n == 0xFFFFU) || (n == 0xFFFEU) || (n == 0xFFFDU)) {
        uint8_t chk = 0U;
        if (!bootloader_runtime_read_bytes(&chk, 1U, 1000U)) {
            return false;
        }
        uint8_t crc = hdr[0] ^ hdr[1];
        if (crc != chk) {
            bl_send_nack();
            return false;
        }
        /*
         * AN3155 defines 0xFFFF global erase, 0xFFFE bank 1 erase, and
         * 0xFFFD bank 2 erase. This bootloader constrains all special erase
         * requests to the app region so the resident bootloader survives.
         */
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
        bootloader_indicator_update(HAL_GetTick());
        bool ok = bootloader_runtime_erase_app();
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
        if (!ok) {
            bl_send_nack();
            return false;
        }
        bl_send_ack();
        return true;
    }

    uint32_t count = (uint32_t)n + 1U;
    uint32_t bytes = count * 2U;
    if (bytes > 512U) {
        bl_send_nack();
        return false;
    }

    uint8_t pages[512];
    if (!bootloader_runtime_read_bytes(pages, (uint16_t)bytes, 1000U)) {
        return false;
    }
    uint8_t chk = 0U;
    if (!bootloader_runtime_read_bytes(&chk, 1U, 1000U)) {
        return false;
    }
    uint8_t crc = hdr[0] ^ hdr[1];
    for (uint32_t i = 0U; i < bytes; i++) {
        crc ^= pages[i];
    }
    if (crc != chk) {
        bl_send_nack();
        return false;
    }

    uint32_t app_first_page = (bl_app_start() - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    uint32_t app_last_page = (bl_app_end_exclusive() - 1U - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    for (uint32_t i = 0U; i < count; i++) {
        uint16_t p = ((uint16_t)pages[2U * i] << 8) | pages[(2U * i) + 1U];
        if (p < app_first_page || p > app_last_page) {
            bl_send_nack();
            return false;
        }
    }

    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
    bootloader_indicator_update(HAL_GetTick());
    bool ok = bootloader_runtime_erase_app();
    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
    if (!ok) {
        bl_send_nack();
        return false;
    }
    bl_send_ack();
    return true;
}

static bool bl_dispatch_cmd(uint8_t cmd) {
    if (cmd == BL_CMD_GET) {
        return bl_handle_get();
    }
    if (cmd == BL_CMD_GET_VERSION) {
        return bl_handle_get_version();
    }
    if (cmd == BL_CMD_GET_ID) {
        return bl_handle_get_id();
    }
    if (cmd == BL_CMD_READ_MEMORY) {
        return bl_handle_read_memory();
    }
    if (cmd == BL_CMD_GO) {
        return bl_handle_go();
    }
    if (cmd == BL_CMD_WRITE_MEMORY) {
        return bl_handle_write_memory();
    }
    if (cmd == BL_CMD_ERASE) {
        return bl_handle_erase();
    }
    if (cmd == BL_CMD_EXT_ERASE) {
        return bl_handle_ext_erase();
    }

    bl_send_nack();
    return false;
}

static bool bl_wait_for_sync_with_indicator(uint32_t timeout_ms) {
    uint32_t start = HAL_GetTick();
    while ((timeout_ms == 0U) || ((HAL_GetTick() - start) < timeout_ms)) {
        bootloader_indicator_update(HAL_GetTick());

        if (bootloader_runtime_poll_sync(5U)) {
            bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
            bl_send_ack();
            return true;
        }
    }
    return false;
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    bootloader_indicator_init();
    bootloader_runtime_init();

    bool forced_bootloader = uart_bootloader_consume_request();
    bool app_valid = bootloader_runtime_is_app_valid();
    bootloader_indicator_set_mode(app_valid ? BOOTLOADER_INDICATOR_APP_PRESENT : BOOTLOADER_INDICATOR_NO_APP);

    uint32_t startup_wait_ms = (app_valid && !forced_bootloader) ?
        BOOTLOADER_APP_STARTUP_WAIT_MS : BOOTLOADER_HANDSHAKE_TIMEOUT_MS;
    if (!bl_wait_for_sync_with_indicator(startup_wait_ms)) {
        if (app_valid) {
            bootloader_runtime_jump_to_app();
        }
    }

    while (1) {
        bootloader_indicator_update(HAL_GetTick());
        uint8_t cmd = 0U;
        if (!bl_read_cmd(&cmd)) {
            if (s_app_write_seen && bootloader_runtime_is_app_valid() &&
                ((HAL_GetTick() - s_last_command_tick) >= BOOTLOADER_POST_FLASH_BOOT_DELAY_MS)) {
                bootloader_runtime_jump_to_app();
            }
            continue;
        }
        s_last_command_tick = HAL_GetTick();
        (void)bl_dispatch_cmd(cmd);
    }
}
