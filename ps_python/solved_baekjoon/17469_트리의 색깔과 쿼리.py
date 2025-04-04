import sys

N, Q = map(int, sys.stdin.readline().split())
P = [0] + [int(sys.stdin.readline()) - 1 for _ in range(N - 1)]
C = [int(sys.stdin.readline()) for _ in range(N)]
queries = [list(map(int, sys.stdin.readline().split())) for _ in range(N + Q - 1)]

parent = [i for i in range(N)]
rank = [0] * N
colors = [{C[i]} for i in range(N)]

def find(x):
    if parent[x] == x: return x
    parent[x] = find(parent[x])
    return parent[x]

def union(x, y):
    x, y = find(x), find(y)
    if x == y: return
    if rank[x] < rank[y]: x, y = y, x
    parent[y] = x
    rank[x] += rank[x] == rank[y]
    colors[x] |= colors[y]

ans = []
for op, a in reversed(queries):
    if op == 1:
        union(a - 1, P[a - 1])
    else:
        ans.append(len(colors[find(a - 1)]))

for a in reversed(ans):
    print(a)