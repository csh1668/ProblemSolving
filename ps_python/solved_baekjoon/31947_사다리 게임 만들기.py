n, m = map(int, input().split())
s, e = map(int, input().split())

dp = [[0] * (m + 1) for _ in range(n)]
dp[e - 1][0] = 1
# print(dp)
# dp[i][j]: 가로선이 j개 생겼을 때 i가 우승할 확률
# n: 세로선
# m: 가로선
for j in range(1, m + 1):
    for i in range(n):
        # 양쪽 끝단일 경우
        if i == 0:
            # 나와 내 오른쪽에 가로선이 생기는 경우 + 나머지 경우
            dp[i][j] = 1/(n-1)*dp[i+1][j-1] + (n-2)/(n-1)*dp[i][j-1]
        elif i == n - 1:
            # 나와 내 왼쪽에 가로선이 생기는 경우 + 나머지 경우
            dp[i][j] = 1/(n-1)*dp[i-1][j-1] + (n-2)/(n-1)*dp[i][j-1]
        # 아닐 경우
        else:
            # 나와 내 왼쪽에 가로선이 생기는 경우 +
            # 나와 내 오른쪽에 가로선이 생기는 경우 +
            # 나머지 경우
            dp[i][j] = 1/(n-1)*dp[i+1][j-1] + 1/(n-1)*dp[i-1][j-1] + (n-3)/(n-1)*dp[i][j-1]
print(dp[s-1][-1])
# print(dp)