import sys
from heapq import heappop, heappush, heapify

n = int(input())
arr = [int(sys.stdin.readline()) for _ in range(n)]
heapify(arr)

res = 0
while arr:
    a = heappop(arr)
    if arr:
        b = heappop(arr)
        res += a + b
        heappush(arr, a + b)
    else:
        break
print(res)