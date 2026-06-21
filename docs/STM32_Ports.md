# List of STM32 Port Names

Embedded Sharepoint supports two STM32G4 ports. Set `PROJECT_TARGET` to the lowercase port name below.

## G4

- `STM32G491VETx` (100-pin)
    - `PROJECT_TARGET=stm32g491vet`
- `STM32G431CBTx` (48-pin)
    - `PROJECT_TARGET=stm32g431cbt`

## How the build resolves port files

Given `PROJECT_TARGET=stm32g491vet`, `sharepoint.mk` parses the target string:

- Series: `stm32g4xx` (characters 6–7 → `g4`)
- Line: `stm32g491` (characters 8–9 → `91`)
- Port directory: `stm/stm32g4xx/stm32g491/`
- Linker script: `STM32G491VETx_FLASH.ld`
- Startup: `startup_stm32g491xx.s`
- BSP config: `stm32g491vet.cfg`

The same pattern applies to `stm32g431cbt` under `stm/stm32g4xx/stm32g431/`.
