import sys

M, N, T = map(int, sys.stdin.readline().split())
g = [list(map(int, sys.stdin.readline().split())) for _ in range(M)]

parent = [[(i, j) for j in range(N)] for i in range(M)]
cnt = [[1] * N for _ in range(M)]
req = [[set() for _ in range(N)] for _ in range(M)]
res = [[0] * N for _ in range(M)]
for i in range(M):
    for j, x in enumerate(map(int, sys.stdin.readline().split())):
        if x: req[i][j].add((i, j))

def find(x, y):
    if parent[x][y] == (x, y): return x, y
    parent[x][y] = find(*parent[x][y])
    return parent[x][y]

def union(x1, x2, y1, y2):
    (x1, y1), (x2, y2) = find(x1, y1), find(x2, y2)
    if (x1, y1) == (x2, y2): return
    if x1 > x2 or (x1 == x2 and y1 > y2): x1, y1, x2, y2 = x2, y2, x1, y1
    parent[x2][y2] = (x1, y1)
    cnt[x1][y1] += cnt[x2][y2]

edges = []
for i in range(M):
    for j in range(N):
        if i + 1 < M: edges.append((i, j, i + 1, j, abs(g[i][j] - g[i + 1][j])))
        if j + 1 < N: edges.append((i, j, i, j + 1, abs(g[i][j] - g[i][j + 1])))
edges.sort(key=lambda x: (x[4], x[0], x[1], x[2], x[3]))

for x1, y1, x2, y2, d in edges:
    (x1, y1), (x2, y2) = find(x1, y1), find(x2, y2)
    if (x1, y1) == (x2, y2): continue
    if x1 > x2 or (x1 == x2 and y1 > y2): x1, y1, x2, y2 = x2, y2, x1, y1
    union(x1, x2, y1, y2)
    for i, j in req[x2][y2]:
        if cnt[x1][y1] >= T: res[i][j] = d
        req[x1][y1].add((i, j))
    req[x2][y2].clear()

print(sum(sum(row) for row in res))