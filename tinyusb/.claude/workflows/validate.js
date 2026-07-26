export const meta = {
  name: 'validate',
  description: 'Pre-PR software validation: unit tests + per-board build sweeps + code-size compare + PVS, in parallel, joined into one verdict',
  whenToUse: 'Before opening or updating a PR, after any non-trivial change',
  phases: [{ title: 'Validate', detail: 'unit + builds + size + pvs in parallel' }],
}

// args: { boards: string[], examples?: string, base?: string, skip?: ('unit'|'size'|'pvs')[] }
if (typeof args === 'string') { try { args = JSON.parse(args) } catch { /* not JSON: shape check below reports it */ } }
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
const PVS = {
  type: 'object', additionalProperties: false,
  required: ['pass', 'ga1', 'ga2', 'changedFindings', 'detail'],
  properties: {
    pass: { type: 'boolean' }, ga1: { type: 'integer' }, ga2: { type: 'integer' },
    changedFindings: {
      type: 'array',
      items: {
        type: 'object', additionalProperties: false,
        required: ['file', 'line', 'rule', 'level', 'message'],
        properties: {
          file: { type: 'string' }, line: { type: 'integer' }, rule: { type: 'string' },
          level: { type: 'integer' }, message: { type: 'string' },
        },
      },
    },
    detail: { type: 'string' },
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
    `Compare TinyUSB code size against ${base}: python3 tools/metrics_compare_base.py --base-branch ${base} -b ${args.boards[0]} -e device/cdc_msc (exactly this command — no extra positional args). ` +
    'The report lands in cmake-metrics/<board>/metrics_compare.md. pass=false only if the tool itself errors; ' +
    'detail = the flash/RAM delta summary from the report (mention any example that grew).',
    { label: 'size', phase: 'Validate', model: 'haiku', schema: STAGE },
  ).then(r => r && { stage: 'size', ...r }))

if (!skip.includes('pvs')) thunks.push(() =>
  agent(
    `Run PVS-Studio static analysis for board ${args.boards[0]}, gating on files changed vs ${base}. ` +
    'Parallel build agents are running — use your dedicated build dir, never cmake-build-<board>.',
    { label: 'pvs', phase: 'Validate', agentType: 'static-analyzer', effort: 'low', schema: PVS },
  ).then(r => r && {
    stage: 'pvs', pass: r.pass,
    detail: r.pass ? r.detail : clip(`${r.detail} ${JSON.stringify(r.changedFindings)}`),
  }))

const results = (await parallel(thunks)).filter(Boolean)
const dead = thunks.length - results.length
if (dead > 0) log(`${dead} stage agent(s) died — counted as failures`)
const failures = results.filter(r => !r.pass)
log(`${results.length}/${thunks.length} stages completed, ${failures.length} failing`)
return { pass: failures.length === 0 && dead === 0, stages: results, failures }
