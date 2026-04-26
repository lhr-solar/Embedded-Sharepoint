#include "bootloader_config.h"
#include "bootloader_indicator.h"
#include "bootloader_runtime.h"
#include "bootloader_hal.h"
#include "bootloader_command.h"

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

static const uint8_t s_can_supported_cmds[] = {
    BL_CMD_GET,
    BL_CMD_GET_VERSION,
    BL_CMD_GET_ID,
    BL_CMD_READ_MEMORY,
    BL_CMD_GO,
    BL_CMD_WRITE_MEMORY,
    BL_CMD_ERASE,
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

static void bl_can_send_ack(uint32_t id) {
    uint8_t b = BL_ACK;
    (void)bootloader_runtime_can_send(id, &b, 1U);
}

static void bl_can_send_nack(uint32_t id) {
    uint8_t b = BL_NACK;
    (void)bootloader_runtime_can_send(id, &b, 1U);
}

static bool bl_can_send_data(uint32_t id, const uint8_t *data, uint8_t len) {
    return bootloader_runtime_can_send(id, data, len);
}

static uint32_t bl_addr_from_can_data(const uint8_t *data) {
    return ((uint32_t)data[0] << 24) | ((uint32_t)data[1] << 16) |
           ((uint32_t)data[2] << 8) | ((uint32_t)data[3]);
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

static void bl_can_handle_get(void) {
    bl_can_send_ack(BL_CMD_GET);

    uint8_t count = (uint8_t)sizeof(s_can_supported_cmds);
    (void)bl_can_send_data(BL_CMD_GET, &count, 1U);

    uint8_t version = BL_VERSION;
    (void)bl_can_send_data(BL_CMD_GET, &version, 1U);

    for (size_t i = 0U; i < sizeof(s_can_supported_cmds); i++) {
        (void)bl_can_send_data(BL_CMD_GET, &s_can_supported_cmds[i], 1U);
    }

    bl_can_send_ack(BL_CMD_GET);
}

static void bl_can_handle_get_version(void) {
    bl_can_send_ack(BL_CMD_GET_VERSION);

    uint8_t version = BL_VERSION;
    (void)bl_can_send_data(BL_CMD_GET_VERSION, &version, 1U);

    const uint8_t dummy[2] = {0U, 0U};
    (void)bl_can_send_data(BL_CMD_GET_VERSION, dummy, sizeof(dummy));

    bl_can_send_ack(BL_CMD_GET_VERSION);
}

static void bl_can_handle_get_id(void) {
    bl_can_send_ack(BL_CMD_GET_ID);

    uint8_t out[2] = {(uint8_t)(BL_DEVICE_ID >> 8), (uint8_t)BL_DEVICE_ID};
    (void)bl_can_send_data(BL_CMD_GET_ID, out, sizeof(out));

    bl_can_send_ack(BL_CMD_GET_ID);
}

static void bl_can_send_memory(uint32_t id, uint32_t addr, uint32_t read_len) {
    uint8_t filler[8];
    uint32_t sent = 0U;

    while (sent < read_len) {
        uint8_t chunk = (uint8_t)(((read_len - sent) > 8U) ? 8U : (read_len - sent));
        const uint8_t *src = NULL;

        if (bl_range_in_app(addr + sent, chunk) || bl_range_in_option_bytes(addr + sent, chunk)) {
            src = (const uint8_t *)(addr + sent);
        } else {
            for (uint8_t i = 0U; i < chunk; i++) {
                filler[i] = 0xFFU;
            }
            src = filler;
        }

        (void)bl_can_send_data(id, src, chunk);
        sent += chunk;
    }
}

static void bl_can_handle_read_memory(const bootloader_can_frame_t *frame) {
    if (frame->dlc != 5U) {
        bl_can_send_nack(BL_CMD_READ_MEMORY);
        return;
    }

    uint32_t addr = bl_addr_from_can_data(frame->data);
    uint32_t read_len = (uint32_t)frame->data[4] + 1U;
    uint32_t end = addr + read_len;
    if (end < addr) {
        bl_can_send_nack(BL_CMD_READ_MEMORY);
        return;
    }

    bl_can_send_ack(BL_CMD_READ_MEMORY);
    bl_can_send_memory(BL_CMD_READ_MEMORY, addr, read_len);
    bl_can_send_ack(BL_CMD_READ_MEMORY);
}

static void bl_can_handle_go(const bootloader_can_frame_t *frame) {
    if (frame->dlc != 4U) {
        bl_can_send_nack(BL_CMD_GO);
        return;
    }

    uint32_t addr = bl_addr_from_can_data(frame->data);
    if (addr != bl_app_start() || !bootloader_runtime_is_app_valid()) {
        bl_can_send_nack(BL_CMD_GO);
        return;
    }

    bl_can_send_ack(BL_CMD_GO);
    HAL_Delay(20U);
    bootloader_runtime_jump_to_app();
}

static void bl_can_handle_write_memory(const bootloader_can_frame_t *frame) {
    if (frame->dlc != 5U) {
        bl_can_send_nack(BL_CMD_WRITE_MEMORY);
        return;
    }

    uint32_t addr = bl_addr_from_can_data(frame->data);
    uint32_t data_len = (uint32_t)frame->data[4] + 1U;
    if (!bl_range_in_app(addr, data_len)) {
        bl_can_send_nack(BL_CMD_WRITE_MEMORY);
        return;
    }

    bl_can_send_ack(BL_CMD_WRITE_MEMORY);

    uint8_t data[256];
    uint32_t received = 0U;
    while (received < data_len) {
        bootloader_can_frame_t data_frame = {0};
        if (!bootloader_runtime_can_recv(&data_frame, 1000U) || data_frame.dlc == 0U ||
            data_frame.id > 0xFFU || ((received + data_frame.dlc) > data_len)) {
            bl_can_send_nack(BL_CMD_WRITE_MEMORY);
            return;
        }

        for (uint8_t i = 0U; i < data_frame.dlc; i++) {
            data[received + i] = data_frame.data[i];
        }
        received += data_frame.dlc;

        if (received == data_len) {
            bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
            bootloader_indicator_update(HAL_GetTick());
            bool ok = bootloader_runtime_write_app(addr - bl_app_start(), data, data_len);
            bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
            if (!ok) {
                bl_can_send_nack(BL_CMD_WRITE_MEMORY);
                return;
            }
            s_app_write_seen = true;
        }

        bl_can_send_ack(BL_CMD_WRITE_MEMORY);
    }
}

static bool bl_can_page_in_app(uint32_t page) {
    uint32_t app_first_page = (bl_app_start() - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    uint32_t app_last_page = (bl_app_end_exclusive() - 1U - BL_FLASH_BASE) / BL_FLASH_PAGE_SIZE;
    return page >= app_first_page && page <= app_last_page;
}

static void bl_can_handle_erase(const bootloader_can_frame_t *frame) {
    if (frame->dlc == 0U) {
        bl_can_send_nack(BL_CMD_ERASE);
        return;
    }

    if (frame->dlc == 1U && frame->data[0] == 0xFFU) {
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
        bootloader_indicator_update(HAL_GetTick());
        bool ok = bootloader_runtime_erase_app();
        bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
        if (ok) {
            bl_can_send_ack(BL_CMD_ERASE);
        } else {
            bl_can_send_nack(BL_CMD_ERASE);
        }
        return;
    }

    for (uint8_t i = 0U; i < frame->dlc; i++) {
        if (!bl_can_page_in_app(frame->data[i])) {
            bl_can_send_nack(BL_CMD_ERASE);
            return;
        }
    }

    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_FLASHING);
    bootloader_indicator_update(HAL_GetTick());
    bool ok = bootloader_runtime_erase_app();
    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
    if (ok) {
        bl_can_send_ack(BL_CMD_ERASE);
    } else {
        bl_can_send_nack(BL_CMD_ERASE);
    }
}

static void bl_can_dispatch_frame(const bootloader_can_frame_t *frame) {
    uint8_t cmd = (uint8_t)(frame->id & 0xFFU);

    if (frame->id > 0xFFU) {
        bl_can_send_nack(frame->id);
        return;
    }

    if (cmd == BL_CMD_GET) {
        bl_can_handle_get();
    } else if (cmd == BL_CMD_GET_VERSION) {
        bl_can_handle_get_version();
    } else if (cmd == BL_CMD_GET_ID) {
        bl_can_handle_get_id();
    } else if (cmd == BL_CMD_READ_MEMORY) {
        bl_can_handle_read_memory(frame);
    } else if (cmd == BL_CMD_GO) {
        bl_can_handle_go(frame);
    } else if (cmd == BL_CMD_WRITE_MEMORY) {
        bl_can_handle_write_memory(frame);
    } else if (cmd == BL_CMD_ERASE) {
        bl_can_handle_erase(frame);
    } else {
        bl_can_send_nack(frame->id);
    }
}

static bool bl_wait_for_sync_with_indicator(uint32_t timeout_ms) {
    uint32_t start = HAL_GetTick();
    while ((timeout_ms == 0U) || ((HAL_GetTick() - start) < timeout_ms)) {
        bootloader_indicator_update(HAL_GetTick());

        if (bootloader_runtime_poll_sync(5U)) {
            bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);
            if (bootloader_runtime_active_transport() == BOOTLOADER_TRANSPORT_CAN) {
                bl_can_send_ack(BOOTLOADER_CAN_SYNC_ID);
            } else {
                bl_send_ack();
            }
            return true;
        }
    }
    return false;
}

static void bl_reset_to_normal_boot(void) {
    bootloader_command_clear_request();
    __DSB();
    NVIC_SystemReset();
}

static void bl_hold_without_flashing(void) {
    bootloader_indicator_set_mode(BOOTLOADER_INDICATOR_CONNECTED);

    while (1) {
        bootloader_indicator_update(HAL_GetTick());

        bootloader_can_frame_t frame = {0};
        if (bootloader_runtime_can_recv(&frame, 100U) &&
            bootloader_command_is_can_reset_message(frame.id, frame.data, frame.dlc)) {
            bl_reset_to_normal_boot();
        }
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();

    bootloader_indicator_init();
    bootloader_runtime_init();

    bootloader_command_request_t boot_request = bootloader_command_consume_request();
    bool forced_bootloader = boot_request != BOOTLOADER_COMMAND_REQUEST_NONE;
    bool app_valid = bootloader_runtime_is_app_valid();
    bootloader_indicator_set_mode(app_valid ? BOOTLOADER_INDICATOR_APP_PRESENT : BOOTLOADER_INDICATOR_NO_APP);

    if (boot_request == BOOTLOADER_COMMAND_REQUEST_HOLD) {
        bl_hold_without_flashing();
    }

    uint32_t startup_wait_ms = (app_valid && !forced_bootloader) ?
        BOOTLOADER_APP_STARTUP_WAIT_MS : BOOTLOADER_HANDSHAKE_TIMEOUT_MS;
    if (!bl_wait_for_sync_with_indicator(startup_wait_ms)) {
        if (app_valid) {
            bootloader_runtime_jump_to_app();
        }
    }

    while (1) {
        bootloader_indicator_update(HAL_GetTick());

        if (bootloader_runtime_active_transport() == BOOTLOADER_TRANSPORT_CAN) {
            bootloader_can_frame_t frame = {0};
            if (!bootloader_runtime_can_recv(&frame, 1000U)) {
                if (s_app_write_seen && bootloader_runtime_is_app_valid() &&
                    ((HAL_GetTick() - s_last_command_tick) >= BOOTLOADER_POST_FLASH_BOOT_DELAY_MS)) {
                    bootloader_runtime_jump_to_app();
                }
                continue;
            }

            s_last_command_tick = HAL_GetTick();
            bl_can_dispatch_frame(&frame);
            continue;
        }

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
