import sys
from collections import deque

n, m = map(int, input().split())
g = [dict() for _ in range(n + 1)]
for _ in range(m):
    a, b, c = map(int, input().split())
    if g[a].get(b) is None or g[a][b] > c:
        g[a][b] = c

dists = [sys.maxsize] * (n + 1)
dists[1] = 0
in_queue = [False] * (n + 1)
in_queue[1] = True
counter = [0] * (n + 1)
q = deque([1])

# SPFA
while q:
    cur = q.popleft()
    in_queue[cur] = False
    counter[cur] += 1
    if counter[cur] >= n:
        print(-1)
        exit()
    for nxt, w in g[cur].items():
        if dists[nxt] > dists[cur] + w:
            dists[nxt] = dists[cur] + w
            if not in_queue[nxt]:
                q.append(nxt)
                in_queue[nxt] = True

for i in range(2, n + 1):
    print(dists[i] if dists[i] != sys.maxsize else -1)