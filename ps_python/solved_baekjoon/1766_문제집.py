import sys
from heapq import heappop, heappush

n, m = map(int, input().split())
indegrees = [0 for _ in range(n + 1)]
indegrees[0] = -1
edges = dict()
for _ in range(m):
    a, b = map(int, sys.stdin.readline().split())
    if a not in edges.keys():
        edges[a] = [b]
    else:
        edges[a].append(b)
    indegrees[b] += 1
for key in edges.keys():
    edges[key].sort()

# print(edges)
# print(indegrees)

q = []
for (idx, indegree) in enumerate(indegrees):
    if indegree == 0:
        heappush(q, idx)

while len(q) > 0:
    cur = heappop(q)
    print(cur, end=' ')
    if cur not in edges.keys():
        continue
    for adj in edges[cur]:
        indegrees[adj] -= 1
        if indegrees[adj] == 0:
            heappush(q, adj)
