import sys
from heapq import heappop, heappush

n = int(sys.stdin.readline())
arr = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
arr.sort()
q = []

# print(arr)
for (a, b) in arr:
    heappush(q, b)
    if a < len(q):
        heappop(q)
print(sum(q))