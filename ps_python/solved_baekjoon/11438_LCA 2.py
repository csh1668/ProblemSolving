import sys
from collections import deque

n = int(sys.stdin.readline())
children = [[] for i in range(n + 1)]
for _ in range(n - 1):
    a, b = map(int, sys.stdin.readline().split())
    children[a].append(b)
    children[b].append(a)

visited = [False] * (n + 1)
depths = [0] * (n + 1)
parents = [[0] * 18 for _ in range(n + 1)]

st = deque([1])
visited[1] = True
parents[1][0] = 1
while st:
    cur = st.pop()
    for child in children[cur]:
        if not visited[child]:
            visited[child] = True
            depths[child] = depths[cur] + 1
            parents[child][0] = cur
            st.append(child)

for i in range(1, 18):
    for j in range(1, n + 1):
        parents[j][i] = parents[parents[j][i - 1]][i - 1]

for _ in range(int(sys.stdin.readline())):
    a, b = map(int, sys.stdin.readline().split())
    if depths[a] < depths[b]:
        a, b = b, a

    diff = depths[a] - depths[b]
    for i in range(18):
        if diff & (1 << i):
            a = parents[a][i]

    if a == b:
        print(a)
        continue

    for i in range(17, -1, -1):
        if parents[a][i] != parents[b][i]:
            a = parents[a][i]
            b = parents[b][i]

    print(parents[a][0])