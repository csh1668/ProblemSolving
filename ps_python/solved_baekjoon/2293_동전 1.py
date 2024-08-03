import sys

n, k = map(int, sys.stdin.readline().split())
v = [int(sys.stdin.readline()) for _ in range(n)]

dp = [0] * (k + 1)
dp[0] = 1
for e in v:
    for i in range(e, k + 1):
        dp[i] += dp[i - e]
    # print(dp)
print(dp[k])