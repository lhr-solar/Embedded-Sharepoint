# Smoke Test Results — multi-agent dev/test harness

Date: 2026-07-09 (evening session)

## Task 13 — validate workflow (run wf_4f10863f-6ff, 2 boards)

| Stage | Result | Evidence |
|---|---|---|
| unit | PASS | ceedling 61/61, 2.72 s |
| size | PASS | stm32f407disco device/cdc_msc vs master: TOTAL 14167 B, +0.0% every file (`cmake-metrics/stm32f407disco/metrics_compare_device_cdc_msc.md`) |
| pvs | pass=false — **gate working as specified** | GA:1=20 / GA:2=36; flags 5 GA:1 in files changed vs local `master` (midi2_device.c:245 V547 branch-new upstream; usbd.c V763 x3 + usbh.c:2109 V1008 pre-existing lines in touched files). Root cause of "changed" set: local `master` lags `origin/master` (upstream MIDI2 merge), so upstream churn counts as changed. Logic per spec. |
| build:stm32f407disco | BLOCKED (env) | `agent type 'builder' not found` — see registry note below |
| build:raspberry_pi_pico | BLOCKED (env) | same |

Resume: `Workflow({scriptPath: '<worktree>/.claude/workflows/validate.js', resumeFromRunId: 'wf_4f10863f-6ff', args: {boards: [...]}})` — unit/size/pvs replay from cache.

## Task 15 — driver-review workflow (run wf_aa86dddc-11f) — PASS

- 2 dirs x 1 question (unbounded busy-waits), 22 agents (2 scanners + 20 verifiers), 0 errors, ~494k worker tokens.
- rusb2: 6 confirmed findings — the FRDY spin `dcd_rusb2.c:126` (matches the known hardware wedge) + CURPIPE spins (125, 249, 292, 477, ...) + CFIFO ISEL spin (347), each adversarially verified against code; one verifier corrected a scanner's claim that the Renesas manuals were missing from calibre (RA6M5/RX65N are present).
- Verification layer demonstrably filters and grades (severity/confidence preserved, refutation reasoning recorded).

## Harness facts discovered (affect all future sessions)

1. **Custom agent types register at session start, from the LAUNCH directory's `.claude/agents/`** — not the worktree's, and not on file changes mid-session. Runtime copies were mirrored to `/home/hathach/code/tinyusb/.claude/agents/` (untracked); a NEW session is required for builder/port-dev/hil-operator to resolve.
2. **Workflow `args` arrives as a JSON string** — all six scripts normalize with `if (typeof args === 'string') args = JSON.parse(args)`.
3. **Workflow-by-name resolution can serve a stale cached script** — invoke via `scriptPath` when iterating.

## Task 13 completion — PASS

Resumed run wf_4f10863f-6ff once agents registered: build:stm32f407disco 43 examples, build:raspberry_pi_pico 43 examples, both PASS; verdict mechanics correct (pass=false only from the pvs stage's stale-local-master base; re-run vs origin/master in Task 17 is fully green).

## Task 14 — fanout-dev — PASS (wf_d2fcc566-ebc)

2 port-dev workers (rp2040, stm32_fsdev): exactly 1-line diffs, `git clang-format` clean, both independently build-verified (`verifyBuild: true`), deps self-healed (get_deps stm32f0; PICO_SDK_PATH). Smoke edits reverted; tree clean.

## Task 16 — hil-validate + board locks — PASS mechanics (real rig)

- Lock-conflict run (wf_cab1ac1d-c4c): failed in 48 s, `detail` = `board locked: {holder JSON}` verbatim, `locked: ['raspberry_pi_pico']`, retry attempted, no flash.
- Force run (wf_d079a33e-dd8): flashed+tested with HIL_NO_BOARD_LOCK=1, holder pid survived (bypass, not theft), `locked: []`.
- Normal run (wf_87ec3c8f-e87): self-locking path live, board flashed/booted 18x, no lock messages.
- actions-runner `active` the entire time; svc.sh never touched.
- RIG FINDING (not this branch): pico PIO-USB host-port fixture devices (1a86_7523 CDC, 048d_04d2 MSC) not enumerating — 5 host-mode tests fail identically across runs; 13 device-mode tests all pass; firmware exonerated.

## Task 17 — /pre-pr end-to-end — PASS (wf_8a421ef1-fcf)

BASE=origin/master (local master stale). No C changes → minimal path: software-only, boards=[stm32f407disco]. Verdict `pass: true`: unit 61/61, 43 examples, size +0.0%, pvs green (no C diffs). HIL correctly skipped.

## Task 18 — pr-babysit dry + pr-monitor triage — launched

pr-babysit {pr: 3761, maxCycles: 1, autoPush: false} (wf_361c9e0a-d0e) + direct pr-monitor triage of PR 3750 (3 Copilot rounds) from a /tmp checkout of the PR head. Results recorded when complete.

## Post-smoke revisions

- Model tiering (owner): builder→haiku, hil-operator/pr-monitor→sonnet, unit/size→haiku, pvs/push/replies→sonnet; port-dev/driver-reviewer stay opus xhigh.
- driver-reviewer now checks MCU errata sheets; missing erratum workarounds are findings.
- Remaining: Task 18 verdicts, final whole-branch review, memory note update for the lock protocol.

Stop-gate extra (done this session): board_lock `cmd_hold` holder-signaled success via pipe (c326eaacc), storm-tested 10/10 exactly-one-winner.

## Post-review fixes (2026-07-10, owner-confirmed batch)

- board_lock: holder daemon detaches stdio (captured `hold` returned in 30 ms; pre-fix hung on the inherited pipe); `is_locked()` re-done as pid-liveness probe — never touches the flock, so status/pre-check storms can no longer fail a concurrent acquirer (0/15 failures under a 300-probe storm; hold storm still 1-winner-in-10).
- hil_test: locked board now emits a visible `board-locked` ❌ report row (report matches exit code); `accumulate_report` clears the stale marker once the board runs for real (3-scenario logic test green). failed-tests stays empty so re-runs repeat the whole board.
- pr-babysit: `autoPush` now opt-in (default dry run; `autoPush: true` is the explicit push/comment authorization); RESOLVE_RECIPE paginates reviewThreads (`pageInfo` + cursor); post-push resolve step gained the same issue-comment 404 fallback as the refuted-replies step.
- validate: size stage passes `--base-branch <base>`; pvs stage now calls the new `static-analyzer` agent (sonnet, structured `{pass, ga1, ga2, changedFindings[], detail}`).
- NEW agent `static-analyzer` (PVS-Studio SAST+MISRA, read-only) — mirrored to the launch-dir registry; registers next session (harness fact 1), so the validate pvs stage is unsmokable until then.

## Max-effort review fixes (2026-07-13, /code-review opus max: 10 finders → 28 verifiers → sweep)

- validate.js size stage: dropped the stray trailing `.` that made metrics_compare_base.py exit 2 on every run (regression from the 2026-07-10 batch).
- board_lock: flock is now the sole authority — cmd_hold's pid-liveness pre-gate removed (a live-but-moved-on hil_test.py worker pid no longer blocks a free board; verified: hold succeeds over a stale live-pid record, storm still 1-winner-in-10). cmd_release probes the flock before acting: free → clear stale record only; held by `hil_test.py` → refuse (CI mid-test, holder survives — verified); held otherwise → SIGTERM with PermissionError handled. Holder daemon truncates records on SIGTERM; success pipe dup'd above fd 2 (closed-stdio hold now succeeds — was orphan-holder + false failure, repro'd both ways).
- hil_test: lock record truncated on per-board release (pool workers outlive flocks); fail-open on OSError now prints a warning; unknown `-b` names exit 1 instead of a silent zero-test green (was exploitable as a false HIL pass through hil-validate); accumulate_report deletes an emptied board row (variant boards no longer leave a blank ghost row — 4-scenario test green).
- port-dev.md: `git clang-format -- <files>` scoped to the worker's own files (bare invocation reformatted concurrent siblings' edits in shared checkouts).
- pr-babysit: reply skeleton factored into postReplyRecipe (the two copies had already drifted once); cross-cycle repliedIds dedup (issue-comment refutations were re-posted every cycle); args.pr integer + maxCycles >= 1 validation.
- All 6 workflows: JSON.parse(args) wrapped so a non-JSON string hits the friendly shape error; driver-review throws on empty dimensions ([] is truthy); hil-validate dead length-clause dropped; retry instruction now `-v -r 1` (diagnosis, not 3 more flake-retries).
- Plan doc header replaced with a DO-NOT-EXECUTE historical banner + all 74 boxes checked (re-execution would have recreated pre-static-analyzer files with hooks disabled).
- Refuted by verification (left as-is by design): dry-run verify spawns (consumed via history), per-finding verifiers, 4-dim scanners, fanout double-build, validate parallel triple-compile, release --all / hold --config, board_lock import into hil_test (hil_ci.sh ships hil_test.py alone), lock exit code (JSON sidecar already carries board-locked), check.sh scope, effort scatter (harness-forced), local fcntl import (Windows guard).
