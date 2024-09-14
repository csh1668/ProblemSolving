import sys
from heapq import heappop, heappush

# sys.stdin = open('08-02.txt', 'r')

n, m, k, q = map(int, sys.stdin.readline().split())
g = [dict() for _ in range(n + 1)]
# 그래프 생성
for _ in range(m):
    u, v, w = map(int, sys.stdin.readline().split())
    if g[u].get(v) is None or g[u][v] > w:
        g[u][v] = w
        g[v][u] = w
# K개의 정점을 시작으로 하는 다익스트라 수행
dists = [sys.maxsize] * (n + 1)
pq = []
for _ in range(k):
    u = int(sys.stdin.readline())
    dists[u] = 0
    pq.append((0, u))
while pq:
    d, u = heappop(pq)
    if dists[u] < d: continue
    for v, w in g[u].items():
        if dists[v] > d + w:
            dists[v] = d + w
            heappush(pq, (d + w, v))

# 간선들을 정렬
edges = []
for u in range(1, n + 1):
    for v in g[u].keys():
        w = min(dists[u], dists[v])
        edges.append((u, v, w))
        del g[v][u]
edges.sort(key=lambda x: x[-1], reverse=True)
# print(edges)

req = [set() for _ in range(n + 1)]
parent = [i for i in range(n + 1)]
res = [0] * q
for i in range(q):
    u, v = map(int, sys.stdin.readline().split())
    req[u].add(i)
    req[v].add(i)

def find(x):
    if parent[x] == x: return x
    parent[x] = find(parent[x])
    return parent[x]

def union(x, y):
    x, y = find(x), find(y)
    if x == y: return
    if x > y: x, y = y, x
    parent[y] = x

for u, v, w in edges:
    assert u < v
    u, v = find(u), find(v)
    if u == v: continue # 이미 같은 그룹이라면
    lo, hi = u, v
    if lo > hi: lo, hi = hi, lo
    for i in req[hi]:
        if i in req[lo]: res[i] = max(res[i], w)
        req[lo].add(i)
    union(lo, hi)

for r in res:
    print(r)