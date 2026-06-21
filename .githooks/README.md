# Git hooks

Repository-managed git hooks for Embedded-Sharepoint.

## `pre-push` — branch-name convention

Before a push is sent, every branch ref being pushed must match:

```
<type>/<desc>
```

- `type` ∈ `feat`, `fix`, `dev`, `chore`, `docs`, `refactor`, `test`
- `desc` = one or more **lowercase** letters, digits, `-`, `_`, `/`

`main`, `master`, and `HEAD` are always allowed. Tags and other non-branch
refs are ignored, and branch deletions are not checked.

Examples:

| Branch            | Result   |
| ----------------- | -------- |
| `feat/foo-bar`    | accepted |
| `fix/issue_12`    | accepted |
| `main`            | accepted |
| `mybranch`        | rejected |
| `Feature/Thing`   | rejected |

## Enabling

The hooks are opt-in via `core.hooksPath`. The repository setup script
(`install.sh`) runs this automatically, or you can enable it yourself:

```sh
git config core.hooksPath .githooks
```

## Bypassing in an emergency

```sh
git push --no-verify
```

This skips the `pre-push` hook for that single push.
