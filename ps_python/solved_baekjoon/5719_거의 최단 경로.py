import sys
from heapq import heappush, heappop
from collections import deque


def dijkstra(g, s):
    dists = [sys.maxsize] * len(g)
    dists[s] = 0
    paths = [set() for _ in range(len(g))]
    pq = [(0, s)]
    while pq:
        d, u = heappop(pq)
        for v, w in g[u].items():
            if d + w < dists[v]:
                dists[v] = d + w
                paths[v] = {u}
                heappush(pq, (d + w, v))
            elif d + w == dists[v]:
                paths[v].add(u)
    return dists, paths

while True:
    N, M = map(int, sys.stdin.readline().split())
    if N == 0 and M == 0: break
    S, D = map(int, sys.stdin.readline().split())
    g = [dict() for _ in range(N)]
    for _ in range(M):
        U, V, P = map(int, sys.stdin.readline().split())
        g[U][V] = P

    dists, paths = dijkstra(g, S)
    if dists[D] == sys.maxsize:
        print(-1)
        continue

    q = deque([D])
    visited = [False] * N
    visited[D] = True
    while q:
        u = q.popleft()
        for v in paths[u]:
            if u in g[v]:
                del g[v][u]
            if visited[v]: continue
            visited[v] = True
            q.append(v)

    dists, _ = dijkstra(g, S)

    print(dists[D] if dists[D] < sys.maxsize else -1)