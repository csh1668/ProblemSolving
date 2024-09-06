n = int(input())
arr = list(map(int, input().split()))

# dp[i][j][k]: i번째 수까지 고려했을 때, j개의 수를 골랐을 때의 합을 n으로 나눈 나머지가 k가 되는가
dp = [[0] * (n + 1) for _ in range(n << 1)]
dp[0][0] |= 1

for i in range(1, n << 1):
    dp[i][0] |= 1
    for j in range(1, n + 1):
        dp[i][j] = dp[i - 1][j]
        dp[i][j] |= (dp[i - 1][j - 1] << arr[i - 1]) % (1 << n)
        dp[i][j] |= (dp[i - 1][j - 1] >> (n - arr[i - 1])) % (1 << n)
        # for k in range(n):
        #     # dp[i][j][k] = dp[i - 1][j][k] or dp[i - 1][j - 1][(k - arr[i - 1] + n) % n]


# for i in range(n << 1):
#     print('current:', i)
#     for j in range(n + 1):
#         for k in range(n):
#             print('1' if dp[i][j][k] else '0', end=' ')
#         print()

i, j, k = (n << 1) - 1, n, 0
while i and j:
    if dp[i - 1][j - 1] & (1 << ((k - arr[i - 1] + n) % n)):
        print(arr[i - 1], end=' ')
        k = (k - arr[i - 1] + n) % n
        j -= 1
    i -= 1
print()