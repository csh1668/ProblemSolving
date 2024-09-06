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
left, right = [0] * (n + 1), [0] * (n + 1)
children = [[] for _ in range(n + 2)]
init_money = [0] * (n + 2)
for i in range(1, n + 1):
    inp = list(map(int, sys.stdin.readline().split()))
    if i == 1:
        init_money[i] = inp[0]
    else:
        init_money[i] = inp[0]
        children[inp[1]].append(i)

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


fenwick = Fenwick(n + 2)

for _ in range(m):
    op, *args = sys.stdin.readline().split()
    if op == 'p':
        a, b = map(int, args)
        if left[a] + 1 == right[a]: continue
        fenwick.update(left[a] + 1, b)
        fenwick.update(right[a], -b)
    else:
        a = int(args[0])
        print(fenwick.find_prefix_sum(left[a]) + init_money[a])