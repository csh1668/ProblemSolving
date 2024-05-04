import sys
from heapq import heappop, heappush

N, L = map(int, input().split())
arr = list(map(int, sys.stdin.readline().split()))
q = []
for idx, a in enumerate(arr):
    heappush(q, (a, idx))
    while True:
        min_val, min_idx = q[0]
        if idx - L >= min_idx:
            heappop(q)
        else:
            break
    print(q[0][0], end=' ')