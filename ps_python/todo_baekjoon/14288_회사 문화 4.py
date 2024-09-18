import sys
from collections import deque


class Fenwick:
    def __init__(self, n):
        self.n = n
        self.tree = [0] * (n + 1)

    def update(self, i, d):
        while i <= self.n:
            self.tree[i] += d
            i += i & -i

    def prefix(self, i):
        ret = 0
        while i > 0:
            ret += self.tree[i]
            i -= i & -i
        return ret

    def query(self, l, r):
        return self.prefix(r) - self.prefix(l - 1)


n, m = map(int, sys.stdin.readline().split())
s, e = [0] * (n + 1), [0] * (n + 1)
c = [[] for _ in range(n + 1)]
for i, v in enumerate(map(int, sys.stdin.readline().split()[1:])):
    c[v].append(i + 2)

stack = deque([(1, 0), (1, 1)], maxlen=n << 1 | 1)
cur = 1
while stack:
    i, mode = stack.pop()
    if mode == 0: e[i] = cur
    else:
        s[i] = cur
        cur += 1
        for child in reversed(c[i]):
            stack.append((child, 0))
            stack.append((child, 1))

fw = Fenwick(n + 2)
dir = 0
for _ in range(m):
    op, *args = map(int, sys.stdin.readline().split())
    if op == 1:
        i, w = args
        if dir == 0:
            fw.update(s[i], w)
            fw.update(e[i], -w)
        else:
            fw.update(s[i], w)
    elif op == 2:
        i = args[0]
        print(fw.query(1, s[i]), fw.query(s[i], e[i] - 1), fw.query(s[i], s[i]))
    else:
        dir ^= 1