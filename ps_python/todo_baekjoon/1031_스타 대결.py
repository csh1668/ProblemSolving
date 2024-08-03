import sys
from collections import deque

n, m = map(int, input().split())
a = list(map(int, input().split()))
b = list(map(int, input().split()))

MAX_NM = max(n, m)
MAX = (MAX_NM + 1) * 2
src, sink = MAX - 1, MAX - 2
adj = [list[int]() for _ in range(MAX)]
cap = [[0] * MAX for _ in range(MAX)]
flw = [[0] * MAX for _ in range(MAX)]
for i in range(n):
    cap[src][i] = a[i]
    adj[src].append(i)
    adj[i].append(src)
    for j in range(m):
        k = j + MAX_NM
        cap[i][k] = 1
        adj[i].append(k)
        adj[k].append(i)
for i in range(m):
    k = i + MAX_NM
    cap[k][sink] = b[i]
    adj[k].append(sink)

cnt = 0
while True:
    parent = [-1] * MAX
    parent[src] = src

    q = deque([src])
    while q and parent[sink] == -1:
        cur = q.popleft()
        for nxt in adj[cur]:
            if cap[cur][nxt] - flw[cur][nxt] > 0 and parent[nxt] == -1:
                parent[nxt] = cur
                q.append(nxt)

    if parent[sink] == -1:
        break

    p = sink
    while p != src:
        flw[parent[p]][p] += 1
        flw[p][parent[p]] -= 1
        p = parent[p]
    cnt += 1

mch = [[False] * m for _ in range(n)]
for i in range(n):
    for j in range(m):
        k = j + MAX_NM
        if flw[i][k]:
            mch[i][j] = True

can = True
for i in range(n):
    if cap[src][i] != flw[src][i]:
        can = False
for j in range(m):
    j += MAX_NM
    if cap[j][sink] != flw[j][sink]:
        can = False


if not can:
    print(-1)
    exit()


def rematch(i, j):
    for x in range(i + 1, n):
        if not mch[x][j]:
            for y in range(j + 1, m):
                if mch[x][y] and not mch[i][y]:
                    return x, y
    return -1, -1


for i in range(n):
    for j in range(m):
        k = j + MAX_NM
        if mch[i][j]:
            # TODO: 2개의 정점만 고려해선 안되고 더 큰 사이클을 고려해야 함
            x, y = rematch(i, j)
            if x == -1 and y == -1: continue
            mch[i][j], mch[x][j] = mch[x][j], mch[i][j]
            mch[i][y], mch[x][y] = mch[x][y], mch[i][y]

for i in range(n):
    for j in range(m):
        print(1 if mch[i][j] else 0, end='')
    print()