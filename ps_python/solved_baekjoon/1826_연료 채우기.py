import sys
from heapq import heappop, heappush

n = int(input())
fuel_stations = [tuple(map(int, sys.stdin.readline().split())) for _ in range(n)]
fuel_stations.sort(key=lambda x: (x[0], -x[1]))
dest, fuel = map(int, input().split())
fuel_stations.append((dest, 0))
cnt = 0

q = []
for pos, fuel_amount in fuel_stations:
    if fuel >= dest:
        break
    while fuel < pos and len(q) > 0:
        fuel += -heappop(q)
        cnt += 1
    if fuel < pos:
        break
    heappush(q, -fuel_amount)
print(cnt if fuel >= dest else -1)