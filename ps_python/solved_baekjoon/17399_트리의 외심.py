import sys

N = int(sys.stdin.readline())
adj = [[] for _ in range(N + 1)]
for _ in range(N - 1):
    u, v = map(int, sys.stdin.readline().split())
    adj[u].append(v)
    adj[v].append(u)

vis = [False] * (N + 1)
dep = [0] * (N + 1)
par = [[0] * 18 for _ in range(N + 1)]

st = [0] * (N + 1); top = 1
st[0] = 1; vis[1] = True
while top:
    x = st[top - 1]; top -= 1
    for v in adj[x]:
        if vis[v]: continue
        vis[v] = True; dep[v] = dep[x] + 1; par[v][0] = x
        st[top] = v; top += 1

for j in range(1, 18):
    for i in range(1, N + 1):
        par[i][j] = par[par[i][j - 1]][j - 1]

def find_lca(u, v):
    if dep[u] > dep[v]: u, v = v, u
    for i in range(17, -1, -1):
        if dep[v] - dep[u] >= (1 << i):
            v = par[v][i]
    if u == v: return u
    for i in range(17, -1, -1):
        if par[u][i] != par[v][i]:
            u = par[u][i]; v = par[v][i]
    return par[u][0]

def find_dist(u, v, lca = -1):
    if lca == -1: lca = find_lca(u, v)
    return dep[u] + dep[v] - 2 * dep[lca] + 1

def find_kth(u, v, k, lca):
    k -= 1
    if dep[u] - dep[lca] >= k:
        for i in range(17, -1, -1):
            if k >= (1 << i): u = par[u][i]; k -= (1 << i)
        return u
    k -= dep[u] - dep[lca]
    k = dep[v] - dep[lca] - k
    for i in range(17, -1, -1):
        if k >= (1 << i): v = par[v][i]; k -= (1 << i)
    return v

def find_center(u, v):
    if dep[u] > dep[v]: u, v = v, u
    lca = find_lca(u, v)
    dist = find_dist(u, v, lca)
    if dist % 2 == 0: return -1
    dist = (dist + 1) // 2
    return find_kth(u, v, dist, lca)

M = int(sys.stdin.readline())
for _ in range(M):
    u, v, w = map(int, sys.stdin.readline().split())
    center_uv = find_center(u, v)
    center_vw = find_center(v, w)
    center_wu = find_center(w, u)
    if center_uv != -1 and find_dist(center_uv, w) == find_dist(u, center_uv):
        print(center_uv)
    elif center_vw != -1 and find_dist(center_vw, u) == find_dist(v, center_vw):
        print(center_vw)
    elif center_wu != -1 and find_dist(center_wu, v) == find_dist(w, center_wu):
        print(center_wu)
    else:
        print(-1)