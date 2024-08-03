import sys
from collections import deque

n, m = map(int, input().split())
a = list(map(int, input().split()))
b = list(map(int, input().split()))

MAX = (max(n, m) + 1) * 2
src, sink = MAX - 1, MAX - 2
adj = [list[int]() for _ in range(MAX)]
cap = [[0] * MAX for _ in range(MAX)]
flw = [[0] * MAX for _ in range(MAX)]
for i in range(n):
	cap[src][i] = a[i]
	adj[src].append(i)
	adj[i].append(src)
	for j in range(m):
		k = j + max(n, m)
		cap[i][k] = 1
		adj[i].append(k)
		adj[k].append(i)
for i in range(m):
	k = i + max(n, m)
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

for i in range(n):
	for j in range(m):
		k = j + max(n, m)
		print(1 if flw[i][k] == 1 else 0, end='')
	print()

#B에서 A로 가는 간선이 없으므로 c[B][A]는 0입니다.
# 그러나 유량의 대칭성에 의해 f[B][A] = -1이 됩니다.
# 잔여 용량은 0 - (-1), 즉 1이 됩니다.
# 실제로는 존재하지 않는 간선이지만 이미 들어온 유량을 다시 상대방에게 보내준다는 의미로 해석할 수 있습니다.
#