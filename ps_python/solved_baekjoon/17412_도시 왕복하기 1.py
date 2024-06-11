import sys
from collections import deque

n, p = map(int, sys.stdin.readline().split())
cap = [[0] * n for _ in range(n)]
flw = [[0] * n for _ in range(n)]
for _ in range(p):
    a, b = map(lambda x: int(x) - 1, sys.stdin.readline().split())
    cap[a][b] = 1
    # cap[b][a] = 1

total = 0
# visited = [False] * n
while True:
    parent = [-1] * n
    start = 0
    dest = 1
    parent[start] = start
    q = deque([start])

    while q and parent[dest] == -1:
        cur = q.popleft()
        for nxt in range(n):
            if cap[cur][nxt] - flw[cur][nxt] > 0 and parent[nxt] == -1:
                # if nxt != 0 and nxt != 1 and visited[nxt]:
                #     continue
                q.append(nxt)
                parent[nxt] = cur

    if parent[dest] == -1:
        break

    amount = sys.maxsize
    p = dest
    while p != start:
        amount = min(cap[parent[p]][p] - flw[parent[p]][p], amount)
        p = parent[p]

    p = dest
    while p != start:
        flw[parent[p]][p] += amount
        flw[p][parent[p]] -= amount
        p = parent[p]
        # visited[p] = True

    total += amount

print(total)