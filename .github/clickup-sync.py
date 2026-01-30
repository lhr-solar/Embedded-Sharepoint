#!/usr/bin/env python3
import os
import sys
import json
import re
import requests
from github import Github
from time import sleep

# ============================================================================
# CONFIG
# ============================================================================

CLICKUP_TOKEN = os.environ["CLICKUP_API_TOKEN"]
CLICKUP_LIST_ID = os.environ["CLICKUP_LIST_ID"]
GITHUB_TOKEN = os.environ["GITHUB_TOKEN"]
GITHUB_REPO = os.environ["GITHUB_REPOSITORY"]
EVENT_NAME = os.environ["GITHUB_EVENT_NAME"]

CLICKUP_API = "https://api.clickup.com/api/v2"
GITHUB_API = "https://api.github.com"

gh = Github(GITHUB_TOKEN)
repo = gh.get_repo(GITHUB_REPO)

HEADERS = {
    "Authorization": CLICKUP_TOKEN,
    "Content-Type": "application/json",
}

# ============================================================================
# CLICKUP HELPERS
# ============================================================================

def clickup_request(method, endpoint, **kwargs):
    url = f"{CLICKUP_API}/{endpoint}"
    for attempt in range(3):
        r = requests.request(method, url, headers=HEADERS, **kwargs)
        if r.status_code == 429:
            sleep(60)
            continue
        r.raise_for_status()
        return r.json() if r.content else {}
    raise RuntimeError("ClickUp request failed")


def get_clickup_index():
    """issue_num -> {id, parent}"""
    tasks = clickup_request(
        "GET",
        f"list/{CLICKUP_LIST_ID}/task",
        params={"subtasks": True, "include_closed": True},
    ).get("tasks", [])

    index = {}
    for t in tasks:
        name = t.get("name", "")
        if "[Issue #" not in name:
            continue
        try:
            num = int(name.split("[Issue #")[1].split("]")[0])
        except Exception:
            continue

        index.setdefault(num, []).append({
            "id": t["id"],
            "parent": t.get("parent"),
        })

    return index


def resolve_task(issue_num, expected_parent, index):
    """
    Parent-safe task resolution.
    Returns task_id or None.
    """
    for entry in index.get(issue_num, []):
        if entry["parent"] == expected_parent:
            return entry["id"]
    return None


def create_task(issue_num, title, body, status, parent=None):
    payload = {
        "name": f"[Issue #{issue_num}] {title}",
        "description": f"{body or ''}\n\n---\n[GitHub Issue]({repo.get_issue(issue_num).html_url})",
        "status": status,
    }
    if parent:
        payload["parent"] = parent

    task = clickup_request("POST", f"list/{CLICKUP_LIST_ID}/task", json=payload)
    return task["id"]


def update_task(task_id, **fields):
    clickup_request("PUT", f"task/{task_id}", json=fields)


def get_dependencies(task_id):
    data = clickup_request("GET", f"task/{task_id}")
    return {d["task_id"] for d in data.get("dependencies", [])}


def add_dependency(task_id, depends_on):
    clickup_request(
        "POST",
        f"task/{task_id}/dependency",
        params={"depends_on": depends_on},
    )


def remove_dependency(task_id, depends_on):
    clickup_request(
        "DELETE",
        f"task/{task_id}/dependency",
        params={"depends_on": depends_on},
    )

# ============================================================================
# GITHUB RELATIONSHIPS
# ============================================================================

def get_blockers(issue_num):
    headers = {
        "Authorization": f"Bearer {GITHUB_TOKEN}",
        "Accept": "application/vnd.github+json",
    }

    blockers = []
    try:
        r = requests.get(
            f"{GITHUB_API}/repos/{GITHUB_REPO}/issues/{issue_num}/timeline",
            headers=headers,
        )
        r.raise_for_status()
        for e in r.json():
            if e.get("event") == "blocked_by_added":
                blockers.append(e["blocker"]["number"])
    except Exception:
        pass

    return blockers


def get_parent(issue):
    body = issue.body or ""
    m = re.search(r"[Pp]arent.*?#(\d+)", body)
    return int(m.group(1)) if m else None

# ============================================================================
# SYNC LOGIC
# ============================================================================

def sync_dependencies(issue_num, task_id, index):
    github_blockers = set(get_blockers(issue_num))
    clickup_deps = get_dependencies(task_id)

    expected_deps = set()
    for b in github_blockers:
        blocker_task = resolve_task(b, None, index)
        if blocker_task:
            expected_deps.add(blocker_task)

    # Add missing
    for dep in expected_deps - clickup_deps:
        print(f"  + Dependency: #{issue_num} depends on task {dep}")
        add_dependency(task_id, dep)

    # Remove stale
    for dep in clickup_deps - expected_deps:
        print(f"  - Removing stale dependency from #{issue_num}")
        remove_dependency(task_id, dep)


def sync_issue(issue_num, index):
    issue = repo.get_issue(issue_num)
    parent_num = get_parent(issue)

    parent_task_id = None
    if parent_num:
        parent_task_id = resolve_task(parent_num, None, index)

    status = (
        "done" if issue.state == "closed"
        else "blocked" if get_blockers(issue_num)
        else "todo"
    )

    task_id = resolve_task(issue_num, parent_task_id, index)

    if not task_id:
        print(f"Creating task for issue #{issue_num}")
        task_id = create_task(
            issue_num, issue.title, issue.body, status, parent_task_id
        )
        index.setdefault(issue_num, []).append({
            "id": task_id,
            "parent": parent_task_id,
        })
    else:
        update_task(task_id, status=status)

    sync_dependencies(issue_num, task_id, index)


def sync_pr(pr_num, index):
    pr = repo.get_pull(pr_num)
    linked = re.findall(r"(?:closes|fixes|resolves)\s+#(\d+)", pr.body or "", re.I)

    for issue_num in map(int, linked):
        sync_issue(issue_num, index)
        task_id = resolve_task(issue_num, None, index)
        if task_id:
            if pr.state == "closed" and pr.merged:
                update_task(task_id, status="done")

# ============================================================================
# MAIN
# ============================================================================

def main():
    event = json.loads(sys.argv[1])
    action = event.get("action")

    print(f"Event: {EVENT_NAME}.{action}")

    index = get_clickup_index()

    if EVENT_NAME == "issues":
        issue_num = event["issue"]["number"]
        sync_issue(issue_num, index)

    elif EVENT_NAME == "pull_request":
        pr_num = event["pull_request"]["number"]
        sync_pr(pr_num, index)

    print("✓ Sync complete")

if __name__ == "__main__":
    main()
