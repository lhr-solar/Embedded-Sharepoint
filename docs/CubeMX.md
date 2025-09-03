# CubeMX
STM32 CubeMX is a graphical tool made by STMicro to help in creating STM32 hardware and software. While the generated code is not always optimal, CubeMX is extremely useful for figuring out pin alternate functions, setting up clock trees, and quickly enabling peripherals.

## 1. Installing CubeMX
Download CubeMX [here](https://www.st.com/en/development-tools/stm32cubemx.html).

## 2. Using the CubeMX Microcontroller Editor
1. Open CubeMX.  
2. Press **"Access to MCU Selector"**.  
3. Search for your mcu part number (stm32f413rht)
4. 

## 3. Editing Peripheral Settings
CubeMX has two main ways to enable and configure peripherals:

### 3.1 Peripheral Selector (Pinout & Configuration Tab)
- On the left-hand side, expand the **Peripherals** list.  
- Check the box of the peripheral you want to enable (e.g., **USART1**, **I2C1**, **TIM2**, etc.).  
- CubeMX will automatically assign the required pins on the microcontroller diagram.  
- If there are conflicts (e.g., two peripherals trying to use the same pin), CubeMX will show a warning and let you reassign pins manually.  
- Hover over the microcontroller diagram and click on a pin to change its function or remap it.

### 3.2 Peripheral Configuration
- After enabling a peripheral, click on its name in the **Configuration Pane** (bottom or side panel).  
- This opens a settings window for that peripheral.  
- Examples of configurations:
  - **USART:** baud rate, word length, stop bits, parity, DMA options.  
  - **Timers:** PWM mode, clock source, prescaler values, channel mappings.  
  - **ADC:** resolution, sampling time, number of channels, trigger source.  
- You can also configure **NVIC interrupts** and enable/disable them from this window.

### 3.3 Clock Configuration
- Go to the **Clock Configuration** tab.  
- Set up the input oscillator (HSI, HSE, LSE) and PLL settings.  
- CubeMX will calculate derived clock speeds for the AHB, APB buses, and peripherals.  
- If you exceed limits, CubeMX will flag the error in red.

## 4. Generating Code
1. Press the **Project Manager** tab.  
2. Choose your project name, location, and toolchain (e.g., STM32CubeIDE, Keil, IAR, or Makefile).  
3. Under **Code Generator**, you can decide:
   - **HAL only** (default)
   - **Low-level drivers** (LL)
   - **Empty project** with just initialization.  
4. Click **Generate Code**.  
5. Open the generated project in your chosen IDE and start writing your application code.

---
