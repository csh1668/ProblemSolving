from heapq import heappop, heappush, heapify

n = int(input())
arr = list(map(int, input().split()))

# naive dp 풀이
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
# 함수 개형을 이용한 최적화
ans = 0
pq = [-arr[0]] # Max heap
for i in range(1, n):
    left_end = -pq[0] + i
    if arr[i] <= left_end:
        ans += left_end - arr[i]
        heappop(pq)
        heappush(pq, -(arr[i] - i))
    heappush(pq, -(arr[i] - i))
print(ans)