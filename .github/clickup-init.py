import os
import requests
from datetime import datetime

# =========================
# Configuration (ENV VARS)
# =========================

GITHUB_REPO = os.environ["GITHUB_REPO"]
GITHUB_TOKEN = os.environ["GITHUB_TOKEN"]
CLICKUP_API_TOKEN = os.environ["CLICKUP_API_TOKEN"]
CLICKUP_LIST_ID = os.environ["CLICKUP_LIST_ID"]

# =========================
# Headers
# =========================

clickup_headers = {
    "Authorization": CLICKUP_API_TOKEN,
    "Content-Type": "application/json",
}

github_headers = {
    "Authorization": f"Bearer {GITHUB_TOKEN}",
    "Accept": "application/vnd.github+json",
    "X-GitHub-Api-Version": "2022-11-28",
}

# =========================
# ClickUp helpers
# =========================

def find_task_by_issue_number(issue_number: int) -> str | None:
    url = f"https://api.clickup.com/api/v2/list/{CLICKUP_LIST_ID}/task"
    params = {"subtasks": "true", "include_closed": "true"}

    response = requests.get(url, headers=clickup_headers, params=params)
    if response.status_code != 200:
        print(f"! Error fetching tasks: {response.text}")
        return None

    for task in response.json().get("tasks", []):
        if f"[Issue #{issue_number}]" in task.get("name", ""):
            return task["id"]

    return None


def create_task(
    issue_number: int,
    title: str,
    body: str,
    url: str,
    status: str,
    parent_task_id: str | None = None,
) -> str | None:

    clickup_url = f"https://api.clickup.com/api/v2/list/{CLICKUP_LIST_ID}/task"

    task_data = {
        "name": f"[Issue #{issue_number}] {title}",
        "description": f"GitHub Issue: {url}\n\n{body}",
        "status": status,
    }

    if parent_task_id:
        task_data["parent"] = parent_task_id

    response = requests.post(clickup_url, json=task_data, headers=clickup_headers)

    if response.status_code == 200:
        task_id = response.json()["id"]
        print(
            f"  Created {'subtask' if parent_task_id else 'task'} "
            f"for issue #{issue_number} ({status})"
        )
        return task_id

    print(f"! Failed creating task for #{issue_number}: {response.text}")
    return None


def add_comment_to_task(task_id: str, comment_text: str) -> bool:
    url = f"https://api.clickup.com/api/v2/task/{task_id}/comment"
    response = requests.post(
        url, json={"comment_text": comment_text}, headers=clickup_headers
    )
    return response.status_code == 200


def set_task_dependency(task_id: str, depends_on_task_id: str) -> bool:
    url = f"https://api.clickup.com/api/v2/task/{task_id}/dependency"
    response = requests.post(
        url, params={"depends_on": depends_on_task_id}, headers=clickup_headers
    )

    return response.status_code == 200 or "already exists" in response.text.lower()

# =========================
# GitHub helpers
# =========================

def get_blocked_by(issue_number: int) -> list[int]:
    url = f"https://api.github.com/repos/{GITHUB_REPO}/issues/{issue_number}/dependencies/blocked_by"
    response = requests.get(url, headers=github_headers)

    if response.status_code != 200:
        print(f"! Error fetching blocked_by for #{issue_number}: {response.text}")
        return []

    blockers = []
    for issue in response.json():
        blockers.append(issue["number"])

    return blockers


def get_all_issues_with_relationships():
    print(f"Fetching issues from {GITHUB_REPO}...")

    all_issues = []
    page = 1

    while True:
        response = requests.get(
            f"https://api.github.com/repos/{GITHUB_REPO}/issues",
            headers=github_headers,
            params={"state": "all", "per_page": 100, "page": page},
        )

        if response.status_code != 200:
            raise RuntimeError(response.text)

        batch = response.json()
        if not batch:
            break

        all_issues.extend(batch)
        page += 1

    all_issues = [i for i in all_issues if "pull_request" not in i]
    print(f"Found {len(all_issues)} issues")

    issue_map = {}
    parent_map = {}

    for issue in all_issues:
        issue_number = issue["number"]
        deps = issue.get("issue_dependencies_summary") or {}

        issue_map[issue_number] = {
            "issue": issue,
            "sub_issues": [],
            "blocked_by": deps.get("blocked_by", 0),
            "total_blocked_by": deps.get("total_blocked_by", 0),
        }

        parent_url = issue.get("parent_issue_url")
        if parent_url:
            parent_map[issue_number] = int(parent_url.split("/")[-1])

    for child, parent in parent_map.items():
        if parent in issue_map:
            issue_map[parent]["sub_issues"].append(child)

    return all_issues, issue_map, parent_map

# =========================
# Main
# =========================

def main():
    print("=" * 60)
    print("GitHub → ClickUp Import (manual)")
    print("=" * 60)

    all_issues, issue_map, parent_map = get_all_issues_with_relationships()

    task_map = {}
    blocked_issues = []

    print("\nCreating top-level tasks...\n")

    for issue_number, data in sorted(issue_map.items()):
        if issue_number in parent_map:
            continue

        existing = find_task_by_issue_number(issue_number)
        if existing:
            task_map[issue_number] = existing
            continue

        issue = data["issue"]

        if issue["state"] == "closed":
            status = "done"
        elif data["total_blocked_by"] > 0:
            status = "blocked"
            blocked_issues.append(issue_number)
        else:
            status = "todo"

        task_id = create_task(
            issue_number,
            issue["title"],
            issue.get("body", ""),
            issue["html_url"],
            status,
        )

        if task_id:
            task_map[issue_number] = task_id

    print("\nCreating subtasks...\n")

    for child, parent in parent_map.items():
        if child in task_map:
            continue

        parent_task_id = task_map.get(parent)
        if not parent_task_id:
            continue

        issue = issue_map[child]["issue"]
        status = "done" if issue["state"] == "closed" else "todo"

        task_id = create_task(
            child,
            issue["title"],
            issue.get("body", ""),
            issue["html_url"],
            status,
            parent_task_id,
        )

        if task_id:
            task_map[child] = task_id

    print("\nSetting dependencies...\n")

    for issue_number in blocked_issues:
        task_id = task_map.get(issue_number)
        if not task_id:
            continue

        for blocker in get_blocked_by(issue_number):
            blocker_task_id = task_map.get(blocker)
            if blocker_task_id:
                set_task_dependency(task_id, blocker_task_id)

    print("\nImport complete! Jolly good!\n")

if __name__ == "__main__":
    main()
