#!/usr/bin/env python3
"""Simple ClickUp-GitHub sync script using GitHub issue relationships. Just 2 files needed!"""
import os
import sys
import json
import re
import requests
from github import Github
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
gh = Github(GITHUB_TOKEN)
repo = gh.get_repo(GITHUB_REPO)

# ============================================================================
# CLICKUP API HELPERS
# ============================================================================
def clickup_request(method, endpoint, **kwargs):
    """Make request to ClickUp API with retry."""
    url = f"{CLICKUP_API}/{endpoint}"
    headers = {"Authorization": CLICKUP_TOKEN, "Content-Type": "application/json"}
    
    for attempt in range(3):
        try:
            resp = requests.request(method, url, headers=headers, **kwargs)
            if resp.status_code == 429:
                sleep(60)
                continue
            resp.raise_for_status()
            return resp.json() if resp.content else {}
        except Exception as e:
            if attempt == 2:
                raise
            sleep(2 ** attempt)

def get_task_by_issue(issue_num):
    """Find ClickUp task by GitHub issue number."""
    tasks = clickup_request("GET", f"list/{CLICKUP_LIST_ID}/task", params={"subtasks": True, "include_closed": True})
    for task in tasks.get("tasks", []):
        if task["name"].startswith(f"#{issue_num}:"):
            return task["id"]
    return None

def create_task(name, desc, status="todo", parent=None):
    """Create ClickUp task."""
    payload = {"name": name, "description": desc, "status": status}
    if parent:
        payload["parent"] = parent
    return clickup_request("POST", f"list/{CLICKUP_LIST_ID}/task", json=payload)

def update_task(task_id, **kwargs):
    """Update ClickUp task."""
    return clickup_request("PUT", f"task/{task_id}", json=kwargs)

def add_comment(task_id, text):
    """Add comment to task."""
    clickup_request("POST", f"task/{task_id}/comment", json={"comment_text": text})

# ============================================================================
# GITHUB RELATIONSHIP HELPERS
# ============================================================================
def get_linked_issues(issue_num):
    """Get all linked issues from GitHub's relationships section."""
    headers = {"Authorization": f"token {GITHUB_TOKEN}", "Accept": "application/vnd.github.v3+json"}
    
    # Get issue details
    issue_url = f"{GITHUB_API}/repos/{GITHUB_REPO}/issues/{issue_num}"
    resp = requests.get(issue_url, headers=headers)
    
    blocking = []
    blocked_by = []
    parent_issue = None
    
    try:
        # Get timeline for cross-referenced events (linked issues)
        timeline_url = f"{GITHUB_API}/repos/{GITHUB_REPO}/issues/{issue_num}/timeline"
        resp = requests.get(timeline_url, headers=headers)
        resp.raise_for_status()
        
        for event in resp.json():
            if event.get("event") == "cross-referenced":
                source = event.get("source", {})
                if source and source.get("type") == "issue":
                    source_issue = source.get("issue", {})
                    source_num = source_issue.get("number")
                    body = source_issue.get("body", "")
                    
                    # Check if source blocks this issue
                    if re.search(rf'[Bb]locks?.*?#{issue_num}\b', body or ""):
                        blocking.append(source_num)
                    
                    # Check if source is blocked by this issue
                    if re.search(rf'[Bb]locked by.*?#{issue_num}\b', body or ""):
                        blocked_by.append(source_num)
    except:
        pass
    
    # Parse body for parent and remaining relationships
    issue = repo.get_issue(issue_num)
    body = issue.body or ""
    
    # Parent relationship
    match = re.search(r'[Pp]arent.*?#(\d+)', body)
    if match:
        parent_issue = int(match.group(1))
    
    return blocking, blocked_by, parent_issue

def parse_linked_issues(body):
    """Extract linked issues from PR body."""
    matches = re.findall(r'(?:[Cc]loses?|[Ff]ixes?|[Rr]esolves?).*?#(\d+)', body or "")
    return [int(m) for m in matches]

# ============================================================================
# SYNC LOGIC
# ============================================================================
def sync_issue_to_task(issue_num):
    """Sync a GitHub issue to ClickUp task."""
    issue = repo.get_issue(issue_num)
    task_id = get_task_by_issue(issue_num)
    
    # Get relationships from GitHub
    blocking, blocked_by, parent_num = get_linked_issues(issue_num)
    
    # Determine status
    status = "to do"
    if blocked_by and any(repo.get_issue(b).state == "open" for b in blocked_by if b):
        status = "blocked"
    
    # Check for linked PRs
    has_pr = False
    try:
        timeline = issue.get_timeline()
        for event in timeline:
            if hasattr(event, 'source') and event.source and hasattr(event.source, 'issue'):
                if event.source.issue.pull_request:
                    has_pr = True
                    break
    except:
        pass
    
    if has_pr and status != "blocked":
        status = "in progress"
    
    # Get or create parent task
    parent_task_id = None
    if parent_num:
        parent_task_id = get_task_by_issue(parent_num)
        if not parent_task_id:
            sync_issue_to_task(parent_num)  # Recursive
            parent_task_id = get_task_by_issue(parent_num)
    
    # Create or update task
    task_name = f"#{issue_num}: {issue.title}"
    task_desc = f"{issue.body or ''}\n\n---\n[GitHub Issue]({issue.html_url})"
    
    if not task_id:
        print(f"Creating task for issue #{issue_num}")
        task = create_task(task_name, task_desc, status, parent_task_id)
        task_id = task["id"]
    else:
        print(f"Updating task for issue #{issue_num}")
        if issue.state == "closed":
            status = "done"
        update_task(task_id, name=task_name, description=task_desc, status=status)
    
    # Handle blocking relationships
    for blocked_num in blocking:
        blocked_task_id = get_task_by_issue(blocked_num)
        if not blocked_task_id:
            sync_issue_to_task(blocked_num)
            blocked_task_id = get_task_by_issue(blocked_num)
        if blocked_task_id:
            update_task(blocked_task_id, status="blocked")
    
    return task_id

def sync_pr_to_task(pr_num):
    """Sync PR info to related issue tasks."""
    pr = repo.get_pull(pr_num)
    linked = parse_linked_issues(pr.body)
    
    # Get last 3 commits
    commits = list(pr.get_commits())[-3:]
    commit_text = "\n".join([
        f"- `{c.sha[:7]}` {c.commit.message.split(chr(10))[0]} by {c.commit.author.name}"
        for c in commits
    ])
    
    for issue_num in linked:
        task_id = get_task_by_issue(issue_num)
        if not task_id:
            sync_issue_to_task(issue_num)
            task_id = get_task_by_issue(issue_num)
        
        if task_id:
            # Update status if not blocked
            _, blocked_by, _ = get_linked_issues(issue_num)
            is_blocked = any(repo.get_issue(b).state == "open" for b in blocked_by if b)
            
            if pr.state == "closed" and pr.merged:
                update_task(task_id, status="done")
                add_comment(task_id, f"✅ PR #{pr_num} merged: {pr.html_url}")
            elif not is_blocked:
                update_task(task_id, status="in progress")
                add_comment(task_id, f"🔗 PR #{pr_num}: {pr.html_url}\n\n**Recent commits:**\n{commit_text}")

def check_unblock(issue_num):
    """Check if issue can be unblocked."""
    _, blocked_by, _ = get_linked_issues(issue_num)
    
    # Check if all blockers are closed
    all_closed = all(repo.get_issue(b).state == "closed" for b in blocked_by if b)
    
    if all_closed:
        task_id = get_task_by_issue(issue_num)
        if task_id:
            # Check if has PR
            has_pr = False
            try:
                issue = repo.get_issue(issue_num)
                timeline = issue.get_timeline()
                for event in timeline:
                    if hasattr(event, 'source') and event.source and hasattr(event.source, 'issue'):
                        if event.source.issue.pull_request:
                            has_pr = True
                            break
            except:
                pass
            
            new_status = "in progress" if has_pr else "to do"
            update_task(task_id, status=new_status)
            add_comment(task_id, "✅ Unblocked - all dependencies resolved")

# ============================================================================
# MAIN EVENT HANDLER
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
            
            # Check if this unblocks any other issues
            all_issues = repo.get_issues(state="open")
            for issue in all_issues:
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
