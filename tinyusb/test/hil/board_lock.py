#!/usr/bin/env python3
"""Per-board advisory locks for the HIL rig.

Arbitrates board access between dev sessions and CI's hil_test.py without
stopping the actions-runner. Locks are kernel flocks: the kernel releases
them automatically when the holder process dies, and holders clear their
lock-file record on release so records stay truthful (/tmp also clears on
reboot).

Usage:
  board_lock.py hold BOARD [BOARD...] --reason TEXT
  board_lock.py hold --all [--config CONFIG.json] --reason TEXT
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
    """True if the recorded holder process is still alive.

    Deliberately never touches the flock: even a momentary probe lock would
    make a concurrent acquirer's LOCK_NB attempt fail spuriously. The flock
    taken by acquirers themselves stays the only authority."""
    info = read_info(board)
    pid = info.get('pid') if isinstance(info, dict) else None
    if not isinstance(pid, int) or pid <= 0:
        return False
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True  # alive but owned by another user (e.g. the CI runner)
    return True


def cmd_hold(boards, reason):
    os.makedirs(LOCK_DIR, exist_ok=True)
    # No pre-check: the holder's own LOCK_NB flock is the only authority — a
    # recorded pid may be stale or recycled (e.g. a live hil_test.py worker
    # that already released this board's flock but not its record).
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
        for b in boards:
            info = read_info(b)
            if info:
                print(f'ERROR: {b} locked: {info}', file=sys.stderr)
        print('ERROR: holder failed to acquire locks', file=sys.stderr)
        return 1
    # intermediate child: detach, then spawn the actual holder
    os.setsid()
    if os.fork() > 0:
        os._exit(0)
    # holder (grandchild): acquire all flocks, signal the parent, sleep until killed
    os.close(r_fd)
    # Keep the success pipe clear of fds 0-2: invoked with stdio closed,
    # os.pipe() can land there and the dup2 loop below would clobber it.
    if w_fd <= 2:
        w_fd = fcntl.fcntl(w_fd, fcntl.F_DUPFD, 3)
    # Detach stdio: a `hold` whose output is captured must see EOF when the
    # front-end exits — the immortal holder must not keep that pipe open.
    devnull = os.open(os.devnull, os.O_RDWR)
    for std_fd in (0, 1, 2):
        os.dup2(devnull, std_fd)
    if devnull > 2:
        os.close(devnull)
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

    def _bow_out(*_):
        # clear the records before dying so read_info/status stay truthful
        # (the kernel drops the flocks themselves on exit either way)
        for h in handles:
            try:
                h.truncate(0)
            except OSError:
                pass
        os._exit(0)

    signal.signal(signal.SIGTERM, _bow_out)
    while True:
        signal.pause()


def cmd_release(boards):
    rc = 0
    victims = set()
    for b in boards:
        try:
            fd = os.open(lock_path(b), os.O_RDWR)
        except OSError:
            continue  # no lock file (or another user's): nothing we can release
        fh = os.fdopen(fd, 'r+')
        try:
            fcntl.flock(fh, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except OSError:
            # flock genuinely held — never SIGTERM on a mere pid record: the
            # pid may be recycled, or a live worker that already moved on.
            fh.close()
            info = read_info(b) or {}
            pid = info.get('pid')
            if info.get('reason') == 'hil_test.py':
                print(f'ERROR: {b} is mid-test by hil_test.py (pid {pid}) — not killing a '
                      'CI run; wait for it to finish', file=sys.stderr)
                rc = 1
            elif isinstance(pid, int) and pid > 0:
                victims.add(pid)
            else:
                print(f'ERROR: {b} is held but its record is unreadable', file=sys.stderr)
                rc = 1
            continue
        # flock was free: only a stale record remained — clear it
        try:
            fh.truncate(0)
        except OSError:
            pass
        fh.close()
    for holder in sorted(victims):
        try:
            os.kill(holder, signal.SIGTERM)
            print(f'released holder pid {holder}')
        except ProcessLookupError:
            pass
        except PermissionError:
            print(f'ERROR: holder pid {holder} belongs to another user — cannot signal it',
                  file=sys.stderr)
            rc = 1
    time.sleep(0.3)
    still = [b for b in boards if is_locked(b)]
    if still:
        print(f'ERROR: still locked: {", ".join(still)}', file=sys.stderr)
        return 1
    return rc


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
    p_hold.add_argument('--config',
                        default=os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                             'tinyusb.json'),
                        help='board roster JSON (default: tinyusb.json beside this script)')
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
