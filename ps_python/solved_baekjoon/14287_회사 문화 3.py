import sys
from collections import deque


class Fenwick:
    def __init__(self, n):
        self.n = n
        self.fenwick = [0] * (n + 1)

    def find_prefix_sum(self, idx):
        ret = 0
        while idx:
            ret += self.fenwick[idx]
            idx -= idx & -idx
        return ret

    def find_sum(self, l, r):
        return self.find_prefix_sum(r) - self.find_prefix_sum(l - 1)

    def update(self, idx, val):
        while idx < self.n:
            self.fenwick[idx] += val
            idx += idx & -idx


n, m = map(int, sys.stdin.readline().split())
left, right = [0] * (n + 5), [0] * (n + 5)
children = [[] for _ in range(n + 5)]
for i, v in enumerate(map(int, sys.stdin.readline().split()[1:])):
    children[v].append(i + 2)

# 비재귀 오일러 투어 테크닉
# 현재 인덱스, 모드 (1: left, 0: right)
s = deque([(1, 0), (1, 1)], maxlen=n << 1 | 1)
cur = 1
while s:
    i, mode = s.pop()
    if mode == 0:
        right[i] = cur
    else:
        left[i] = cur
        cur += 1
        for child in reversed(children[i]):
            s.append((child, 0))
            s.append((child, 1))


fw = Fenwick(n + 5)
for _ in range(m):
    op, *args = map(int, sys.stdin.readline().split())
    if op == 1:
        fw.update(left[args[0]], args[1])
    else:
        print(fw.find_sum(left[args[0]], right[args[0]] - 1))