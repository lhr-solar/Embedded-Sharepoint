// TinyUSB Dual Port CDC Example

#include "stm32xx_hal.h"
#include "tusb.h"

#include <ctype.h>

static void MX_USB_PCD_Init(void);
static void MX_GPIO_Init(void);

PCD_HandleTypeDef hpcd_USB_FS;

static void echo_serial_port(uint8_t itf, uint8_t buf[], uint32_t count) {
  uint8_t const case_diff = 'a' - 'A';

  for (uint32_t i = 0; i < count; i++) {
    if (itf == 0) {
      // echo back 1st port as lower case
      if (isupper(buf[i])) buf[i] += case_diff;
    } else {
      // echo back 2nd port as upper case
      if (islower(buf[i])) buf[i] -= case_diff;
    }
    tud_cdc_n_write_char(itf, buf[i]);
  }

  tud_cdc_n_write_flush(itf);
}

// Invoked when device is mounted
void tud_mount_cb(void) {
  //Do nothing for now
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
}

// Invoked when device is unmounted
void tud_umount_cb(void) {
  //Do nothing for now
  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
}

static void cdc_task(void) {
  uint8_t itf;

  for (itf = 0; itf < CFG_TUD_CDC; itf++) {
    // connected() check for DTR bit
    // Most but not all terminal client set this when making connection
    // if ( tud_cdc_n_connected(itf) )
    {
      if (tud_cdc_n_available(itf)) {
        uint8_t buf[64];

        uint32_t count = tud_cdc_n_read(itf, buf, sizeof(buf));

        // echo back to both serial ports
        echo_serial_port(0, buf, count);
        echo_serial_port(1, buf, count);
      }
    }
  }
}

int main() {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_USB_PCD_Init();
    
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);  

    tusb_rhport_init_t dev_init = {
        .role = TUSB_ROLE_DEVICE,
        .speed = TUSB_SPEED_AUTO
    };
    tusb_init(BOARD_TUD_RHPORT, &dev_init);

    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);

    /* Random msg */
    while (1) {
        tud_task();
        cdc_task();

        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
        HAL_Delay(500);
    }

}

static void MX_USB_PCD_Init(void)
{
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void) {
    /* enable port A for usb pins */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // heartbeat
    GPIO_InitTypeDef led_config = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_NOPULL,
        .Pin = GPIO_PIN_0
    };
    HAL_GPIO_Init(GPIOA, &led_config);
}

void USBWakeUp_IRQHandler(void)
{
  tud_int_handler(0);
}

void Error_Handler(void)
{
  while (1)
  {
    // chud about
  }
}