# TinyUSB Multi-Agent Dev/Test Harness Implementation Plan

> **HISTORICAL RECORD — DO NOT EXECUTE.** This plan was fully executed on
> 2026-07-09/10 (all tasks done; boxes below are checked). It is kept only as
> the audit trail of how the harness was built. The embedded file bodies are
> STALE SNAPSHOTS — the shipped `.claude/agents/*.md` and `.claude/workflows/*.js`
> have since evolved (static-analyzer agent, schema and recipe changes); the
> living design doc is `docs/superpowers/specs/2026-07-09-claude-agents-workflows-design.md`.
> Do NOT run superpowers:executing-plans or subagent-driven-development on this
> file, and do not copy its `git commit --no-verify` instructions — they applied
> only to the original build-out.

**Goal:** Add 5 custom worker agents, 6 workflows, and a `/pre-pr` skill that let Claude Code sessions develop and test TinyUSB with cheap-to-orchestrate multi-agent fan-out.

**Architecture:** Layered — agent definitions (`.claude/agents/*.md`) carry TinyUSB domain knowledge and run on opus; workflow scripts (`.claude/workflows/*.js`) do deterministic fan-out/join in plain JS; one skill (`.claude/skills/pre-pr/SKILL.md`) is the human entry point. Spec: `docs/superpowers/specs/2026-07-09-claude-agents-workflows-design.md`.

**Tech Stack:** Claude Code custom agents (markdown + YAML frontmatter), Claude Code Workflow scripts (restricted JS: `agent()/pipeline()/parallel()/phase()/log()/workflow()`), bash, `gh` CLI, existing repo tooling (cmake/ninja, ceedling, `tools/metrics_compare_base.py`, PVS-Studio, `test/hil/hil_test.py`).

## Global Constraints

- Worktree root (all paths below are relative to it): `/home/hathach/code/tinyusb/.claude/worktrees/claude-agents-workflows`
- Tiered worker models (owner revision 2026-07-09): `port-dev`/`driver-reviewer` frontmatter `model: opus`; `hil-operator`/`pr-monitor` `model: sonnet`; `builder` `model: haiku`. Any workflow `agent()` call **without** an `agentType` must pass an explicit `model` (haiku for mechanical stages, sonnet for judgment/push stages) — omitting it inherits the session model (expensive).
- Effort at call sites: `port-dev` and `driver-reviewer` calls pass `effort: 'xhigh'`; haiku-backed calls (`builder`, unit/size stages) pass NO effort; `hil-operator`, `pr-monitor`, and sonnet stages omit `effort` (default) except pvs (`low`).
- Every agent's final message must be raw JSON only (it is parsed by workflow scripts) — each agent file ends with an explicit output contract.
- Workflow scripts must NOT use `Date.now()`, `Math.random()`, or argless `new Date()` (breaks resume). They are not plain ESM (top-level `return` is legal) — syntax-check each with `bash .claude/workflows/check.sh <file>` (created in Task 6), expected output `OK: <file>`.
- Schemas are duplicated between workflow scripts by design: workflow scripts are self-contained and cannot import each other.
- This machine is host `ci` (the HIL rig itself). HIL is LOCAL mode with `test/hil/tinyusb.json`. The actions-runner service is NEVER stopped or started: per-board flock locks arbitrate hardware access (Tasks 19–20). `hil_test.py` self-locks each board while testing it; manual hardware work uses `test/hil/board_lock.py hold/release`; locked boards FAIL fast in CI and are re-runnable.
- `hil-operator` is strictly serial: never two instances concurrently; `hil-validate.js` is the only workflow that drives it, in a plain `for` loop.
- Task ordering: execute Tasks 19–21 (board-lock tool, hil_test.py guard, hil skill update) immediately after Task 5 — Task 4's agent references `board_lock.py`, and smoke Task 16 depends on the whole protocol.
- Commit after every task. Message style: imperative, `feat:`/`docs:`/`chore:` prefix, plus footer:
  ```
  Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
  Claude-Session: https://claude.ai/code/session_01Rn1AN5DsTdFhRwhugfgKZi
  ```
  Use `git commit --no-verify` (these are `.claude/` config + docs files; the repo pre-commit fuzz hook is irrelevant and slow).
- Tasks 13–18 (smoke tests) invoke the Workflow tool and MUST be executed by the main session, not dispatched to a subagent.

---

### Task 1: `builder` agent

**Files:**
- Create: `.claude/agents/builder.md`

**Interfaces:**
- Produces (consumed by Tasks 6, 7 via `agentType: 'builder'`): final-message JSON
  `{"board": string, "pass": boolean, "builtCount": integer, "failures": [{"example": string, "class": string, "firstError": string}]}`

- [x] **Step 1: Write the agent file**

Write `.claude/agents/builder.md` with exactly this content:

````markdown
---
name: builder
description: Build TinyUSB examples for one board and report structured pass/fail with first-error triage. Use for build sweeps and post-change build verification. Never edits source.
tools: Bash, Read, Grep, Glob
model: haiku
---

You build TinyUSB examples for exactly one board per run and report the result as machine-readable JSON. You never modify source files.

## Build commands

Full example set for a board (the default; HIL tests expect this exact build dir name):

```bash
cd examples
cmake -B cmake-build-<BOARD> -DBOARD=<BOARD> -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel .
cmake --build cmake-build-<BOARD>
```

Single example (only when the prompt restricts scope). If the prompt asks for a unique build dir, use `mktemp -d`:

```bash
BUILD=$(mktemp -d /tmp/build-<BOARD>-XXXX)
cmake -S examples/<group>/<example> -B "$BUILD" -DBOARD=<BOARD> -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel
cmake --build "$BUILD"
```

Espressif boards (listed under `hw/bsp/espressif/boards/`): run `. $HOME/code/esp-idf/export.sh` first; only ESP-IDF examples build for them (e.g. `cdc_msc_freertos`): `idf.py -DBOARD=<BOARD> build` from the example dir.

## Recovery rules

- Missing dependency errors (`lib/...` or `hw/mcu/...` not found): run `python3 tools/get_deps.py <FAMILY>` once (FAMILY = the `hw/bsp/` subdir containing the board), then retry.
- objcopy errors during a full sweep are often non-critical: retry that example alone; report it failed only if the retry fails.
- Unknown board: check `hw/bsp/*/boards/`; report class `config-error`.
- Builds of a full set take minutes — use generous Bash timeouts (>= 10 min).

## Failure triage

For each failing example capture the FIRST compiler or linker error line (not the ninja/make summary). Classify each failure: `compile-error` | `link-error` | `config-error` | `deps-missing` | `toolchain-missing` | `other`.

## Output contract

Your final message is parsed by a program. Return ONLY this JSON — no prose, no code fences:

{"board": "<board>", "pass": true, "builtCount": 42, "failures": [{"example": "device/cdc_msc", "class": "compile-error", "firstError": "..."}]}

`pass` is true only when zero failures remain after retries. `builtCount` = number of examples that built.
````

- [x] **Step 2: Verify structure**

Run: `head -8 .claude/agents/builder.md`
Expected: frontmatter block containing `name: builder`, `tools: Bash, Read, Grep, Glob`, `model: opus`.

- [x] **Step 3: Commit**

```bash
git add .claude/agents/builder.md
git commit --no-verify -m "feat: add builder worker agent"
```
(Include the Global Constraints footer in this and every commit.)

---

### Task 2: clang-format install + `port-dev` agent

**Files:**
- Create: `.claude/agents/port-dev.md`

**Interfaces:**
- Produces (consumed by Tasks 7, 11 via `agentType: 'port-dev'`): final-message JSON
  `{"item": string, "diffstat": string, "buildOk": boolean, "board": string, "notes": string}`

- [x] **Step 1: Install clang-format (host tool, one-time)**

Run: `sudo apt-get install -y clang-format && git clang-format -h | head -3`
Expected: apt succeeds; `git clang-format -h` prints usage (proves both `clang-format` and the `git clang-format` subcommand exist). If the package is already installed this is a no-op.

- [x] **Step 2: Write the agent file**

Write `.claude/agents/port-dev.md` with exactly this content:

````markdown
---
name: port-dev
description: Implement one well-scoped change in one TinyUSB port or explicit file set, following repo style and .clang-format, verified by a targeted build. Use for fan-out development across ports and for fixing validated PR findings.
model: opus
---

You implement exactly one specified change in one assigned scope (a directory under `src/portable/`, a class driver, or an explicitly listed file set). Never touch files outside the assigned scope.

## Code rules

- C99, 2-space indent (no tabs); snake_case helpers; UPPER_CASE macros; public APIs `tud_`/`tuh_`; macros `TU_`.
- No dynamic allocation. Defer ISR work to task context. `TU_ASSERT()` for error checks; always check return values.
- Include order: C stdlib → tusb common → drivers → classes.
- Surgical changes: only what the task requires; match surrounding style; do not refactor working code.
- Comments: short, only the non-obvious why.

## Datasheets

When changing dcd/hcd register logic, cross-check the MCU reference manual / datasheet / programming guide in `$HOME/Documents/calibre-library` (search by MCU or USB-IP name). If the document is missing, say so in `notes` and do NOT guess register semantics.

## Finish checklist (in order)

1. Format only the lines you changed: `git clang-format` (no args — formats working-tree changes vs HEAD using the repo `.clang-format`). If it reformats anything, re-check your diff still builds.
2. Verify with a targeted build of `device/cdc_msc` for the board named in your task (or pick one from `hw/bsp/<family>/boards/` whose family uses your scope). Use a unique build dir to survive parallel siblings:
   ```bash
   BUILD=$(mktemp -d /tmp/portdev-<BOARD>-XXXX)
   cmake -S examples/device/cdc_msc -B "$BUILD" -DBOARD=<BOARD> -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel && cmake --build "$BUILD"
   ```
   On missing deps: `python3 tools/get_deps.py <FAMILY>` once, retry.
3. Capture `git diff --stat -- <your scope>` as a single string for `diffstat`.

## Output contract

Your final message is parsed by a program. Return ONLY this JSON — no prose, no code fences:

{"item": "<assigned scope>", "diffstat": "...", "buildOk": true, "board": "<board built>", "notes": "..."}

`buildOk` is the result of step 2. Put datasheet gaps, judgment calls, and anything a reviewer must know into `notes`.
````

- [x] **Step 3: Verify structure**

Run: `head -6 .claude/agents/port-dev.md`
Expected: frontmatter with `name: port-dev`, `model: opus`, and NO `tools:` line (port-dev needs edit tools — inherits all).

- [x] **Step 4: Commit**

```bash
git add .claude/agents/port-dev.md
git commit --no-verify -m "feat: add port-dev worker agent"
```

---

### Task 3: `driver-reviewer` agent

**Files:**
- Create: `.claude/agents/driver-reviewer.md`

**Interfaces:**
- Produces (consumed by Tasks 7, 8, 11 via `agentType: 'driver-reviewer'`): final-message JSON, one of two shapes selected by the calling prompt/schema:
  - findings: `{"scope": string, "dimension": string, "findings": [{"file": string, "line": integer, "snippet": string, "why": string, "severity": "critical"|"major"|"minor", "confidence": "high"|"medium"|"low"}]}`
  - verification: `{"addresses": boolean, "reason": string}` (also used with keys `real`/`reason` when the prompt asks to refute a finding)

- [x] **Step 1: Write the agent file**

Write `.claude/agents/driver-reviewer.md` with exactly this content:

````markdown
---
name: driver-reviewer
description: Review one TinyUSB driver directory or one diff against one review dimension (correctness, ISR safety, datasheet/errata conformance, style) with coverage-first structured findings; or adversarially verify a single finding / fix. Read-only.
tools: Bash, Read, Grep, Glob
model: opus
---

You review exactly the scope given in your prompt (one driver directory, or one git diff) for exactly the dimension(s) given. Read the code yourself; follow callers, headers, and macros as far as needed to judge correctly. You never modify files.

## Datasheets & errata

For register-use review, find the MCU/USB-IP reference manual in `$HOME/Documents/calibre-library` — and ALSO search the library for the part's errata / silicon-bug sheets (search terms: "errata" plus the MCU or USB-IP name). When the code touches behavior an erratum covers, verify the driver implements the documented workaround; a missing erratum workaround IS a finding (severity by impact — the nRF52 erratum-199 DMA class is major). If a needed document is absent, mark affected findings `confidence: "low"` and name the missing document in `why`.

## Reporting discipline

Coverage-first: report every issue you find, including uncertain or low-severity ones — do NOT filter for importance or confidence; a downstream verifier does that. It is better to surface a finding that gets refuted than to silently drop a real bug. For each finding include `severity` (critical|major|minor) and `confidence` (high|medium|low). `snippet` is the offending line(s), `why` is one or two sentences.

## Verification mode

When the prompt instead asks a yes/no question — "does this diff address finding X?" or "try to refute this finding" — investigate with the same rigor and answer only the JSON shape the prompt specifies. When refuting: default to refuted if the claim does not clearly hold in the actual code.

## Output contract

Your final message is parsed by a program. Return ONLY the JSON shape your prompt specifies — no prose, no code fences. Findings shape:

{"scope": "src/portable/...", "dimension": "...", "findings": [{"file": "...", "line": 123, "snippet": "...", "why": "...", "severity": "major", "confidence": "high"}]}
````

- [x] **Step 2: Verify structure**

Run: `head -8 .claude/agents/driver-reviewer.md`
Expected: frontmatter with `name: driver-reviewer`, `tools: Bash, Read, Grep, Glob`, `model: opus`.

- [x] **Step 3: Commit**

```bash
git add .claude/agents/driver-reviewer.md
git commit --no-verify -m "feat: add driver-reviewer worker agent"
```

---

### Task 4: `hil-operator` agent

**Files:**
- Create: `.claude/agents/hil-operator.md`

**Interfaces:**
- Produces (consumed by Task 9 via `agentType: 'hil-operator'`): final-message JSON per calling prompt — board runs `{"board": string, "pass": boolean, "detail": string, "wedged": boolean}`
- Consumes: `test/hil/board_lock.py` (Task 19) for manual hardware work.

- [x] **Step 1: Write the agent file**

Write `.claude/agents/hil-operator.md` with exactly this content:

````markdown
---
name: hil-operator
description: Run TinyUSB hardware-in-the-loop actions on the physical test rig — per-board locking, firmware flash, hil_test.py runs, USB recovery. Strictly one instance at a time. Never edits source; never touches the actions-runner service.
tools: Bash, Read, Grep, Glob
model: sonnet
---

You operate physical USB test hardware. These repo skills are your source of truth — read the relevant one BEFORE acting:

- `.claude/skills/hil/SKILL.md` — run `hostname` first (host `ci` = local mode with `test/hil/tinyusb.json`; host `htpc` = local `local.json` or remote via `test/hil/hil_ci.sh`); the board lock protocol; exact `hil_test.py` invocations.
- `.claude/skills/usb-recover/SKILL.md` — only when a device/fixture is wedged or processes hang in D state.
- `.claude/skills/usb-debug/SKILL.md` — only when you need to explain WHY the host rejected a device (dmesg analysis).

## Board lock protocol (CI runs concurrently — NEVER stop the actions-runner)

The GitHub Actions runner keeps running during your work. Per-board flock locks in `/tmp/tinyusb-hil-locks/` arbitrate the hardware; CI's `hil_test.py` fails fast on locked boards (re-runnable later).

- `python3 test/hil/hil_test.py ...` runs: do NOT pre-hold those boards — `hil_test.py` self-locks each board for its flash+test (pre-holding would deadlock it).
- ANY other hardware action (JLinkExe/openocd/GDB, manual flash, usbtest.py, serial poking): hold first, release when done — release is mandatory cleanup (a crashed holder auto-releases via kernel flock, but do not rely on it):
  ```bash
  python3 test/hil/board_lock.py hold <board...> --reason "<task>"
  # ... hardware work ...
  python3 test/hil/board_lock.py release <board...>
  ```
- Rig-wide operations (uhubctl power cycling, pci-rebind — they renumber buses): `python3 test/hil/board_lock.py hold --all --reason "<why>"` first.
- If a lock is already held by someone else: report holder/reason (`board_lock.py status`) — never force, never kill the holder. If the holder's reason is `hil_test.py`, that is a concurrent CI job mid-test on the board: waiting a few minutes and retrying once is appropriate when your task allows; otherwise return the holder info so the orchestrator can ask the user.
- You cannot ask the user anything. Bypassing a lock (`HIL_NO_BOARD_LOCK=1`, or proceeding with manual hardware work despite a held lock) is allowed ONLY when your prompt explicitly states the user authorized forcing.

## Hard rules

- HIL runs take 2–5 min per board: use Bash timeouts >= 20 min (1200000 ms) and NEVER cancel early.
- One hardware action at a time. You are never run concurrently with another hil-operator.
- On test failure: retry once with `-v` appended. If a board/fixture stops enumerating or tools hang in D state, consult usb-recover and capture `dmesg | tail -50` into `detail`; set `wedged` true.

## Output contract

Your final message is parsed by a program. Return ONLY the JSON shape your prompt specifies — no prose, no code fences. Typical board-run shape:

{"board": "raspberry_pi_pico", "pass": true, "detail": "<per-test summary or first failure>", "wedged": false}
````

- [x] **Step 2: Verify structure**

Run: `head -8 .claude/agents/hil-operator.md`
Expected: frontmatter with `name: hil-operator`, `tools: Bash, Read, Grep, Glob`, `model: opus`.

- [x] **Step 3: Commit**

```bash
git add .claude/agents/hil-operator.md
git commit --no-verify -m "feat: add hil-operator worker agent"
```

---

### Task 5: `pr-monitor` agent

**Files:**
- Create: `.claude/agents/pr-monitor.md`

**Interfaces:**
- Produces (consumed by Task 11 via `agentType: 'pr-monitor'`): final-message JSON
  `{"ci": {"status": "green"|"red"|"running", "infraRerun": [string], "realFailures": [{"check": string, "firstError": string, "files": [string]}]}, "findings": [{"source": string, "commentId": integer, "file": string, "line": integer, "claim": string, "verdict": "valid"|"invalid"|"stale", "reason": string, "fixHint": string}], "replies": [{"commentId": integer, "body": string}], "done": boolean}`

- [x] **Step 1: Write the agent file**

Write `.claude/agents/pr-monitor.md` with exactly this content:

````markdown
---
name: pr-monitor
description: Triage one TinyUSB GitHub PR — CI status + failure classification, infra re-runs, bot review harvesting (Codex/Copilot/Claude) with adversarial validation of each finding against the code. Read/triage/re-run only; never edits code, never pushes.
tools: Bash, Read, Grep, Glob
model: sonnet
---

You triage exactly one PR (number given in your prompt) using `gh`. You never modify source files, never commit, never push.

## CI triage

1. `gh pr checks <N>`. If checks are running and your prompt says to wait, use `gh pr checks <N> --watch` with a Bash timeout >= 30 min.
2. For each failing check, find its run and read the failure: `gh run view <run-id> --log-failed | head -150`.
3. Classify each failure:
   - **infra/flake**: runner lost communication, network/DNS timeouts, artifact 404, docker pull/rate-limit errors, cancelled-by-timeout with no test output.
   - **real**: compile/link errors, test assertions, HIL failures with device output.
4. Re-run infra failures once: `gh run rerun <run-id> --failed`; record run ids in `infraRerun`.
5. For real failures extract the FIRST error line and the source files involved (from the log paths).

## Bot review harvest

- Inline review comments: `gh api repos/{owner}/{repo}/pulls/<N>/comments --paginate` (use `gh repo view --json nameWithOwner -q .nameWithOwner` for owner/repo). Issue comments: `gh pr view <N> --comments`.
- Known signals: Codex posts an issue comment when done — "Didn't find any major issues" means clean, not silence. Copilot is finished when it no longer appears in `requested_reviewers`. Bot logins differ across REST/GraphQL — match authors case-insensitively on substrings `codex`, `copilot`, `claude`.
- For EACH unresolved bot finding: open the file at the cited line in the current checkout and judge the claim adversarially. `valid` only if the code truly has the problem; `invalid` with a concrete refutation otherwise; `stale` if the current code already fixed it.
- Draft a courteous, technical reply for every `invalid`/`stale` finding (cite the code that refutes it). Put them in `replies` with the comment id — a later step posts them; you do not post.

## done

`done` = true only when CI is green (all checks pass, nothing running) AND no unresolved `valid` findings remain.

## Output contract

Your final message is parsed by a program. Return ONLY this JSON — no prose, no code fences:

{"ci": {"status": "green", "infraRerun": [], "realFailures": [{"check": "...", "firstError": "...", "files": ["..."]}]},
 "findings": [{"source": "codex", "commentId": 123, "file": "...", "line": 1, "claim": "...", "verdict": "valid", "reason": "...", "fixHint": "..."}],
 "replies": [{"commentId": 123, "body": "..."}],
 "done": false}
````

- [x] **Step 2: Verify structure**

Run: `head -8 .claude/agents/pr-monitor.md`
Expected: frontmatter with `name: pr-monitor`, `tools: Bash, Read, Grep, Glob`, `model: opus`.

- [x] **Step 3: Commit**

```bash
git add .claude/agents/pr-monitor.md
git commit --no-verify -m "feat: add pr-monitor triage agent"
```

---

### Task 6: workflow syntax checker + `validate.js`

**Files:**
- Create: `.claude/workflows/check.sh`
- Create: `.claude/workflows/validate.js`

**Interfaces:**
- `check.sh <file.js>` prints `OK: <file>` and exits 0 on valid workflow syntax (used by every later workflow task).
- `validate` workflow — Consumes: `builder` agent (Task 1). Args `{boards: string[], examples?: string, base?: string, skip?: ('unit'|'size'|'pvs')[]}`. Returns `{pass: boolean, stages: [{stage, pass, detail}], failures: [...]}` (consumed by Task 10 via `workflow('validate', ...)`).

- [x] **Step 1: Write the checker**

Write `.claude/workflows/check.sh` with exactly this content (then `chmod +x .claude/workflows/check.sh`):

```bash
#!/usr/bin/env bash
# Syntax-check a Claude Code workflow script. Workflow bodies are not plain
# ESM (top-level `return` is legal because the runtime wraps them), so wrap
# in an async arrow that declares the runtime globals before parsing.
set -euo pipefail
f="${1:?usage: check.sh <workflow.js>}"
tmp="$(mktemp --suffix=.mjs)"
trap 'rm -f "$tmp"' EXIT
{
  echo '(async (args, agent, pipeline, parallel, phase, log, workflow, budget) => {'
  sed 's/^export //' "$f"
  echo '})'
} > "$tmp"
node --check "$tmp" && echo "OK: $f"
```

- [x] **Step 2: Verify checker fails on bad input and passes on good**

Run: `printf 'return }broken\n' > /tmp/bad.js; bash .claude/workflows/check.sh /tmp/bad.js; echo "exit=$?"`
Expected: SyntaxError printed, `exit=1` (non-zero).

- [x] **Step 3: Write validate.js**

Write `.claude/workflows/validate.js` with exactly this content:

```javascript
export const meta = {
  name: 'validate',
  description: 'Pre-PR software validation: unit tests + per-board build sweeps + code-size compare + PVS, in parallel, joined into one verdict',
  whenToUse: 'Before opening or updating a PR, after any non-trivial change',
  phases: [{ title: 'Validate', detail: 'unit + builds + size + pvs in parallel' }],
}

// args: { boards: string[], examples?: string, base?: string, skip?: ('unit'|'size'|'pvs')[] }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !Array.isArray(args.boards) || args.boards.length === 0) {
  throw new Error('args must be { boards: string[], examples?, base?, skip? }')
}
const skip = args.skip || []
for (const s of skip) log(`stage skipped by request: ${s}`)
const base = args.base || 'master'
const clip = (s, n = 800) =>
  s.length > n ? s.slice(0, n) + ` …[truncated ${s.length - n} chars]` : s

const STAGE = {
  type: 'object', additionalProperties: false,
  required: ['pass', 'detail'],
  properties: { pass: { type: 'boolean' }, detail: { type: 'string' } },
}
const BUILD = {
  type: 'object', additionalProperties: false,
  required: ['board', 'pass', 'builtCount', 'failures'],
  properties: {
    board: { type: 'string' }, pass: { type: 'boolean' }, builtCount: { type: 'integer' },
    failures: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['example', 'class', 'firstError'],
        properties: { example: { type: 'string' }, class: { type: 'string' }, firstError: { type: 'string' } },
      },
    },
  },
}

const thunks = []

if (!skip.includes('unit')) thunks.push(() =>
  agent(
    'Run the TinyUSB unit tests: cd test/unit-test && ceedling test:all. ' +
    'pass=true only if every test passes. detail = the ceedling summary line, or the first failing test output.',
    { label: 'unit', phase: 'Validate', model: 'haiku', schema: STAGE },
  ).then(r => r && { stage: 'unit', ...r }))

for (const b of args.boards) thunks.push(() =>
  agent(
    `Build TinyUSB examples for board ${b}` + (args.examples ? ` (only: ${args.examples})` : ' (full example set)') + '.',
    { label: `build:${b}`, phase: 'Validate', agentType: 'builder', schema: BUILD },
  ).then(r => r && {
    stage: `build:${b}`, pass: r.pass,
    detail: r.pass ? `${r.builtCount} examples built` : clip(JSON.stringify(r.failures)),
  }))

if (!skip.includes('size')) thunks.push(() =>
  agent(
    `Compare TinyUSB code size against ${base}: python3 tools/metrics_compare_base.py -b ${args.boards[0]} -e device/cdc_msc . ` +
    'The report lands in cmake-metrics/<board>/metrics_compare.md. pass=false only if the tool itself errors; ' +
    'detail = the flash/RAM delta summary from the report (mention any example that grew).',
    { label: 'size', phase: 'Validate', model: 'haiku', schema: STAGE },
  ).then(r => r && { stage: 'size', ...r }))

if (!skip.includes('pvs')) thunks.push(() =>
  agent(
    'Run PVS-Studio static analysis per .claude/skills/pvs/SKILL.md, but with a DEDICATED build dir so you do not collide with parallel build agents: ' +
    `cd examples && cmake -B cmake-build-pvs -DBOARD=${args.boards[0]} -G Ninja -DCMAKE_BUILD_TYPE=MinSizeRel . && cmake --build cmake-build-pvs. ` +
    'Then: pvs-studio-analyzer analyze -f examples/cmake-build-pvs/compile_commands.json -R .PVS-Studio/.pvsconfig -o pvs-report.log -j12 ' +
    '--security-related-issues --misra-c-version 2023 --misra-cpp-version 2008 --use-old-parser ' +
    'and view with: plog-converter -a GA:1,2 -t errorfile pvs-report.log. ' +
    `pass=false only if GA:1 diagnostics exist in files changed vs ${base} (git diff --name-only ${base}...HEAD). ` +
    'detail = GA:1/GA:2 counts plus any diagnostics in changed files.',
    { label: 'pvs', phase: 'Validate', model: 'sonnet', effort: 'low', schema: STAGE },
  ).then(r => r && { stage: 'pvs', ...r }))

const results = (await parallel(thunks)).filter(Boolean)
const dead = thunks.length - results.length
if (dead > 0) log(`${dead} stage agent(s) died — counted as failures`)
const failures = results.filter(r => !r.pass)
log(`${results.length}/${thunks.length} stages completed, ${failures.length} failing`)
return { pass: failures.length === 0 && dead === 0, stages: results, failures }
```

- [x] **Step 4: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/validate.js`
Expected: `OK: .claude/workflows/validate.js`

- [x] **Step 5: Commit**

```bash
git add .claude/workflows/check.sh .claude/workflows/validate.js
git commit --no-verify -m "feat: add validate workflow and workflow syntax checker"
```

---

### Task 7: `fanout-dev.js`

**Files:**
- Create: `.claude/workflows/fanout-dev.js`

**Interfaces:**
- Consumes: `port-dev` (Task 2), `builder` (Task 1), `driver-reviewer` (Task 3).
- Args `{task: string, items: string[], board?: string | {[item]: string}, review?: boolean, worktree?: boolean}`. Returns array of `{item, diffstat, buildOk, board, notes, verifyBuild?, review?}`.

- [x] **Step 1: Write the workflow**

Write `.claude/workflows/fanout-dev.js` with exactly this content:

```javascript
export const meta = {
  name: 'fanout-dev',
  description: 'Implement one described change across many ports/file-sets: one port-dev worker per item, independent builder verification, optional review',
  whenToUse: 'Applying a fix or pattern across multiple TinyUSB ports (e.g. the same DCD bug in several drivers)',
  phases: [
    { title: 'Implement', detail: 'port-dev per item (opus xhigh)' },
    { title: 'Verify', detail: 'builder single-example check' },
    { title: 'Review', detail: 'optional driver-reviewer pass' },
  ],
}

// args: { task: string, items: string[], board?: string | Record<string,string>, review?: boolean, worktree?: boolean }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !args.task || !Array.isArray(args.items) || args.items.length === 0) {
  throw new Error('args must be { task: string, items: string[], board?, review?, worktree? }')
}
const boardFor = (item) =>
  typeof args.board === 'string' ? args.board : (args.board && args.board[item]) || null
const short = (s) => s.replace(/\/+$/, '').split('/').slice(-2).join('/')
if (args.worktree) log('worktree mode: independent builder verification and review skipped (workers verify inside their own worktrees)')

const DEV = {
  type: 'object', additionalProperties: false,
  required: ['item', 'diffstat', 'buildOk', 'board', 'notes'],
  properties: {
    item: { type: 'string' }, diffstat: { type: 'string' }, buildOk: { type: 'boolean' },
    board: { type: 'string' }, notes: { type: 'string' },
  },
}
const BUILD = {
  type: 'object', additionalProperties: false,
  required: ['board', 'pass', 'builtCount', 'failures'],
  properties: {
    board: { type: 'string' }, pass: { type: 'boolean' }, builtCount: { type: 'integer' },
    failures: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['example', 'class', 'firstError'],
        properties: { example: { type: 'string' }, class: { type: 'string' }, firstError: { type: 'string' } },
      },
    },
  },
}
const FINDINGS = {
  type: 'object', additionalProperties: false,
  required: ['scope', 'dimension', 'findings'],
  properties: {
    scope: { type: 'string' }, dimension: { type: 'string' },
    findings: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['file', 'line', 'snippet', 'why', 'severity', 'confidence'],
        properties: {
          file: { type: 'string' }, line: { type: 'integer' }, snippet: { type: 'string' },
          why: { type: 'string' }, severity: { type: 'string' }, confidence: { type: 'string' },
        },
      },
    },
  },
}

const results = await pipeline(
  args.items,

  item => agent(
    `${args.task}\n\nAssigned scope: ${item} — touch nothing outside it.` +
    (boardFor(item)
      ? ` Verify with board ${boardFor(item)}.`
      : ' Pick a verification board from hw/bsp whose family uses this scope.'),
    {
      label: `dev:${short(item)}`, phase: 'Implement',
      agentType: 'port-dev', effort: 'xhigh', schema: DEV,
      ...(args.worktree ? { isolation: 'worktree' } : {}),
    },
  ),

  (dev, item) => {
    if (!dev) return null
    // worktree mode: edits live in the worker's own worktree; an independent
    // verifier in the shared tree cannot see them — trust dev.buildOk.
    if (args.worktree) return dev
    return agent(
      `Build the single example device/cdc_msc for board ${dev.board}. Use a unique build dir (mktemp -d) to avoid collisions with parallel builds.`,
      { label: `verify:${short(item)}`, phase: 'Verify', agentType: 'builder', schema: BUILD },
    ).then(b => {
      // verifyBuild: true/false = real builder verdict; null = builder died
      if (!b) log(`verify:${short(item)}: builder agent died — independent verification unknown`)
      return { ...dev, verifyBuild: b ? b.pass : null }
    })
  },

  (r, item) => {
    if (!r || !args.review || args.worktree) return r
    return agent(
      `Review the uncommitted change in ${item} (inspect with: git diff -- ${item}) against this task:\n${args.task}\n` +
      'Dimension: does the diff correctly and completely implement the task with no unintended side effects? Coverage-first findings.',
      { label: `review:${short(item)}`, phase: 'Review', agentType: 'driver-reviewer', effort: 'xhigh', schema: FINDINGS },
    ).then(f => {
      // review: array = findings; null = reviewer died; absent = not requested
      if (!f) log(`review:${short(item)}: reviewer agent died`)
      return { ...r, review: f ? f.findings : null }
    })
  },
)

const done = results.filter(Boolean)
const dropped = args.items.length - done.length
if (dropped > 0) log(`${dropped} item(s) dropped (worker died)`)
log(`${done.length}/${args.items.length} items completed; ${done.filter(r => r.buildOk && r.verifyBuild !== false).length} build-clean`)
return done
```

- [x] **Step 2: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/fanout-dev.js`
Expected: `OK: .claude/workflows/fanout-dev.js`

- [x] **Step 3: Commit**

```bash
git add .claude/workflows/fanout-dev.js
git commit --no-verify -m "feat: add fanout-dev workflow"
```

---

### Task 8: `driver-review.js`

**Files:**
- Create: `.claude/workflows/driver-review.js`

**Interfaces:**
- Consumes: `driver-reviewer` (Task 3).
- Args `{dirs: string[], dimensions?: string[], question?: string}`. Returns array of `{dir, dim, findings: [finding & {verdict: {real, reason}}]}` — confirmed findings only.
- Note: supersedes the untracked prototype `.claude/workflows/port-audit.js` in the master working tree (deleted at merge time; nothing to do on this branch).

- [x] **Step 1: Write the workflow**

Write `.claude/workflows/driver-review.js` with exactly this content:

```javascript
export const meta = {
  name: 'driver-review',
  description: 'Review driver directories across dimensions with driver-reviewer scanners, then adversarially verify every finding; returns only confirmed findings',
  whenToUse: 'Auditing dcd/hcd drivers for a bug class (pass question) or a full-dimension review (default dimensions)',
  phases: [
    { title: 'Scan', detail: 'driver-reviewer per (dir x dimension)' },
    { title: 'Verify', detail: 'adversarial refutation per finding' },
  ],
}

// args: { dirs: string[], dimensions?: string[], question?: string }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !Array.isArray(args.dirs) || args.dirs.length === 0) {
  throw new Error('args must be { dirs: string[], dimensions?, question? }')
}
const DIMS = args.question ? [args.question] : (args.dimensions || [
  'correctness: transfer state machines, endpoint bookkeeping, completion and error paths',
  'ISR safety: work deferred to task context, shared-state races, register access ordering',
  'register use vs datasheet and MCU errata: cross-check the reference manual AND errata sheets in $HOME/Documents/calibre-library; a missing erratum workaround is a finding',
  'style: repo conventions (TU_ASSERT, no dynamic allocation, include order, naming)',
])
const short = (s) => s.replace(/\/+$/, '').split('/').slice(-2).join('/')

const FINDINGS = {
  type: 'object', additionalProperties: false,
  required: ['scope', 'dimension', 'findings'],
  properties: {
    scope: { type: 'string' }, dimension: { type: 'string' },
    findings: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['file', 'line', 'snippet', 'why', 'severity', 'confidence'],
        properties: {
          file: { type: 'string' }, line: { type: 'integer' }, snippet: { type: 'string' },
          why: { type: 'string' }, severity: { type: 'string' }, confidence: { type: 'string' },
        },
      },
    },
  },
}
const VERDICT = {
  type: 'object', additionalProperties: false,
  required: ['real', 'reason'],
  properties: { real: { type: 'boolean' }, reason: { type: 'string' } },
}

const pairs = args.dirs.flatMap(dir => DIMS.map(dim => ({ dir, dim })))
log(`${pairs.length} scan units (${args.dirs.length} dirs x ${DIMS.length} dimensions)`)

const results = await pipeline(
  pairs,

  p => agent(
    `Review ${p.dir} for exactly one dimension: ${p.dim}. Read the sources yourself. Coverage-first — report everything, a verifier filters.`,
    { label: `scan:${short(p.dir)}`, phase: 'Scan', agentType: 'driver-reviewer', effort: 'xhigh', schema: FINDINGS },
  ),

  (scan, p) => {
    if (!scan) return null  // dead scanner — dropped, counted, and logged below
    if (scan.findings.length === 0) return { dir: p.dir, dim: p.dim, findings: [] }
    return parallel(scan.findings.map(f => () =>
      agent(
        `Adversarially verify ONE review finding about ${p.dir}.\nDimension: ${p.dim}\nFinding: ${JSON.stringify(f)}\n` +
        'Read the cited code plus enough context (callers, ISR paths, macros, and the datasheet if register-related) to judge. ' +
        'Try to REFUTE it; real=true only if it survives your best attempt. Return {"real": bool, "reason": string}.',
        { label: `verify:${short(p.dir)}:${f.line}`, phase: 'Verify', agentType: 'driver-reviewer', effort: 'xhigh', schema: VERDICT },
      ).then(v => v && { ...f, verdict: v })
    )).then(vs => {
      const alive = vs.filter(Boolean)
      if (alive.length < scan.findings.length) {
        log(`${short(p.dir)}: ${scan.findings.length - alive.length} finding(s) lost to dead verifiers — treat as unverified, re-run if needed`)
      }
      return { dir: p.dir, dim: p.dim, findings: alive.filter(x => x.verdict.real) }
    })
  },
)

const units = results.filter(Boolean)
if (units.length < pairs.length) log(`${pairs.length - units.length} scan unit(s) dropped (scanner died)`)
const confirmed = units.filter(r => r.findings.length > 0)
log(`${confirmed.length} scan units produced confirmed findings`)
return confirmed
```

- [x] **Step 2: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/driver-review.js`
Expected: `OK: .claude/workflows/driver-review.js`

- [x] **Step 3: Commit**

```bash
git add .claude/workflows/driver-review.js
git commit --no-verify -m "feat: add driver-review workflow (supersedes port-audit prototype)"
```

---

### Task 9: `hil-validate.js`

**Files:**
- Create: `.claude/workflows/hil-validate.js`

**Interfaces:**
- Consumes: `hil-operator` (Task 4). Requires `examples/cmake-build-<board>/` to exist for every board.
- Args `{boards: string[]}`. Returns `{pass: boolean, results: [{board, pass, detail, wedged}], wedged: [string]}` (consumed by Task 10).

- [x] **Step 1: Write the workflow**

Write `.claude/workflows/hil-validate.js` with exactly this content:

```javascript
export const meta = {
  name: 'hil-validate',
  description: 'Serialized hardware-in-the-loop run: flash+test each board with hil-operator; per-board flock locks arbitrate with concurrent CI (the actions-runner keeps running)',
  whenToUse: 'After validate passes, to exercise built firmware on the physical rig. Requires examples/cmake-build-<board> for each board. If the result has non-empty `locked`, ask the user: force (re-invoke with force: true), continue waiting (re-invoke later), or accept the partial result. Pass force: true ONLY with explicit user authorization.',
  phases: [{ title: 'HIL', detail: 'strictly serial per-board hil-operator runs' }],
}

// args: { boards: string[], force?: boolean }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !Array.isArray(args.boards) || args.boards.length === 0) {
  throw new Error('args must be { boards: string[], force? } with examples/cmake-build-<board> already built')
}

const HIL = {
  type: 'object', additionalProperties: false,
  required: ['board', 'pass', 'detail', 'wedged'],
  properties: {
    board: { type: 'string' }, pass: { type: 'boolean' },
    detail: { type: 'string' }, wedged: { type: 'boolean' },
  },
}

const runBoard = (b) => agent(
  `Run the HIL test for board ${b} per .claude/skills/hil/SKILL.md. Do NOT touch the actions-runner service and do NOT pre-hold the board lock — hil_test.py self-locks the board while testing. ` +
  (args.force
    ? 'THE USER HAS EXPLICITLY AUTHORIZED FORCING: run hil_test.py with HIL_NO_BOARD_LOCK=1 in the environment (bypasses the board lock check; do NOT release or kill the existing holder). '
    : 'If the run fails because the board lock is held (a dev session or concurrent CI job), report pass=false and set detail to start EXACTLY with "board locked:" followed by the holder JSON verbatim — never force the lock. ') +
  'Reserve the phrase "board locked" strictly for lock contention; describe a frozen or non-enumerating board as "unresponsive" instead. ' +
  `Firmware is in examples/cmake-build-${b}. Use the config for this host (hostname first), single-board flag -b ${b}, Bash timeout >= 20 min, never cancel early. ` +
  'On non-lock failures retry once with -v. wedged=true if the board/fixture is unresponsive after the run (capture dmesg | tail -50 into detail).',
  { label: `hil:${b}`, phase: 'HIL', agentType: 'hil-operator', schema: HIL },
)

const results = []
for (const b of args.boards) {
  const r = await runBoard(b)
  results.push(r || { board: b, pass: false, detail: 'hil-operator agent died', wedged: false })
  log(`${b}: ${results[results.length - 1].pass ? 'PASS' : 'FAIL'}`)
}

// A concurrent CI job may have held some boards (its hil_test.py flock).
// CI finishes a board in minutes — retry locked boards once, at the end.
if (!args.force) {
  for (let i = 0; i < results.length; i++) {
    if (results[i].pass || !results[i].detail.startsWith('board locked')) continue
    log(`${results[i].board}: was locked — retrying once`)
    const r = await runBoard(results[i].board)
    if (r) results[i] = r
    else results[i].detail += ' (retry operator died)'
    log(`${results[i].board}: retry ${results[i].pass ? 'PASS' : 'FAIL'}`)
  }
}

const wedged = results.filter(r => r.wedged).map(r => r.board)
if (wedged.length) log(`WEDGED boards needing usb-recover: ${wedged.join(', ')}`)
// Workers cannot prompt the user — surface still-locked boards for the main
// session to ask: force (re-invoke with force: true), wait, or accept.
const locked = args.force ? [] : results.filter(r => !r.pass && r.detail.startsWith('board locked')).map(r => r.board)
if (locked.length) log(`still locked after retry: ${locked.join(', ')} — ask the user: force / keep waiting / accept`)
return { pass: results.length === args.boards.length && results.every(r => r.pass), results, wedged, locked }
```

- [x] **Step 2: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/hil-validate.js`
Expected: `OK: .claude/workflows/hil-validate.js`

- [x] **Step 3: Commit**

```bash
git add .claude/workflows/hil-validate.js
git commit --no-verify -m "feat: add hil-validate workflow (serialized, lock-arbitrated)"
```

---

### Task 10: `full-check.js`

**Files:**
- Create: `.claude/workflows/full-check.js`

**Interfaces:**
- Consumes: `validate` workflow (Task 6), `hil-validate` workflow (Task 9) via `workflow()` nesting (one level — legal).
- Args `{boards: string[], hilBoards?: string[], examples?: string, base?: string, skip?: string[]}`. Returns `{pass, software, hardware}` (consumed by the `/pre-pr` skill, Task 12).

- [x] **Step 1: Write the workflow**

Write `.claude/workflows/full-check.js` with exactly this content:

```javascript
export const meta = {
  name: 'full-check',
  description: 'Composed pre-PR gate: validate (software) then, only if green, hil-validate (hardware)',
  whenToUse: 'One-shot pre-PR verdict; usually launched via the /pre-pr skill',
  phases: [{ title: 'Software' }, { title: 'Hardware' }],
}

// args: { boards: string[], hilBoards?: string[], examples?: string, base?: string, skip?: string[] }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !Array.isArray(args.boards) || args.boards.length === 0) {
  throw new Error('args must be { boards: string[], hilBoards?, examples?, base?, skip? }')
}

phase('Software')
const software = await workflow('validate', {
  boards: args.boards, examples: args.examples, base: args.base, skip: args.skip,
})
if (!software || !software.pass) {
  log('software validation failed — skipping HIL')
  return { pass: false, software, hardware: null }
}

const hilBoards = args.hilBoards || []
if (hilBoards.length === 0) {
  log('no HIL boards requested — software-only verdict')
  return { pass: true, software, hardware: null }
}

phase('Hardware')
const hardware = await workflow('hil-validate', { boards: hilBoards })
if (hardware && hardware.locked && hardware.locked.length) {
  log(`locked boards pending user decision (force / wait / accept): ${hardware.locked.join(', ')}`)
}
return { pass: !!(hardware && hardware.pass), software, hardware }
```

- [x] **Step 2: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/full-check.js`
Expected: `OK: .claude/workflows/full-check.js`

- [x] **Step 3: Commit**

```bash
git add .claude/workflows/full-check.js
git commit --no-verify -m "feat: add full-check composed workflow"
```

---

### Task 11: `pr-babysit.js`

**Files:**
- Create: `.claude/workflows/pr-babysit.js`

**Interfaces:**
- Consumes: `pr-monitor` (Task 5), `port-dev` (Task 2), `driver-reviewer` (Task 3).
- Args `{pr: number, maxCycles?: number, autoPush?: boolean}`. Must run from a checkout of the PR branch. Returns `{pass, cycles, history, reason?, dryRun?}`.
- Push authorization: invoking with `autoPush !== false` authorizes pushes to the PR branch (spec'd exception to hold-pushes rule).

- [x] **Step 1: Write the workflow**

Write `.claude/workflows/pr-babysit.js` with exactly this content:

```javascript
export const meta = {
  name: 'pr-babysit',
  description: 'Drive a PR to green: pr-monitor triage (CI + bot reviews), port-dev fixes for validated findings, driver-reviewer verification, one commit+push per cycle',
  whenToUse: 'After opening a PR, from a checkout of the PR branch. Invoking with autoPush enabled authorizes pushes to that branch.',
  phases: [{ title: 'Triage' }, { title: 'Fix' }, { title: 'Verify' }, { title: 'Push' }],
}

// args: { pr: number, maxCycles?: number, autoPush?: boolean }
if (typeof args === 'string') args = JSON.parse(args)  // tolerate stringified invocation args
if (!args || !args.pr) {
  throw new Error('args must be { pr: number, maxCycles?, autoPush? }; run from a checkout of the PR branch')
}
const maxCycles = args.maxCycles ?? 3

const TRIAGE = {
  type: 'object', additionalProperties: false,
  required: ['ci', 'findings', 'replies', 'done'],
  properties: {
    ci: {
      type: 'object', additionalProperties: false,
      required: ['status', 'infraRerun', 'realFailures'],
      properties: {
        status: { type: 'string', enum: ['green', 'red', 'running'] },
        infraRerun: { type: 'array', items: { type: 'string' } },
        realFailures: {
          type: 'array',
          items: {
            type: 'object', additionalProperties: false,
            required: ['check', 'firstError', 'files'],
            properties: {
              check: { type: 'string' }, firstError: { type: 'string' },
              files: { type: 'array', items: { type: 'string' } },
            },
          },
        },
      },
    },
    findings: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['source', 'commentId', 'file', 'line', 'claim', 'verdict', 'reason', 'fixHint'],
        properties: {
          source: { type: 'string' }, commentId: { type: 'integer' },
          file: { type: 'string' }, line: { type: 'integer' }, claim: { type: 'string' },
          verdict: { type: 'string', enum: ['valid', 'invalid', 'stale'] },
          reason: { type: 'string' }, fixHint: { type: 'string' },
        },
      },
    },
    replies: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['commentId', 'body'],
        properties: { commentId: { type: 'integer' }, body: { type: 'string' } },
      },
    },
    done: { type: 'boolean' },
  },
}
const DEV = {
  type: 'object', additionalProperties: false,
  required: ['item', 'diffstat', 'buildOk', 'board', 'notes'],
  properties: {
    item: { type: 'string' }, diffstat: { type: 'string' }, buildOk: { type: 'boolean' },
    board: { type: 'string' }, notes: { type: 'string' },
  },
}
const CHECK = {
  type: 'object', additionalProperties: false,
  required: ['addresses', 'reason'],
  properties: { addresses: { type: 'boolean' }, reason: { type: 'string' } },
}
const OP = {
  type: 'object', additionalProperties: false,
  required: ['pass', 'detail'],
  properties: { pass: { type: 'boolean' }, detail: { type: 'string' } },
}

// Marking a review thread resolved has no REST endpoint — it needs the
// GraphQL resolveReviewThread mutation. Shared recipe handed to the posting
// agents so a fixed/refuted comment ends up both answered AND resolved.
const RESOLVE_RECIPE =
  'To resolve the review thread for an inline review comment (its integer databaseId is the commentId): ' +
  'get owner/repo via `gh repo view --json nameWithOwner -q .nameWithOwner`; find the thread node id with ' +
  '`gh api graphql -f query=\'query($o:String!,$r:String!,$p:Int!){repository(owner:$o,name:$r){pullRequest(number:$p){reviewThreads(first:100){nodes{id isResolved comments(first:50){nodes{databaseId}}}}}}}\' -F o=OWNER -F r=REPO -F p=' + args.pr + '` ' +
  '(paginate with the endCursor if there are more than 100 threads), pick the thread whose comments contain that databaseId, then resolve it with ' +
  '`gh api graphql -f query=\'mutation($id:ID!){resolveReviewThread(input:{threadId:$id}){thread{isResolved}}}\' -F id=THREAD_ID`. ' +
  'Issue comments (the 404 fallback case) have no thread — do not try to resolve those.'

const history = []
for (let cycle = 1; cycle <= maxCycles; cycle++) {
  const t = await agent(
    `Triage PR #${args.pr}. If checks are still running, wait for them first (gh pr checks ${args.pr} --watch, Bash timeout >= 30 min). ` +
    'Then follow your triage procedure: classify CI failures, re-run infra ones, harvest and adversarially validate bot review findings, draft replies for invalid/stale ones.',
    { label: `triage#${cycle}`, phase: 'Triage', agentType: 'pr-monitor', schema: TRIAGE },
  )
  if (!t) {
    history.push({ cycle, error: 'pr-monitor died' })
    return { pass: false, cycles: cycle, history, reason: 'pr-monitor-died' }
  }
  const entry = { cycle, triage: t }
  history.push(entry)

  // Post drafted replies to REFUTED findings as soon as triage produces them —
  // decoupled from fixing/pushing so done/unactionable cycles still post.
  // Reply AND resolve the thread. Outward-facing, so gated on autoPush.
  if (t.replies.length > 0 && args.autoPush !== false) {
    const posted = await agent(
      `Reply to and resolve these refuted review comments on PR #${args.pr}. For each: post the reply with ` +
      `gh api repos/{owner}/{repo}/pulls/${args.pr}/comments/{commentId}/replies -f body=<body> ` +
      '(valid for inline review comments); if that 404s, the id is an issue comment — post a regular PR comment instead ' +
      `(gh pr comment ${args.pr} --body <quote the original point, then the reply>) and skip resolving. ` +
      `After replying to an inline comment, mark its thread resolved. ${RESOLVE_RECIPE} ` +
      `Replies: ${JSON.stringify(t.replies)}. pass=true only if every reply was posted and every inline thread resolved; detail = what went where.`,
      { label: `replies#${cycle}`, phase: 'Push', model: 'sonnet', schema: OP },
    )
    if (!posted || !posted.pass) log(`cycle ${cycle}: refuted reply/resolve incomplete — ${posted ? posted.detail : 'agent died'}`)
  }

  if (t.done) {
    log(`cycle ${cycle}: PR is green with no unresolved valid findings`)
    return { pass: true, cycles: cycle, history }
  }

  // Group actionable work by top-level scope (plain JS — no model tokens).
  const groups = new Map()
  const groupOf = (key) => {
    if (!groups.has(key)) groups.set(key, { key, files: new Set(), notes: [] })
    return groups.get(key)
  }
  for (const f of t.findings.filter(x => x.verdict === 'valid')) {
    const g = groupOf(f.file.split('/').slice(0, 3).join('/'))
    g.files.add(f.file)
    g.notes.push(`${f.file}:${f.line} [${f.source}] ${f.claim} — hint: ${f.fixHint}`)
  }
  for (const rf of t.ci.realFailures) {
    const g = groupOf((rf.files[0] || rf.check).split('/').slice(0, 3).join('/'))
    rf.files.forEach(x => g.files.add(x))
    g.notes.push(`CI ${rf.check}: ${rf.firstError}`)
  }
  const work = [...groups.values()]

  if (work.length === 0) {
    if (t.ci.status === 'running' || t.ci.infraRerun.length > 0) {
      log(`cycle ${cycle}: only infra re-runs in flight — next cycle waits on them`)
      continue
    }
    log(`cycle ${cycle}: nothing actionable`)
    return { pass: false, cycles: cycle, history, reason: 'unactionable' }
  }

  const fixes = await pipeline(
    work,
    w => agent(
      `Fix the following issues on the current PR branch (the working tree IS the PR checkout).\n` +
      `Scope: ${[...w.files].join(', ')}\nIssues:\n- ${w.notes.join('\n- ')}`,
      { label: `fix:${w.key}`, phase: 'Fix', agentType: 'port-dev', effort: 'xhigh', schema: DEV },
    ),
    (fix, w) => fix && agent(
      `Verify the uncommitted changes for ${[...w.files].join(', ')} (use git diff -- <files>, and read any newly created untracked files directly) address these issues:\n- ${w.notes.join('\n- ')}\n` +
      'Return {"addresses": bool, "reason": string}.',
      { label: `check:${w.key}`, phase: 'Verify', agentType: 'driver-reviewer', effort: 'xhigh', schema: CHECK },
    ).then(v => ({ ...fix, addresses: !!(v && v.addresses), checkReason: v ? v.reason : 'verifier died' })),
  )
  const aliveFixes = fixes.filter(Boolean)
  if (aliveFixes.length < work.length) log(`${work.length - aliveFixes.length} fix group(s) lost to dead workers`)
  entry.fixes = aliveFixes

  if (args.autoPush === false) {
    log('autoPush=false: fixes left uncommitted in the working tree (dry run)')
    return { pass: false, cycles: cycle, history, dryRun: true }
  }

  // Verification gates the push: never push a cycle containing an unverified
  // fix or the partial edits of a dead worker.
  const unverified = aliveFixes.filter(f => f.addresses !== true)
  if (aliveFixes.length < work.length || unverified.length > 0) {
    for (const f of unverified) log(`fix for ${f.item}: failed verification — ${f.checkReason}`)
    log(`cycle ${cycle}: fixes left uncommitted for human review — not pushing unverified changes`)
    return { pass: false, cycles: cycle, history, reason: 'fix-verification-failed' }
  }

  const push = await agent(
    `On the current PR branch: commit ALL working-tree changes as ONE commit (imperative message summarizing the cycle-${cycle} fixes for PR #${args.pr}, repo commit conventions), ` +
    "then push to the PR's remote branch. pass=true only if commit AND push succeeded; detail = pushed SHA.",
    { label: `push#${cycle}`, phase: 'Push', model: 'sonnet', schema: OP },
  )
  if (!push || !push.pass) {
    log(`cycle ${cycle}: push failed — stopping`)
    return { pass: false, cycles: cycle, history, reason: 'push-failed' }
  }

  // The valid bot findings were fixed and pushed — answer each inline comment
  // with what changed and resolve its thread. CI-failure work has no comment.
  const fixed = t.findings.filter(x => x.verdict === 'valid')
  if (fixed.length > 0) {
    const resolved = await agent(
      `The fixes for PR #${args.pr}'s valid review findings were just committed and pushed (${push.detail}). ` +
      'For each finding below: post a threaded reply to its inline comment via ' +
      `gh api repos/{owner}/{repo}/pulls/${args.pr}/comments/{commentId}/replies -f body=<body>, stating it is fixed in the pushed commit and one line on the change, ` +
      `then mark its thread resolved. ${RESOLVE_RECIPE} ` +
      `Findings: ${JSON.stringify(fixed.map(f => ({ commentId: f.commentId, file: f.file, line: f.line, claim: f.claim, fixHint: f.fixHint })))}. ` +
      'pass=true only if every reply was posted and every thread resolved; detail = what went where.',
      { label: `resolve#${cycle}`, phase: 'Push', model: 'sonnet', schema: OP },
    )
    if (!resolved || !resolved.pass) log(`cycle ${cycle}: fixed reply/resolve incomplete — ${resolved ? resolved.detail : 'agent died'}`)
  }
}
return { pass: false, cycles: maxCycles, history, reason: 'maxCycles reached' }
```

- [x] **Step 2: Syntax-check**

Run: `bash .claude/workflows/check.sh .claude/workflows/pr-babysit.js`
Expected: `OK: .claude/workflows/pr-babysit.js`

- [x] **Step 3: Commit**

```bash
git add .claude/workflows/pr-babysit.js
git commit --no-verify -m "feat: add pr-babysit workflow"
```

---

### Task 12: `/pre-pr` skill

**Files:**
- Create: `.claude/skills/pre-pr/SKILL.md`

**Interfaces:**
- Consumes: `full-check` workflow (Task 10), `test/hil/tinyusb.json` (board roster), `hw/bsp/*/family.cmake|family.mk` (family mapping).

- [x] **Step 1: Write the skill**

Write `.claude/skills/pre-pr/SKILL.md` with exactly this content:

````markdown
---
name: pre-pr
description: Use before opening or updating a TinyUSB PR — derives affected boards from the branch diff, runs the full-check workflow (software validation + optional HIL on the rig), and summarizes a ship/no-ship verdict.
---

# /pre-pr — pre-PR validation

Run the software + hardware gate for the current branch. The user invoking this skill is the opt-in for launching the workflows below.

## 1. Scout the diff (inline — no agents)

- `BASE` = `master` unless the user names another base.
- `git diff --name-only $(git merge-base HEAD $BASE)..HEAD`
- If NO C sources changed (only docs / `.claude/` / tools): say so, and run a minimal software-only gate — `boards = [stm32f407disco]`, no HIL — unless the user asks for more.

## 2. Map changes to boards

- For each changed `src/portable/<vendor>/<ip>/` (or `src/portable/<name>/` for single-level ports): families = the `hw/bsp/<family>` directories whose build files reference it — `grep -rl "<vendor>/<ip>" hw/bsp/*/family.cmake hw/bsp/*/family.mk`, then take each matching file's directory name.
- For `src/class/*`, `src/common/*`, `src/device/*`, `src/host/*`, or `src/tusb.c`: broad change — use `stm32f407disco` + `raspberry_pi_pico` PLUS any families from portable changes.
- For `hw/bsp/<family>/...` changes: that family directly.
- Catch-all: any other C/CMake source change (`examples/*`, `test/*`, anything unmatched above) → the representative set `stm32f407disco` + `raspberry_pi_pico`. The boards list must NEVER end up empty — final fallback is `[stm32f407disco]` (full-check throws on an empty list).
- Rig roster: `python3 -c "import json;print([b['name'] for b in json.load(open('test/hil/tinyusb.json'))['boards']])"`
- Pick ONE board per affected family, preferring boards on the rig roster; otherwise the first entry in `hw/bsp/<family>/boards/`. Cap at 4 boards and tell the user which families the cap dropped.

## 3. HIL boards

- `hilBoards` = chosen boards that are on the rig roster. This host must be able to reach the rig (per `.claude/skills/hil/SKILL.md`: host `ci` = local, `htpc` = remote). If none qualify, run software-only.

## 4. Launch

Invoke the Workflow tool:

```
{ name: 'full-check', args: { boards: [...], hilBoards: [...], base: BASE } }
```

## 5. Summarize

- Per-stage table: unit / build:<board> / size / pvs, then HIL per board — pass/fail with the first error for each failure.
- If the hardware result has non-empty `locked` (a CI job held those boards): ask the user with AskUserQuestion — **Force now** (re-invoke `hil-validate` with `force: true` for those boards; user accepts the risk of colliding with a mid-test CI job), **Keep waiting** (re-invoke `hil-validate` for them after a few minutes; ask again if still locked), or **Accept** the partial verdict. Never force without the user's answer.
- Wedged boards: point at `.claude/skills/usb-recover/SKILL.md`.
- End with a clear ship / no-ship verdict and what to fix first.
````

- [x] **Step 2: Verify structure**

Run: `head -4 .claude/skills/pre-pr/SKILL.md`
Expected: frontmatter with `name: pre-pr` and a `description:` line mentioning full-check.

- [x] **Step 3: Commit**

```bash
git add .claude/skills/pre-pr/SKILL.md
git commit --no-verify -m "feat: add /pre-pr skill entry point"
```

---

### Task 13: Smoke test — `validate` (MAIN SESSION ONLY)

**Files:** none created; exercises Tasks 1, 6.

- [x] **Step 1: Run** — Invoke the Workflow tool: `{ name: 'validate', args: { boards: ['stm32f407disco', 'raspberry_pi_pico'] } }`. (If board deps are missing the builder self-heals via get_deps.)
- [x] **Step 2: Verify** — Returned object has `stages` entries for `unit`, `build:stm32f407disco`, `build:raspberry_pi_pico`, `size`, `pvs`; each `detail` is meaningful; `pass` consistent with stage results. Master is green, so expect `pass: true`; investigate any failure before proceeding (a real regression on master is possible but unlikely).
- [x] **Step 3: Record** — Append the verdict JSON (and rough token usage from /workflows) to `docs/superpowers/plans/2026-07-09-smoke-results.md`; commit with `docs: record validate smoke result`.

### Task 14: Smoke test — `fanout-dev` (MAIN SESSION ONLY)

- [x] **Step 1: Run** — Invoke Workflow: `{ name: 'fanout-dev', args: { task: 'Add a single comment line `// fanout-dev smoke test — remove me` at the very top of the main dcd_*.c source file in your assigned scope. Make no other change.', items: ['src/portable/raspberrypi/rp2040', 'src/portable/st/stm32_fsdev'], board: { 'src/portable/raspberrypi/rp2040': 'raspberry_pi_pico', 'src/portable/st/stm32_fsdev': 'stm32f072disco' } } }`
- [x] **Step 2: Verify** — Both items return `buildOk: true` and `verifyBuild: true`; `git diff --stat` shows exactly 2 files, 1 insertion each; diffs are clang-format-clean (`git clang-format --diff` reports no changes).
- [x] **Step 3: Revert the smoke edits** — `git checkout -- src/portable/` (verify `git status` clean afterwards).
- [x] **Step 4: Record** — Append results to the smoke-results doc; commit.

### Task 15: Smoke test — `driver-review` (MAIN SESSION ONLY)

- [x] **Step 1: Run** — Invoke Workflow: `{ name: 'driver-review', args: { dirs: ['src/portable/renesas/rusb2', 'src/portable/nxp/lpc_ip3511'], question: 'unbounded busy-wait loops polling hardware status bits with no timeout or bail-out (whole-stack freeze risk if hardware never sets the bit)' } }`
- [x] **Step 2: Verify** — Returns only confirmed findings, each carrying `verdict.real: true` with a reasoned `verdict.reason`; spot-check one finding by reading the cited code yourself. (rusb2's known FRDY wedge was bounded in a past fix — a clean result there is plausible; judge findings on the code, not on expectations.)
- [x] **Step 3: Record** — Append results to the smoke-results doc; commit.

### Task 16: Smoke test — `hil-validate` + board locks (MAIN SESSION ONLY)

- [x] **Step 1: Preconditions** — `ls examples/cmake-build-raspberry_pi_pico/` exists (from Task 13); Tasks 19–20 are done (`test/hil/board_lock.py` exists, `hil_test.py` guard in place); runner is ACTIVE: `systemctl is-active actions.runner.hathach-tinyusb.tinyusb.service` prints `active`.
- [x] **Step 2: Lock-conflict run** — `python3 test/hil/board_lock.py hold raspberry_pi_pico --reason "smoke lock test"`, then invoke Workflow: `{ name: 'hil-validate', args: { boards: ['raspberry_pi_pico'] } }`.
  Expected: the board entry FAILS fast, `detail` cites `board locked` with the holder JSON (reason `smoke lock test`), no flash occurred (no JLink/flasher output), and the result carries `locked: ['raspberry_pi_pico']` — the signal for the force/wait/accept user prompt.
- [x] **Step 3: Force path (lock still held)** — invoke Workflow: `{ name: 'hil-validate', args: { boards: ['raspberry_pi_pico'], force: true } }`.
  Expected: real flash+test proceeds despite the held lock (operator ran `HIL_NO_BOARD_LOCK=1`), result has empty `locked`, and the `board_lock.py status` holder is still alive afterwards (bypass, not theft).
- [x] **Step 4: Release and normal run** — `python3 test/hil/board_lock.py release raspberry_pi_pico`, invoke the Step 2 Workflow call again (no `force`).
  Expected: a real flash+test result via the normal self-locking path (pass expected — firmware is master-green).
- [x] **Step 5: Runner untouched** — `systemctl is-active actions.runner.hathach-tinyusb.tinyusb.service` still prints `active`; the workflow made no stop/start calls.
- [x] **Step 6: Record** — Append all three results to the smoke-results doc; update the memory note `~/.claude/projects/-home-hathach-code-tinyusb/memory/ci-rig-stop-actions-runner.md` to describe the lock protocol (with the caveat that CI enforces it only after this branch merges to master); commit the smoke-results doc.

### Task 17: Smoke test — `/pre-pr` end-to-end (MAIN SESSION ONLY)

- [x] **Step 1: Run** — Invoke the `pre-pr` skill on this branch (its diff is docs + `.claude/` only, so expect the minimal path: software-only, `boards = [stm32f407disco]`).
- [x] **Step 2: Verify** — The skill correctly detects "no C changes", runs `full-check` with the minimal args, and produces the per-stage summary + verdict.
- [x] **Step 3: Record** — Append to the smoke-results doc; commit.

### Task 18: Smoke test — `pr-babysit` dry run (MAIN SESSION ONLY)

- [x] **Step 1: Pick a target** — `gh pr list --limit 10 --json number,title,headRefName` — choose an open PR with completed CI and at least one bot review comment; check out its branch in a THROWAWAY worktree (`git worktree add /tmp/prsmoke <headRef>` after `git fetch`), and run from there so fix edits can't dirty this branch.
- [x] **Step 2: Run** — Invoke Workflow from that checkout: `{ name: 'pr-babysit', args: { pr: <N>, maxCycles: 1, autoPush: false } }`
- [x] **Step 3: Verify** — Triage classifies CI checks plausibly (compare with `gh pr checks <N>` yourself); each bot finding has a reasoned verdict (spot-check one against the code); result has `dryRun: true` if fixes were produced, and NOTHING was committed or pushed (`git -C /tmp/prsmoke status`, `gh pr view <N> --json comments` unchanged).
- [x] **Step 4: Clean up** — `git worktree remove --force /tmp/prsmoke`.
- [x] **Step 5: Record** — Append to the smoke-results doc; commit.

---

### Task 19: `test/hil/board_lock.py` (execute right after Task 5)

**Files:**
- Create: `test/hil/board_lock.py`

**Interfaces:**
- Produces (consumed by `hil-operator` agent, Task 20's guard shares the same lock files): CLI `hold <board...> [--all] [--config PATH] --reason TEXT` / `release <board...> [--all]` / `status`. Lock files: `/tmp/tinyusb-hil-locks/<board>.lock`, exclusive `fcntl.flock` held by a background holder process; JSON `{pid, reason, since}` written into the file.

- [x] **Step 1: Write the tool**

Write `test/hil/board_lock.py` with exactly this content (then `chmod +x test/hil/board_lock.py`):

```python
#!/usr/bin/env python3
"""Per-board advisory locks for the HIL rig.

Arbitrates board access between dev sessions and CI's hil_test.py without
stopping the actions-runner. Locks are kernel flocks: the kernel releases
them automatically when the holder process dies, so stale locks are
impossible (/tmp also clears on reboot).

Usage:
  board_lock.py hold BOARD [BOARD...] --reason TEXT
  board_lock.py hold --all [--config test/hil/tinyusb.json] --reason TEXT
  board_lock.py release BOARD [BOARD...] | release --all
  board_lock.py status

A holder process holds ALL boards given in one `hold` call; releasing any of
them kills that holder and releases all of its boards.
"""
import argparse
import fcntl
import json
import os
import select
import signal
import sys
import time

LOCK_DIR = '/tmp/tinyusb-hil-locks'


def lock_path(board: str) -> str:
    return os.path.join(LOCK_DIR, f'{board}.lock')


def boards_from_config(config: str) -> list:
    try:
        with open(config) as f:
            return [b['name'] for b in json.load(f)['boards']]
    except (OSError, ValueError, KeyError) as e:
        print(f'ERROR: cannot read board roster {config}: {e}', file=sys.stderr)
        sys.exit(1)


def read_info(board: str):
    try:
        with open(lock_path(board)) as f:
            return json.load(f)
    except (OSError, ValueError):
        return None


def is_locked(board: str) -> bool:
    """True if some live process currently holds the flock."""
    path = lock_path(board)
    if not os.path.exists(path):
        return False
    with open(path) as f:
        try:
            fcntl.flock(f, fcntl.LOCK_EX | fcntl.LOCK_NB)
            fcntl.flock(f, fcntl.LOCK_UN)
            return False
        except OSError:
            return True


def cmd_hold(boards, reason):
    os.makedirs(LOCK_DIR, exist_ok=True)
    already = [b for b in boards if is_locked(b)]
    if already:
        for b in already:
            print(f'ERROR: {b} already locked: {read_info(b)}', file=sys.stderr)
        return 1
    # The holder signals success through this pipe. A generic is_locked()
    # poll would be fooled by a RIVAL invocation's flock — only the holder
    # itself knows whether it won every board.
    r_fd, w_fd = os.pipe()
    pid = os.fork()
    if pid > 0:
        os.close(w_fd)
        os.waitpid(pid, 0)  # reap intermediate child
        ready, _, _ = select.select([r_fd], [], [], 10)
        ok = bool(ready) and os.read(r_fd, 1) == b'1'
        os.close(r_fd)
        if ok:
            print(f'held: {", ".join(boards)}')
            return 0
        print('ERROR: holder failed to acquire locks (lost a race?)', file=sys.stderr)
        return 1
    # intermediate child: detach, then spawn the actual holder
    os.setsid()
    if os.fork() > 0:
        os._exit(0)
    # holder (grandchild): acquire all flocks, signal the parent, sleep until killed
    os.close(r_fd)
    try:
        handles = []
        for b in boards:
            # O_RDWR without O_TRUNC: never truncate before the flock is
            # held — a losing racer must not wipe the winner's holder info.
            fd = os.open(lock_path(b), os.O_RDWR | os.O_CREAT, 0o666)
            fh = os.fdopen(fd, 'r+')
            fcntl.flock(fh, fcntl.LOCK_EX | fcntl.LOCK_NB)
            fh.truncate(0)
            fh.seek(0)
            json.dump({'pid': os.getpid(), 'reason': reason,
                       'since': time.strftime('%Y-%m-%dT%H:%M:%S%z')}, fh)
            fh.flush()
            handles.append(fh)
    except OSError:
        try:
            os.write(w_fd, b'0')
        except OSError:
            pass
        os._exit(1)  # lost a race; parent reports the failure
    os.write(w_fd, b'1')
    os.close(w_fd)
    signal.signal(signal.SIGTERM, lambda *_: os._exit(0))
    while True:
        signal.pause()


def cmd_release(boards):
    pids = set()
    for b in boards:
        if not is_locked(b):
            continue
        info = read_info(b) or {}
        if info.get('pid'):
            pids.add(info['pid'])
    for holder in sorted(pids):
        try:
            os.kill(holder, signal.SIGTERM)
            print(f'released holder pid {holder}')
        except ProcessLookupError:
            pass
    time.sleep(0.3)
    still = [b for b in boards if is_locked(b)]
    if still:
        print(f'ERROR: still locked: {", ".join(still)}', file=sys.stderr)
        return 1
    return 0


def cmd_status():
    if not os.path.isdir(LOCK_DIR):
        print('no locks')
        return 0
    any_locked = False
    for fn in sorted(os.listdir(LOCK_DIR)):
        if not fn.endswith('.lock'):
            continue
        b = fn[:-5]
        if is_locked(b):
            any_locked = True
            print(f'{b}: {read_info(b)}')
    if not any_locked:
        print('no locks')
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    p_hold = sub.add_parser('hold')
    p_hold.add_argument('boards', nargs='*')
    p_hold.add_argument('--all', action='store_true')
    p_hold.add_argument('--config', default='test/hil/tinyusb.json')
    p_hold.add_argument('--reason', required=True)
    p_rel = sub.add_parser('release')
    p_rel.add_argument('boards', nargs='*')
    p_rel.add_argument('--all', action='store_true')
    sub.add_parser('status')
    a = ap.parse_args()
    if a.cmd == 'hold':
        boards = boards_from_config(a.config) if a.all else a.boards
        if not boards:
            ap.error('no boards given (name boards or use --all)')
        sys.exit(cmd_hold(boards, a.reason))
    if a.cmd == 'release':
        if a.all:
            boards = ([fn[:-5] for fn in os.listdir(LOCK_DIR) if fn.endswith('.lock')]
                      if os.path.isdir(LOCK_DIR) else [])
        else:
            boards = a.boards
        if not boards:
            ap.error('no boards given (name boards or use --all)')
        sys.exit(cmd_release(boards))
    sys.exit(cmd_status())


if __name__ == '__main__':
    main()
```

- [x] **Step 2: Test the lock lifecycle**

Run each line and check the expectation before the next:

```bash
python3 test/hil/board_lock.py status                                        # -> "no locks"
python3 test/hil/board_lock.py hold fakeboard --reason "test hold"           # -> "held: fakeboard", exit 0
python3 test/hil/board_lock.py status                                        # -> fakeboard: {'pid': ..., 'reason': 'test hold', ...}
python3 test/hil/board_lock.py hold fakeboard --reason "second"; echo $?     # -> ERROR ... already locked, exit 1
python3 test/hil/board_lock.py release fakeboard                             # -> "released holder pid ...", exit 0
python3 test/hil/board_lock.py status                                        # -> "no locks"
python3 test/hil/board_lock.py hold fakeboard --reason "crash test"
kill -9 $(python3 -c "import json;print(json.load(open('/tmp/tinyusb-hil-locks/fakeboard.lock'))['pid'])")
sleep 0.3; python3 test/hil/board_lock.py status                             # -> "no locks"  (kernel auto-released)
python3 test/hil/board_lock.py hold fakeboard --reason "race test"
python3 -c "
import fcntl, os
fd = os.open('/tmp/tinyusb-hil-locks/fakeboard.lock', os.O_RDWR | os.O_CREAT, 0o666)
fh = os.fdopen(fd, 'r+')
try:
    fcntl.flock(fh, fcntl.LOCK_EX | fcntl.LOCK_NB); print('ERROR: stole lock')
except OSError:
    print('refused; info intact:', fh.read())
"                                                                            # -> refused; info intact: {..."reason": "race test"...}
python3 test/hil/board_lock.py release fakeboard                             # -> released holder pid ...
rm -f /tmp/tinyusb-hil-locks/fakeboard.lock
```

- [x] **Step 3: Commit**

```bash
git add test/hil/board_lock.py
git commit --no-verify -m "feat(hil): add per-board advisory lock tool"
```

---

### Task 20: `hil_test.py` board-lock guard (execute after Task 19)

**Files:**
- Modify: `test/hil/hil_test.py` (small guard in the per-board worker path)

**Interfaces:**
- Consumes: the lock files of Task 19 (`/tmp/tinyusb-hil-locks/<board>.lock`).
- Produces: a locked board FAILS immediately with `board locked: <holder JSON>` (no flash); while testing, `hil_test.py` holds the board's flock and writes its own holder info (`reason: "hil_test.py"`) so the reverse conflict reports truthfully. CI semantics: job fails for locked boards, `re-run failed` passes after release.

- [x] **Step 1: Locate the per-board entry point**

Run: `grep -n "Pool(\|\.map\|\.imap\|def test_board\|def run_board" test/hil/hil_test.py`
Identify the function the `multiprocessing.Pool` maps over the board list (each worker process handles one board's flash+test) and how it reports failure (inspect how a flash error is reported/raised so the locked case matches that convention exactly).

- [x] **Step 2: Add the guard**

Add near the top of `hil_test.py` (module level, after existing imports — `fcntl` and `os` may need importing):

```python
# --- per-board dev-session locks (see test/hil/board_lock.py) ------------
BOARD_LOCK_DIR = '/tmp/tinyusb-hil-locks'

def acquire_board_lock(board_name):
    """Take this board's flock for the duration of its flash+test.
    Returns an open file handle (keep it referenced; closing releases it),
    or None when HIL_NO_BOARD_LOCK=1 or the lock dir is unusable (fail-open:
    locking must never break a test run by itself).
    Raises RuntimeError only when another session holds the board."""
    import fcntl
    if os.environ.get('HIL_NO_BOARD_LOCK') == '1':
        return None  # user-authorized bypass — see board_lock.py / hil skill
    try:
        os.makedirs(BOARD_LOCK_DIR, exist_ok=True)
        fd = os.open(os.path.join(BOARD_LOCK_DIR, f'{board_name}.lock'),
                     os.O_RDWR | os.O_CREAT, 0o666)
        fh = os.fdopen(fd, 'r+')
    except OSError:
        return None  # odd lock dir (perms, path collision): proceed unlocked
    try:
        fcntl.flock(fh, fcntl.LOCK_EX | fcntl.LOCK_NB)
    except OSError:
        try:
            info = fh.read(500).strip()
        except (OSError, UnicodeDecodeError):
            info = ''
        fh.close()
        raise RuntimeError(f'board locked: {info or "unknown holder"}')
    # announce ourselves so the other side's conflict message is truthful;
    # best-effort — the flock itself is already held
    try:
        fh.truncate(0)
        fh.seek(0)
        json.dump({'pid': os.getpid(), 'reason': 'hil_test.py',
                   'since': time.strftime('%Y-%m-%dT%H:%M:%S%z')}, fh)
        fh.flush()
    except OSError:
        pass
    return fh
```

(`os`, `json`, and `time` are already imported by `hil_test.py`; `fcntl` is imported locally above.)

Then wrap the per-board worker function's body:

```python
try:
    _lock_fh = acquire_board_lock(<the board-name variable used in that function>)
except RuntimeError as e:
    <report failure using the function's existing failure convention, message = str(e)>
    <return/raise per that convention>
try:
    <existing flash+test body, unchanged>
finally:
    if _lock_fh:
        _lock_fh.close()   # closing releases the flock (None under HIL_NO_BOARD_LOCK=1)
```

Match indentation and the file's existing style exactly; keep the diff minimal (guard function + one try/finally wrap).

- [x] **Step 3: Test the locked path (no hardware touched)**

```bash
python3 test/hil/board_lock.py hold raspberry_pi_pico --reason "guard test"
python3 test/hil/hil_test.py -b raspberry_pi_pico -B examples test/hil/tinyusb.json; echo "exit=$?"
python3 test/hil/board_lock.py release raspberry_pi_pico
```
Expected: the run fails FAST (seconds, no JLink/flasher invocation in output), the board's failure message contains `board locked: {"pid": ..., "reason": "guard test", ...}`, exit code non-zero. (The unlocked happy path is exercised on real hardware in Task 16.)

- [x] **Step 4: Sanity-check no syntax damage**

Run: `python3 -m py_compile test/hil/hil_test.py && echo OK`
Expected: `OK`

- [x] **Step 5: Commit**

```bash
git add test/hil/hil_test.py
git commit --no-verify -m "feat(hil): fail fast on dev-locked boards instead of stopping the runner"
```

---

### Task 21: Update the `hil` skill for the lock protocol (execute after Task 20)

**Files:**
- Modify: `.claude/skills/hil/SKILL.md` (replace the "Stop the CI runner first" section, currently lines 17–29)

- [x] **Step 1: Replace the runner-stop section**

In `.claude/skills/hil/SKILL.md`, replace the entire section from the heading `## Stop the CI runner first (on \`ci\`)` up to (not including) `## Prerequisites` with:

````markdown
## Board locks — the CI runner keeps running

The `ci` rig also hosts a GitHub Actions runner that flashes boards and runs HIL as part of CI. Hardware access is arbitrated **per board** with kernel flocks in `/tmp/tinyusb-hil-locks/` — do NOT stop the runner service.

- `hil_test.py` self-locks each board for the duration of its flash+test (holder reason `hil_test.py`). A locked board FAILS immediately (`FAILED (board locked: ...)`) without flashing — in CI, re-run the failed job once the lock is released.
- If your `hold` fails and the holder's reason is `hil_test.py`, a CI job is mid-test on that board — wait a few minutes and retry rather than forcing.
- For hardware work outside `hil_test.py` (JLink/GDB, manual flashing, `usbtest.py`, serial poking), hold the lock first:

```bash
python3 test/hil/board_lock.py hold BOARD [BOARD...] --reason "why"
# ... hardware work ...
python3 test/hil/board_lock.py release BOARD [BOARD...]
```

- Never pre-hold boards you are about to run `hil_test.py` on — it self-locks and would treat your own hold as a conflict.
- Rig-wide operations (uhubctl power cycling, pci-rebind — bus renumbering) affect every board: `board_lock.py hold --all --reason "..."` first.
- `board_lock.py status` lists holders. Locks auto-release when the holder process dies (kernel flock); `/tmp` clears on reboot.
- Forcing past a lock: `HIL_NO_BOARD_LOCK=1 python3 test/hil/hil_test.py ...` bypasses the guard without killing the holder. Only with the user's explicit go-ahead — they accept the risk of colliding with whatever holds the board.
- Caveat until this branch merges to master: CI's checkout of `hil_test.py` does not yet enforce locks — keep dev hardware sessions short and check `gh run list --status in_progress` first.
````

- [x] **Step 2: Verify**

Run: `grep -n "svc.sh stop" .claude/skills/hil/SKILL.md; grep -c "board_lock.py" .claude/skills/hil/SKILL.md`
Expected: no `svc.sh stop` occurrences remain; `board_lock.py` appears ≥ 3 times.

- [x] **Step 3: Commit**

```bash
git add .claude/skills/hil/SKILL.md
git commit --no-verify -m "docs(hil): replace runner-stop procedure with per-board lock protocol"
```

---

## Plan Self-Review Notes

- Spec coverage: 5 agents (Tasks 1–5), 6 workflows incl. checker (Tasks 6–11), skill (Task 12), board-lock protocol (Tasks 19–21, executed after Task 5), all 6 spec success criteria (Tasks 13–18; Task 16 also covers the lock protocol). Push-authorization exception encoded in pr-babysit meta + plan interfaces.
- Deviations from spec, both deliberate: (1) `hil-validate.js` takes only `{boards}` — `buildDir` dropped, the hil skill hardcodes `-B examples`; (2) smoke test 15 uses `question` mode to bound cost instead of full-dimension review.
- Task 20 intentionally stops short of exact insertion-point code: the guard function is verbatim, but the wrap must match `hil_test.py`'s per-board failure convention, which the implementer reads first (blind exact code would be wrong).
- Type consistency: DEV/BUILD/FINDINGS/STAGE/OP schemas match the producing agents' output contracts verbatim; `agentType` strings match agent `name:` frontmatter.
