from heapq import heappop, heappush, heapify

n = int(input())
arr = list(map(int, input().split()))

# naive한 dp 풀이
# MAX = 3000
#
# dp = [[0] * (MAX + 1) for _ in range(n)]
# for k in range(0, MAX + 1):
#     dp[0][k] = abs(arr[0] - k)
# for x in range(1, n):
#     for k in range(0, MAX + 1):
#         dp[x][k] = min(dp[x - 1][0:k + 1], default=MAX) + abs(arr[x] - k)
#
# print(min(dp[-1][1:]))

# 이해하고 제출해야겠죠..?
# # 그래프 개형을 이용한 풀이
ans = 0
q = [-arr[0]]
for i in range(1, n):
    left_end = -q[0] + i
    if arr[i] <= left_end:
        ans += left_end - arr[i]
        heappop(q)
        heappush(q, -(arr[i] - i))
    heappush(q, -(arr[i] - i))
print(ans)