import sys

n, m = map(int, sys.stdin.readline().split())
edges = sorted([tuple(map(int, sys.stdin.readline().split())) for _ in range(m)], key=lambda x: x[2])
q = int(sys.stdin.readline())
req = [set() for _ in range(n + 1)]
res = [(0, 0) for _ in range(q)]
for i in range(q):
    a, b = map(int, sys.stdin.readline().split())
    req[a].add(i)
    req[b].add(i)
parent = [i for i in range(n + 1)]
cnt = [1 for _ in range(n + 1)]

def find(x):
    if x == parent[x]: return x
    parent[x] = find(parent[x])
    return parent[x]

def union(x, y):
    x, y = find(x), find(y)
    if x == y: return
    if x > y: x, y = y, x
    parent[y] = x
    cnt[x] += cnt[y]

for u, v, c in edges:
    u, v = find(u), find(v)
    if u == v: continue
    if u > v: u, v = v, u
    union(u, v)
    for i in req[v]:
        if i in req[u]: res[i] = (c, cnt[u])
        req[u].add(i)

for c, cnt in res:
    if cnt == 0: print(-1)
    else: print(c, cnt)