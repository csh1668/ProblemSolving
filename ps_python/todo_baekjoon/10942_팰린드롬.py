import sys

n = int(sys.stdin.readline())
arr = list(map(int, sys.stdin.readline().split()))
m = int(sys.stdin.readline())

dp = [[True if i == j else False for j in range(n)] for i in range(n)]
for i in range(1, n):
    for j in range(i):
        if (dp[i - 1][j + 1] or dp[i - 1][j]) and arr[j] == arr[i]:
            dp[i][j] = True

for _ in range(m):
    s, e = map(int, sys.stdin.readline().split())
    print('1' if dp[e - 1][s - 1] else '0')