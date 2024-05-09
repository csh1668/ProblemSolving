dp = [0] * (30 + 1)
dp[2] = 3

for i in range(4, 31):
    if i & 1:
        continue
    dp[i] = dp[i - 2] * 3
    for j in range(i - 4, -1, -2):
        dp[i] += dp[j] * 2
    dp[i] += 2
print(dp[int(input())])