import sys
from collections import deque


class Node:
    def __init__(self, key):
        self.key = key
        self.children = {}
        self.fail = None
        self.is_end = False


root = Node('')
root.fail = root
n = int(sys.stdin.readline())
for _ in range(n):
    w = sys.stdin.readline().strip()
    cur = root
    for ch in w:
        if ch not in cur.children:
            cur.children[ch] = Node(ch)
        cur = cur.children[ch]
    cur.is_end = True

q = deque([root])
while q:
    cur = q.popleft()
    for ch, nxt in cur.children.items():
        if cur == root: nxt.fail = root
        else:
            dest = cur.fail
            while dest != root and ch not in dest.children:
                dest = dest.fail
            if ch in dest.children:
                dest = dest.children[ch]
            nxt.fail = dest
        if nxt.fail.is_end: nxt.output = True
        q.append(nxt)

m = int(sys.stdin.readline())
for _ in range(m):
    s = sys.stdin.readline().strip()
    cur = root
    ans = False
    for i, ch in enumerate(s):
        while cur != root and ch not in cur.children:
            cur = cur.fail
        if ch in cur.children:
            cur = cur.children[ch]
        if cur.is_end:
            ans = True
            break
    print('YES' if ans else 'NO')