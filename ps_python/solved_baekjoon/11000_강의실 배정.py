import sys
from heapq import heappop, heappush

n = int(input())
arr = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
arr.sort()

res = 0
q = []
for i, j in arr:
    # 저장되어 있는 끝나는 시간이 현재 시작 시간보다 작거나 같을 때까지
    while q and q[0] <= i:
        heappop(q)
    heappush(q, j)
    res = max(res, len(q))
print(res)