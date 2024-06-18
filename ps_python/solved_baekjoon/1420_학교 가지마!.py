import sys
from collections import deque, defaultdict

n, m = map(int, input().split())
board = []
for _ in range(n):
    tmp = []
    for ch in input():
        if ch == 'K': tmp.append(2)
        elif ch == 'H': tmp.append(3)
        elif ch == '.': tmp.append(0)
        else: tmp.append(1)
    board.append(tmp)

# 특별한 경우: K, H가 인접해있을 경우 or N, M이 1일 경우
if n == 1 and m == 1:
    print(-1)
    exit()
for x in range(n):
    for y in range(m):
        if board[x][y] == 2:
            for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
                nx, ny = x + dx, y + dy
                if 0 <= nx < n and 0 <= ny < m and board[nx][ny] == 3:
                    print(-1)
                    exit()
            break


adj = defaultdict(set)
cap = defaultdict(lambda: defaultdict(int))
flw = defaultdict(lambda: defaultdict(int))

start = (0, 0)
dest = (1, 1)
for x in range(n):
    for y in range(m):
        cur_in = (x << 1, y << 1)
        cur_out = (x << 1 | 1, y << 1 | 1)
        if board[x][y] == 2:
            start = cur_out
        elif board[x][y] == 3:
            dest = cur_in
        elif board[x][y] == 1:
            continue
        cap[cur_in][cur_out] = 1
        adj[cur_in].add(cur_out)
        adj[cur_out].add(cur_in)

        for dx, dy in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nx, ny = x + dx, y + dy
            if 0 <= nx < n and 0 <= ny < m and board[nx][ny] != 1:
                nxt_in = (nx << 1, ny << 1)
                nxt_out = (nx << 1 | 1, ny << 1 | 1)
                cap[cur_out][nxt_in] = sys.maxsize
                cap[nxt_out][cur_in] = sys.maxsize
                adj[cur_out].add(nxt_in)
                adj[nxt_out].add(cur_in)
                adj[cur_in].add(nxt_out)
                adj[nxt_in].add(cur_out)
total = 0
while True:
    parent = defaultdict(lambda: (-1, -1))
    parent[start] = start
    q = deque([start])

    while q and parent[dest] == (-1, -1):
        cur = q.popleft()
        for nxt in adj[cur]:
            tmp1, tmp2 = cap[cur][nxt], flw[cur][nxt]
            if cap[cur][nxt] - flw[cur][nxt] > 0 and parent[nxt] == (-1, -1):
                q.append(nxt)
                parent[nxt] = cur

    if parent[dest] == (-1, -1):
        break

    p = dest
    while p != start:
        flw[parent[p]][p] += 1
        flw[p][parent[p]] -= 1
        p = parent[p]
    total += 1

print(total)