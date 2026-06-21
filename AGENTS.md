# Agent guide (canonical)

STM32 G4 firmware library and build system shared as a submodule/superproject asset.

## Project

- **What:** Embedded Sharepoint — reusable STM32 firmware library, drivers, BSP, and test harness.
- **Targets:** `stm32g491vet` (100-pin) and `stm32g431cbt` (48-pin).
- **Bootloader:** Resident CAN/DFU bootloader contract and tooling under `bootloader/`.

## Build & test

All builds run inside the Nix dev shell:

```bash
nix develop
cd test && make TEST=<name> PROJECT_TARGET=<port>
```

- **Engine:** `sharepoint.mk` (invoked from `test/Makefile`).
- **Run all tests:** `cd test && python3 makeAllTests.py`
- **Ports:** `PROJECT_TARGET=stm32g491vet` or `stm32g431cbt`

## Conventions

- **Lean / minimal (ponytail):** YAGNI, stdlib/platform first, smallest diff that works, deletion over addition. No unrequested abstractions or dependencies.
- **Surgical changes (karpathy):** Touch only what the task requires; match existing style; define verifiable success criteria and check each step.
- **Git:** Never `git push` or `git commit` unless the user explicitly asks in the current request.

## Agent assets

Full rules: [`.cursor/rules/`](.cursor/rules/)  
Skills: [`.cursor/skills/`](.cursor/skills/)

Source copies also live under `.agent/` (do not delete). Sync to parent superprojects via `scripts/sync/sharepoint-sync.sh`.

## Knowledge graph (optional)

Code-only graphify scaffolding: `graphify.yaml`, `scripts/graphify/run-graphify.sh`. Output goes to `.graphify/` (gitignored). Graphify is installed in a later phase; the wrapper exits cleanly if the CLI is missing.
