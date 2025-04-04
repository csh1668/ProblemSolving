import sys


class Node:
    def __init__(self, key):
        self.key = key
        self.children = dict()
        self.is_end = False
        self.max_sz = 0

root = Node('')
n = int(sys.stdin.readline())
words = [sys.stdin.readline().strip() for _ in range(n)]
for s in words:
    cur = root
    for i, ch in enumerate(s):
        if ch not in cur.children:
            cur.children[ch] = Node(ch)
        cur = cur.children[ch]
        cur.max_sz = max(cur.max_sz, len(s))
    cur.is_end = True

inst = []

def dfs(cur, depth=0):
    if cur.is_end:
        inst.append('P')
    for ch, child in sorted(cur.children.items(), key=lambda x: x[1].max_sz):
        inst.append(ch)
        dfs(child, depth+1)
    inst.append('-')

dfs(root)
while inst[-1] == '-': inst.pop()
print(len(inst))
print('\n'.join(inst))