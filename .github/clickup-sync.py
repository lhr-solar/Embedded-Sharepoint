#!/usr/bin/env python3
import os
import sys
import json
import re
import requests
from github import Github, Auth
from time import sleep

# ============================================================================
# CONFIGURATION
# ============================================================================
CLICKUP_TOKEN = os.environ['CLICKUP_API_TOKEN']
CLICKUP_LIST_ID = os.environ['CLICKUP_LIST_ID']
GITHUB_TOKEN = os.environ['GITHUB_TOKEN']
GITHUB_REPO = os.environ['GITHUB_REPOSITORY']
EVENT_NAME = os.environ['GITHUB_EVENT_NAME']

CLICKUP_API = "https://api.clickup.com/api/v2"
GITHUB_API = "https://api.github.com"

# Use the new PyGithub auth method
gh = Github(auth=Auth.Token(GITHUB_TOKEN))
repo = gh.get_repo(GITHUB_REPO)

# ============================================================================
# HELPERS
# ============================================================================

def safe_text(text: str) -> str:
    """Sanitize text for ClickUp API: ensure UTF-8 and replace invalid characters."""
    if not text:
        return ""
    return text.encode('utf-8', errors='replace').decode('utf-8')


def clickup_request(method, endpoint, **kwargs):
    """ClickUp API request with retries for rate-limiting."""
    url = f"{CLICKUP_API}/{endpoint}"
    headers = {"Authorization": CLICKUP_TOKEN, "Content-Type": "application/json"}

    for attempt in range(3):
        try:
            resp = requests.request(method, url, headers=headers, **kwargs)
            if resp.status_code == 429:  # rate limit
                sleep(60)
                continue
            resp.raise_for_status()
            return resp.json() if resp.content else {}
        except Exception as e:
            if attempt == 2:
                raise
            sleep(2 ** attempt)


# ----------------- ClickUp operations -----------------
def get_task_by_issue(issue_num):
    """Get ClickUp task by GitHub issue number."""
    tasks = clickup_request(
        "GET",
        f"list/{CLICKUP_LIST_ID}/task",
        params={"subtasks": True, "include_closed": True}
    )
    for task in tasks.get("tasks", []):
        if task["name"].startswith(f"#{issue_num}:"):
            return task["id"]
    return None


def create_task(name, desc, status="todo", parent=None):
    """Create ClickUp task safely."""
    payload = {
        "name": safe_text(name),
        "description": safe_text(desc),
        "status": status
    }
    if parent:
        payload["parent"] = parent
    return clickup_request("POST", f"list/{CLICKUP_LIST_ID}/task", json=payload)


def update_task(task_id, **kwargs):
    """Update ClickUp task."""
    return clickup_request("PUT", f"task/{task_id}", json=kwargs)


def add_comment(task_id, text):
    """Add comment to ClickUp task."""
    clickup_request("POST", f"task/{task_id}/comment", json={"comment_text": safe_text(text)})


# ----------------- GitHub relationship helpers -----------------
def get_linked_issues(issue_num):
    """Get relationships from GitHub (blocking, blocked_by, parent)."""
    headers = {"Authorization": f"token {GITHUB_TOKEN}", "Accept": "application/vnd.github.v3+json"}
    blocking = []
    blocked_by = []
    parent_issue = None

    try:
        timeline_url = f"{GITHUB_API}/repos/{GITHUB_REPO}/issues/{issue_num}/timeline"
        resp = requests.get(timeline_url, headers=headers)
        resp.raise_for_status()
        for event in resp.json():
            if event.get("event") == "cross-referenced":
                source = event.get("source", {})
                if source and source.get("type") == "issue":
                    src_issue = source.get("issue", {})
                    src_num = src_issue.get("number")
                    body = src_issue.get("body", "")
                    # Detect blocking
                    if re.search(rf'[Bb]locks?.*?#{issue_num}\b', body or ""):
                        blocking.append(src_num)
                    if re.search(rf'[Bb]locked by.*?#{issue_num}\b', body or ""):
                        blocked_by.append(src_num)
    except:
        pass

    # Parse body for parent issue
    try:
        issue = repo.get_issue(issue_num)
        match = re.search(r'[Pp]arent.*?#(\d+)', issue.body or "")
        if match:
            parent_issue = int(match.group(1))
    except:
        pass

    return blocking, blocked_by, parent_issue


def parse_linked_issues(body):
    """Extract linked issues from PR body."""
    matches = re.findall(r'(?:[Cc]loses?|[Ff]ixes?|[Rr]esolves?).*?#(\d+)', body or "")
    return [int(m) for m in matches]


# ----------------- Sync logic -----------------
def sync_issue_to_task(issue_num):
    """Sync a GitHub issue to ClickUp task, respecting dependencies and parents."""
    issue = repo.get_issue(issue_num)
    task_id = get_task_by_issue(issue_num)

    blocking, blocked_by, parent_num = get_linked_issues(issue_num)

    # Determine status
    status = "to do"
    if blocked_by and any(repo.get_issue(b).state == "open" for b in blocked_by if b):
        status = "blocked"

    # Handle parent task recursively
    parent_task_id = None
    if parent_num:
        parent_task_id = get_task_by_issue(parent_num)
        if not parent_task_id:
            sync_issue_to_task(parent_num)
            parent_task_id = get_task_by_issue(parent_num)

    # Task name/description
    task_name = f"#{issue_num}: {issue.title}"
    task_desc = f"{issue.body or ''}\n\n---\n[GitHub Issue]({issue.html_url})"

    # Create or update task
    if not task_id:
        print(f"Creating task for issue #{issue_num}")
        task = create_task(task_name, task_desc, status, parent_task_id)
        task_id = task["id"]
    else:
        print(f"Updating task for issue #{issue_num}")
        if issue.state == "closed":
            status = "done"
        update_task(task_id, name=task_name, description=task_desc, status=status)

    # Update tasks that are blocked by this issue
    for blocked_num in blocking:
        blocked_task_id = get_task_by_issue(blocked_num)
        if not blocked_task_id:
            sync_issue_to_task(blocked_num)
            blocked_task_id = get_task_by_issue(blocked_num)
        if blocked_task_id:
            update_task(blocked_task_id, status="blocked")
            add_comment(blocked_task_id, f"Blocked by issue #{issue_num}")

    return task_id


def sync_pr_to_task(pr_num):
    """Sync PR to linked issues."""
    pr = repo.get_pull(pr_num)
    linked_issues = parse_linked_issues(pr.body)

    commits = list(pr.get_commits())[-3:]
    commit_text = "\n".join([
        f"- `{c.sha[:7]}` {c.commit.message.split(chr(10))[0]} by {c.commit.author.name}"
        for c in commits
    ])

    for issue_num in linked_issues:
        task_id = get_task_by_issue(issue_num)
        if not task_id:
            sync_issue_to_task(issue_num)
            task_id = get_task_by_issue(issue_num)

        if task_id:
            _, blocked_by, _ = get_linked_issues(issue_num)
            is_blocked = any(repo.get_issue(b).state == "open" for b in blocked_by if b)
            if pr.state == "closed" and pr.merged:
                update_task(task_id, status="done")
                add_comment(task_id, f"✅ PR #{pr_num} merged: {pr.html_url}")
            elif not is_blocked:
                update_task(task_id, status="in progress")
                add_comment(task_id, f"🔗 PR #{pr_num}: {pr.html_url}\n\n**Recent commits:**\n{commit_text}")


def check_unblock(issue_num):
    """Unblock a task if all blockers are closed."""
    _, blocked_by, _ = get_linked_issues(issue_num)
    if all(repo.get_issue(b).state == "closed" for b in blocked_by if b):
        task_id = get_task_by_issue(issue_num)
        if task_id:
            issue = repo.get_issue(issue_num)
            has_pr = any([True for pr in issue.get_pulls()])  # Check for linked PR
            new_status = "in progress" if has_pr else "to do"
            update_task(task_id, status=new_status)
            add_comment(task_id, "✅ Unblocked - all dependencies resolved")


# ============================================================================
# MAIN
# ============================================================================
def main():
    event = json.loads(sys.argv[1])
    action = event.get("action")
    print(f"Event: {EVENT_NAME}.{action}")

    if EVENT_NAME == "issues":
        issue_num = event["issue"]["number"]

        if action in ["opened", "edited", "reopened"]:
            sync_issue_to_task(issue_num)
        elif action == "closed":
            task_id = get_task_by_issue(issue_num)
            if task_id:
                update_task(task_id, status="done")
            # Unblock dependent issues
            for issue in repo.get_issues(state="open"):
                _, blocked_by, _ = get_linked_issues(issue.number)
                if issue_num in blocked_by:
                    check_unblock(issue.number)

    elif EVENT_NAME == "pull_request":
        pr_num = event["pull_request"]["number"]
        if action in ["opened", "edited", "synchronize"]:
            sync_pr_to_task(pr_num)
        elif action == "closed":
            sync_pr_to_task(pr_num)

    print("✓ Sync complete")


if __name__ == "__main__":
    main()
