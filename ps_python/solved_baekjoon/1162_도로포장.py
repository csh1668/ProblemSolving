import sys
from heapq import heappop, heappush

n, m, k = map(int, sys.stdin.readline().split())
g = [dict() for _ in range(n + 1)]
for _ in range(m):
    u, v, w = map(int, sys.stdin.readline().split())
    if g[u].get(v) is None or g[u][v] > w:
        g[u][v] = w
        g[v][u] = w

dists = [[sys.maxsize] * (k + 1) for _ in range(n + 1)]
dists[1] = [0] * (k + 1)
pq = [(0, 1, 0)]
while pq:
    d, u, cnt = heappop(pq)
    if dists[u][cnt] < d: continue
    for v, w in g[u].items():
        if dists[v][cnt] > d + w:
            dists[v][cnt] = d + w
            heappush(pq, (d + w, v, cnt))
        if cnt < k and dists[v][cnt + 1] > d:
            dists[v][cnt + 1] = d
            heappush(pq, (d, v, cnt + 1))

print(min(dists[n]))