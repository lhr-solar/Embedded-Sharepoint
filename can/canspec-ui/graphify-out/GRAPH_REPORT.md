# Graph Report - canspec-ui  (2026-06-19)

## Corpus Check
- 21 files · ~4,320 words
- Verdict: corpus is large enough that graph structure adds value.

## Summary
- 149 nodes · 205 edges · 12 communities (11 shown, 1 thin omitted)
- Extraction: 100% EXTRACTED · 0% INFERRED · 0% AMBIGUOUS
- Token cost: 0 input · 0 output

## Graph Freshness
- Built from commit: `4cee8f2c`
- Run `git rev-parse HEAD` and compare to check if the graph is stale.
- Run `graphify update .` after code changes (no API cost).

## Community Hubs (Navigation)
- [[_COMMUNITY_Community 0|Community 0]]
- [[_COMMUNITY_Community 1|Community 1]]
- [[_COMMUNITY_Community 2|Community 2]]
- [[_COMMUNITY_Community 3|Community 3]]
- [[_COMMUNITY_Community 4|Community 4]]
- [[_COMMUNITY_Community 5|Community 5]]
- [[_COMMUNITY_Community 6|Community 6]]
- [[_COMMUNITY_Community 7|Community 7]]
- [[_COMMUNITY_Community 8|Community 8]]
- [[_COMMUNITY_Community 9|Community 9]]

## God Nodes (most connected - your core abstractions)
1. `compilerOptions` - 19 edges
2. `compilerOptions` - 16 edges
3. `scripts` - 7 edges
4. `useLayoutOutlet()` - 7 edges
5. `CanMessage` - 5 edges
6. `mapChoices()` - 4 edges
7. `mapSignal()` - 4 edges
8. `findVehicle()` - 4 edges
9. `VehiclePage()` - 4 edges
10. `CanspecPayload` - 4 edges

## Surprising Connections (you probably didn't know these)
- `Layout()` --calls--> `useCanspecData()`  [EXTRACTED]
  src/Layout.tsx → src/useCanspecData.ts
- `VehiclePage()` --calls--> `findVehicle()`  [EXTRACTED]
  src/pages/VehiclePage.tsx → src/data.ts
- `Home()` --calls--> `useLayoutOutlet()`  [EXTRACTED]
  src/pages/Home.tsx → src/layoutContext.ts
- `NetworkPage()` --calls--> `useLayoutOutlet()`  [EXTRACTED]
  src/pages/NetworkPage.tsx → src/layoutContext.ts
- `VehiclePage()` --calls--> `useLayoutOutlet()`  [EXTRACTED]
  src/pages/VehiclePage.tsx → src/layoutContext.ts

## Import Cycles
- None detected.

## Communities (12 total, 1 thin omitted)

### Community 0 - "Community 0"
Cohesion: 0.14
Nodes (15): formatMessageId(), MessageCard(), MessageSort, NetworkExplorer(), frameIdSearchTokens(), IdSearchFormat, messageMatchesSearch(), CanMessage (+7 more)

### Community 1 - "Community 1"
Cohesion: 0.09
Nodes (21): dependencies, react, react-dom, react-router-dom, devDependencies, @types/node, @types/react, @types/react-dom (+13 more)

### Community 2 - "Community 2"
Cohesion: 0.10
Nodes (20): compilerOptions, allowImportingTsExtensions, jsx, lib, module, moduleDetection, moduleResolution, noEmit (+12 more)

### Community 3 - "Community 3"
Cohesion: 0.14
Nodes (17): assetUrl(), buildValueTableIndex(), dataTypeFrom(), fmtId(), loadCanspecData(), mapChoices(), mapConversion(), mapMessage() (+9 more)

### Community 4 - "Community 4"
Cohesion: 0.11
Nodes (17): compilerOptions, lib, module, moduleDetection, moduleResolution, noEmit, noFallthroughCasesInSwitch, noUncheckedSideEffectImports (+9 more)

### Community 5 - "Community 5"
Cohesion: 0.24
Nodes (9): Home(), NetworkPage(), Resolved, VehiclePage(), findNetwork(), findVehicle(), LayoutOutletContext, useLayoutOutlet() (+1 more)

### Community 6 - "Community 6"
Cohesion: 0.18
Nodes (12): canRoot, copyDoc(), examplesRoot, here, index, isV3Doc(), mdcDir, publicDir (+4 more)

### Community 7 - "Community 7"
Cohesion: 0.40
Nodes (4): here, index, indexPath, publicDir

### Community 8 - "Community 8"
Cohesion: 0.60
Nodes (3): Layout(), CanspecPayload, useCanspecData()

## Knowledge Gaps
- **79 isolated node(s):** `name`, `private`, `type`, `dev`, `copy-mdc` (+74 more)
  These have ≤1 connection - possible missing edges or undocumented components.
- **1 thin communities (<3 nodes) omitted from report** — run `graphify query` to explore isolated nodes.

## Suggested Questions
_Questions this graph is uniquely positioned to answer:_

- **What connects `name`, `private`, `type` to the rest of the system?**
  _79 weakly-connected nodes found - possible documentation gaps or missing edges._
- **Should `Community 0` be split into smaller, more focused modules?**
  _Cohesion score 0.14492753623188406 - nodes in this community are weakly interconnected._
- **Should `Community 1` be split into smaller, more focused modules?**
  _Cohesion score 0.09090909090909091 - nodes in this community are weakly interconnected._
- **Should `Community 2` be split into smaller, more focused modules?**
  _Cohesion score 0.09523809523809523 - nodes in this community are weakly interconnected._
- **Should `Community 3` be split into smaller, more focused modules?**
  _Cohesion score 0.14210526315789473 - nodes in this community are weakly interconnected._
- **Should `Community 4` be split into smaller, more focused modules?**
  _Cohesion score 0.1111111111111111 - nodes in this community are weakly interconnected._