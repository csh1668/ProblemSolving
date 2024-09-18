import sys
from heapq import heappop, heappush

n, m = map(int, sys.stdin.readline().split())
g = [dict() for _ in range(n + 1)]
for _ in range(m):
    a, b, c = map(int, sys.stdin.readline().split())
    if g[a].get(b) is None or g[a][b] > c:
        g[a][b] = c
        g[b][a] = c

# [0]: 거리, [1]: 이전 간선의 가중치
dists = [[sys.maxsize] * 2 for _ in range(n + 1)]
dists[1] = [0, 0]
pq = [(0, 0, 1)]
while pq:
    d, pd, u = heappop(pq)
    if dists[u][0] < d: continue
    for v, w in g[u].items():
        if dists[v][0] > d + w:
            dists[v][0] = d + w
            dists[v][1] = w
            heappush(pq, (d + w, w, v))
        elif dists[v][0] == d + w and w < dists[v][1]:
            dists[v][1] = w

ans = 0
for i in range(2, n + 1):
    ans += dists[i][0] * 9 // 10 + dists[i][1] // 10

print(ans)
# print(dists)