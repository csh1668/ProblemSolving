import sys
from collections import deque

src, sink, MAX = 0, 25, 52
n = int(input())
capacity = [[0] * MAX for _ in range(MAX)]
flow = [[0] * MAX for _ in range(MAX)]
# 입력 받기
for _ in range(n):
    a, b, f = input().split()
    a = ord(a) - ord('A') if a.isupper() else ord(a) - ord('a') + 26
    b = ord(b) - ord('A') if b.isupper() else ord(b) - ord('a') + 26
    capacity[a][b] += int(f)
    capacity[b][a] += int(f)

total_flow = 0
while True:
    parent = [-1] * MAX
    parent[src] = src
    q = deque([src])

    while q and parent[sink] == -1:
        cur = q.popleft()
        for nxt in range(0, MAX):
            # 유량이 더 흐를 수 있고, 방문하지 않았다면
            if capacity[cur][nxt] - flow[cur][nxt] > 0 and parent[nxt] == -1:
                q.append(nxt)
                parent[nxt] = cur

    # 더이상 sink로 가는 유량이 없다면
    if parent[sink] == -1:
        break

    amount = sys.maxsize
    p = sink
    while p != src:
        amount = min(capacity[parent[p]][p] - flow[parent[p]][p], amount)
        p = parent[p]

    p = sink
    while p != src:
        flow[parent[p]][p] += amount
        flow[p][parent[p]] -= amount
        p = parent[p]

    total_flow += amount

print(total_flow)