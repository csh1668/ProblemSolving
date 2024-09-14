import sys
from bisect import bisect_left

n = int(sys.stdin.readline())
a = sorted(tuple(map(int, sys.stdin.readline().split())) for _ in range(n))

dp = [a[0][1]]
pos = [-1] * n
pos[0] = 0
for i in range(n):
    if a[i][1] > dp[-1]:
        dp.append(a[i][1])
        pos[i] = len(dp) - 1
    else:
        idx = bisect_left(dp, a[i][1])
        dp[idx] = a[i][1]
        pos[i] = idx

print(n - len(dp))
mark, cur = [False] * n, len(dp) - 1
for i in range(n - 1, -1, -1):
    if pos[i] == cur:
        mark[i] = True
        cur -= 1
    if cur == -1: break

print(*[a[i][0] for i in range(n) if not mark[i]])