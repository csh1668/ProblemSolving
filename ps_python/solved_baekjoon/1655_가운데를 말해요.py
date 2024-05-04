import sys
from heapq import heappush, heappop

n = int(sys.stdin.readline())
mid = int(sys.stdin.readline())
print(mid)
max_q, min_q = [], []
for _ in range(1, n):
    k = int(sys.stdin.readline())
    # left = max_q[0]
    # right = min_q[0]
    if mid < k:
        heappush(min_q, k)
    elif mid >= k:
        heappush(max_q, -k)
    if len(max_q) + 1 < len(min_q):
        heappush(max_q, -mid)
        mid = heappop(min_q)
    if len(max_q) > len(min_q):
        heappush(min_q, mid)
        mid = -heappop(max_q)
    print(mid)