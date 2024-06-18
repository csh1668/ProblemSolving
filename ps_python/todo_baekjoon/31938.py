import sys
from heapq import heappush, heappop
from collections import defaultdict

n, m = map(int, input().split())
roads = defaultdict(lambda: defaultdict(int))
for _ in range(m):
    a, b, c = map(int, sys.stdin.readline().split())
    if roads[a][b] == 0 or roads[a][b] > c:
        roads[a][b] = c
        roads[b][a] = c

dists = {1: 0}
visited = defaultdict(bool)
q = [(0, 1, n - 1)] # 거리, 위치, 남은 트럭 수
while q:
    cur_dist, cur_pos, trucks = heappop(q)

    for neighbor, d in roads[cur_pos].items():
        if d == 0:
            continue
        if visited[(cur_pos, neighbor)]:
            d = int(d * 0.9)
        nxt_dist = cur_dist + d
        if dists.get(neighbor) is None or dists[neighbor] > nxt_dist:
            dists[neighbor] = nxt_dist
            visited[(cur_pos, neighbor)] = True
            visited[(neighbor, cur_pos)] = True
            # 트럭을 한 대만 보내봄
            heappush(q, (nxt_dist, neighbor, 1))

    if trucks == 1:
        continue
    for neighbor, d in roads[cur_pos].items():
        if d == 0:
            continue
        if visited[(cur_pos, neighbor)]:
            d = int(d * 0.9)
        nxt_dist = cur_dist + d
        if dists.get(neighbor) is None or dists[neighbor] > nxt_dist:
            dists[neighbor] = nxt_dist
            visited[(cur_pos, neighbor)] = True
            visited[(neighbor, cur_pos)] = True
            # 나머지 트럭도 다 보냄
            heappush(q, (nxt_dist, neighbor, trucks - 2))
print(dists)