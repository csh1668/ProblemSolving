from heapq import heappush, heappop, heapify
import sys

n, k = map(int, sys.stdin.readline().split())
a = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
b = [int(sys.stdin.readline()) for _ in range(k)]

heapify(a)
b.sort()

ans = 0

packable = list()
for i in range(k):
    while len(a) > 0 and b[i] >= a[0][0]:
        heappush(packable, -heappop(a)[1])
    if len(packable) > 0:
        ans += -heappop(packable)
    elif len(a) == 0:
        break

print(ans)