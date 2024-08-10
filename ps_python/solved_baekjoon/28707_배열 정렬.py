import sys
from heapq import heappop, heappush
from collections import defaultdict

n = int(input())
start = [0] + list(map(int, input().split()))
move = [defaultdict(int) for _ in range(n + 1)]
for _ in range(int(input())):
    l, r, c = map(int, input().split())
    if move[l][r] == 0 or move[l][r] > c:
        move[l][r] = c
    if move[r][l] == 0 or move[r][l] > c:
        move[r][l] = c

dest = ' '.join(map(str, sorted(start)))
start = ' '.join(map(str, start))

dists = {start: 0}
q = [(0, start)]

while q:
    dist, cur = heappop(q)
    arr = list(map(int, cur.split()))
    for i, m in enumerate(move):
        for j, c in m.items():
            new_arr = arr[:]
            new_arr[i], new_arr[j] = new_arr[j], new_arr[i]
            new_arr = ' '.join(map(str, new_arr))
            new_dist = dist + c
            if new_dist < dists.get(new_arr, sys.maxsize):
                dists[new_arr] = new_dist
                heappush(q, (new_dist, new_arr))

print(dists[dest] if dest in dists else -1)