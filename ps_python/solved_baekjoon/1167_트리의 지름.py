from heapq import heappush, heappop


distances = {}


def dijkstra(start, start_dist, arr):
    q = []
    heappush(q, (start_dist, start))  # 거리 0, 현재 위치 1
    distances = {start: start_dist}

    while len(q) > 0:
        cur_distance, cur_pos = heappop(q)
        for edge in arr[cur_pos]:
            nxt_pos, distance_to_cur = edge
            nxt_distance = cur_distance + distance_to_cur
            nxt_distance_cached = distances.get(nxt_pos)
            if nxt_distance_cached is None or nxt_distance_cached > nxt_distance:
                distances[nxt_pos] = nxt_distance
                heappush(q, (nxt_distance, nxt_pos))
    return distances


v = int(input())
tree = dict()

for _ in range(v):
    tokens = list(map(int, input().split()))
    i = 1
    while True:
        if tokens[i] == -1:
            break
        lst = tree.get(tokens[0])
        if lst is not None:
            lst.append((tokens[i], tokens[i + 1]))
        else:
            lst = [(tokens[i], tokens[i + 1])]
            tree[tokens[0]] = lst
        i += 2

dists = dijkstra(1, 0, tree)
far_pos, far_dist = max(dists.items(), key=lambda x: x[1])
dists_2 = dijkstra(far_pos, 0, tree)
result_dist = max(dists_2.values())
print(result_dist)