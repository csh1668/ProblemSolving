import sys

n, k = map(int, sys.stdin.readline().split())
v = [int(sys.stdin.readline()) for _ in range(n)]

# dp[i][j] i번째 동전까지 고려했을 때, 가치 j를 만들기 위한 최소 동전 개수
dp = [[sys.maxsize] * (k + 1) for _ in range(n)]
for i in range(n):
    dp[i][0] = 0
    for j in range(1, k + 1):
        if v[i] <= j:
            dp[i][j] = min(dp[i][j - v[i]] + 1, dp[i - 1][j])
        else:
            dp[i][j] = dp[i - 1][j]

res = min(map(lambda line: line[-1], dp))
print(res if res < sys.maxsize else -1)