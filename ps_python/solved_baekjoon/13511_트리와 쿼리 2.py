import sys

N = int(sys.stdin.readline())
adj = [[] for _ in range(N+1)]

for _ in range(N-1):
    u, v, w = map(int, sys.stdin.readline().split())
    adj[u].append((v, w))
    adj[v].append((u, w))

vis = [False] * (N+1)
dep = [0] * (N+1)
par = [[0] * 18 for _ in range(N+1)]
cst = [[0] * 18 for _ in range(N+1)]

st = [0] * (N+1); top = 1
st[0] = 1
vis[1] = True
while top:
    x = st[top-1]
    top -= 1
    for v, w in adj[x]:
        if vis[v]: continue
        vis[v] = True
        dep[v] = dep[x] + 1
        par[v][0] = x
        cst[v][0] = w
        st[top] = v; top += 1

for j in range(1, 18):
    for i in range(1, N+1):
        par[i][j] = par[par[i][j-1]][j-1]
        cst[i][j] = cst[i][j-1] + cst[par[i][j-1]][j-1]

def lca_and_cst(u, v):
    if dep[u] > dep[v]: u, v = v, u
    ret = 0
    for i in range(17, -1, -1):
        if dep[v] - dep[u] >= (1 << i):
            ret += cst[v][i]
            v = par[v][i]
    if u == v: return u, ret
    for i in range(17, -1, -1):
        if par[u][i] != par[v][i]:
            ret += cst[u][i] + cst[v][i]
            u = par[u][i]
            v = par[v][i]
    return par[u][0], ret + cst[u][0] + cst[v][0]

def find_kth(u, v, k):
    # u -> v
    # k가 0이 되거나 lca(u, v)를 만날 때까지...
    # u에서 올라가면서 k의 값을 감소시킨다
    # lca를 만났는데 k값이 0이 아니라면
    # v에서 올라가면서 k의 값을 감소시킨다...?
    # 이때 경로가 반대이므로 k의 값은 반대로 관리되어야 한다
    k -= 1
    a = lca_and_cst(u, v)[0]
    if dep[u] - dep[a] >= k:
        for i in range(17, -1, -1):
            if k >= (1 << i):
                u = par[u][i]
                k -= (1 << i)
        return u
    k -= dep[u] - dep[a]
    k = dep[v] - dep[a] - k
    for i in range(17, -1, -1):
        if k >= (1 << i):
            v = par[v][i]
            k -= (1 << i)
    return v


M = int(sys.stdin.readline())
for _ in range(M):
    op, *args = map(int, sys.stdin.readline().split())
    if op == 1:
        u, v = args
        print(lca_and_cst(u, v)[1])
    else:
        u, v, k = args
        print(find_kth(u, v, k))