import sys
from heapq import heappush, heappop


for _ in range(int(sys.stdin.readline())):
    n, m, k = map(int, sys.stdin.readline().split())
    g = [[] for _ in range(n + 1)]
    for _ in range(k):
        u, v, c, d = map(int, sys.stdin.readline().split())
        g[u].append((v, c, d))
    # for i in range(1, n + 1):
    #     g[i].sort(key=lambda x: x[2])

    dp = [[sys.maxsize] * (m + 1) for _ in range(n + 1)]
    dp[1] = [0] * (m + 1)

    pq = [(0, 0, 1)]    # 시간, 비용, 정점
    while pq:
        d, c, u = heappop(pq)
        if u == n: break
        if dp[u][c] < d: continue
        for v, nc, w in g[u]:
            if c + nc <= m and dp[v][c + nc] > d + w:
                for i in range(c + nc, m + 1):
                    if dp[v][i] > d + w:
                        dp[v][i] = d + w
                    else:
                        break
                heappush(pq, (d + w, c + nc, v))

    ans = min(dp[n])
    print(ans if ans != sys.maxsize else 'Poor KCM')