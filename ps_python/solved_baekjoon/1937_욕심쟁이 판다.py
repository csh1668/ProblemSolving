n = int(input())
arr = [list(map(int, input().split())) for _ in range(n)]
dp = [[-1] * n for _ in range(n)]


def dfs(x, y, depths=1):
    res = 0
    for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
        nx, ny = x + dx, y + dy
        if 0 <= nx < n and 0 <= ny < n and arr[nx][ny] > arr[x][y]:
            if dp[nx][ny] == -1:
                dp[nx][ny] = dp[x][y]
                dp[nx][ny] += dfs(nx, ny, depths + 1)
            res = max(res, dp[nx][ny])
    return res


ans = 0
for i in range(n):
    for j in range(n):
        if dp[i][j] == -1:
            dp[i][j] = 1
            dp[i][j] += dfs(i, j)
        ans = max(ans, dp[i][j])

print(ans)