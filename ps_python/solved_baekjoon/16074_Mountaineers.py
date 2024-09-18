import sys


# num = 2
# sys.stdin = open(f'random{num}.in', 'r')
# sys.stdout = open(f'random{num}.out', 'w')

m, n, q = map(int, sys.stdin.readline().split())
g = [list(map(int, sys.stdin.readline().split())) for _ in range(m)]

parent = [[(j, i) for i in range(n)] for j in range(m)]
req = [[set() for _ in range(n)] for _ in range(m)]
res = [sys.maxsize] * q
for i in range(q):
    x1, y1, x2, y2 = map(lambda x: int(x) - 1, sys.stdin.readline().split())
    req[x1][y1].add(i)
    req[x2][y2].add(i)
    if (x1, y1) == (x2, y2):
        res[i] = g[x1][y1]

def find(x, y):
    if parent[x][y] == (x, y): return x, y
    parent[x][y] = find(*parent[x][y])
    return parent[x][y]

def union(x1, y1, x2, y2):
    (x1, y1), (x2, y2) = find(x1, y1), find(x2, y2)
    if (x1, y1) == (x2, y2): return
    if x1 > x2 or (x1 == x2 and y1 > y2): x1, y1, x2, y2 = x2, y2, x1, y1
    parent[x2][y2] = (x1, y1)

edges = []
for i in range(m):
    for j in range(n):
        if i + 1 < m: edges.append((i, j, i + 1, j, max(g[i][j], g[i + 1][j])))
        if j + 1 < n: edges.append((i, j, i, j + 1, max(g[i][j], g[i][j + 1])))
edges.sort(key=lambda x: (x[4], x[0], x[1], x[2], x[3]))
# print(edges)

for x1, y1, x2, y2, w in edges:
    assert x1 < x2 or (x1 == x2 and y1 < y2)
    (x1, y1), (x2, y2) = find(x1, y1), find(x2, y2)
    if (x1, y1) == (x2, y2): continue
    if x1 > x2 or (x1 == x2 and y1 > y2): x1, y1, x2, y2 = x2, y2, x1, y1
    for i in req[x2][y2]:
        if i in req[x1][y1]: res[i] = min(res[i], w)
        req[x1][y1].add(i)
    union(x1, y1, x2, y2)

for r in res:
    print(r)