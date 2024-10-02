#pragma GCC optimize("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")
#pragma GCC optimize("inline")
#pragma GCC optimize("omit-frame-pointer")
#pragma GCC optimize("unsafe-math-optimizations")
#pragma GCC optimize("fast-math")
#pragma GCC optimize("no-stack-protector")

#include <bits/stdc++.h>

using namespace std;
constexpr int SZ = 100'001, SQ = 317;

int color[SZ], weight[SZ], change[SZ], vis[SZ], turn;
vector<int> adj[SZ], inp[SZ];

struct UnionFind {
    int par[SZ], wt[SZ], sz[SZ];
    
    void init() { 
        iota(par, par + SZ, 0);
        fill(sz, sz + SZ, 1);
        for (int i = 1; i <= SZ; ++i) wt[i] = weight[i];
    }

    int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }

    void merge(int u, int v) {
        u = find(u), v = find(v); if (u == v) return;
        par[v] = u; wt[u] = max(wt[u], wt[v]); sz[u] += sz[v];
    }
} uf;

struct Query { int op, u, w; } q[SZ];

pair<int, int> bfs(int u) {
    u = uf.find(u); int ans = 0, size = 0;
    queue<int> Q; Q.push(u);
    vis[u] = ++turn;
    while (!Q.empty()) {
        u = Q.front(); Q.pop();
        ans = max(ans, uf.wt[uf.find(u)]);
        size += uf.sz[uf.find(u)];
        for (auto& v : adj[u]) {
            if (vis[v] == turn) continue;
            if (color[u] == color[v]) {
                vis[v] = turn;
                Q.push(v);
            }
        }
    }
    return {ans, size};
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    // freopen("03.in", "r", stdin);

    int N, M; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        inp[u].push_back(v); inp[v].push_back(u);
    }
    for (int i = 1; i <= N; ++i) cin >> color[i];
    for (int i = 1; i <= N; ++i) cin >> weight[i];

    cin >> M;
    for (int i = 0; i < M; ++i) {
        cin >> q[i].op >> q[i].u;
        if (q[i].op == 3) cin >> q[i].w;
    }

    int tmp = 0;
    for (int l = 0; l < M;) {
        fill(change, change + SZ, 0);
        uf.init();
        int r = min(l + SQ, M);
        // l ~ r 쿼리 중 색이나 가중치가 바뀌는 정점들 체크
        for (int i = l; i < r; ++i) {
            if (q[i].op == 1 || q[i].op == 3) change[q[i].u] = true;
        }

        // 쿼리 중 색과 가중치가 바뀌지 않는 정점들끼리 합치기
        for (int u = 1; u <= N; ++u) {
            adj[u].clear();
            if (change[u]) continue;
            for (auto& v : inp[u]) {
                if (change[v] || color[u] != color[v]) continue;
                uf.merge(u, v);
            }
        }
        // 압축된 그래프 생성 / 리프 노드 미포함
        for (int u = 1; u <= N; ++u) {
            for (auto& v : inp[u]) {
                if (uf.find(u) == uf.find(v)) continue;
                adj[uf.find(u)].push_back(uf.find(v));
            }
        }
        // 각 쿼리 처리
        bool flag = false; int cache = -1;
        for (int i = l; i < r; ++i) {
            if (q[i].op == 1) {
                flag = false;
                color[q[i].u] ^= true;
            } else if (q[i].op == 2) {
                if (flag) {
                    // 트리가 모두 연결되어 있다면 저장된 값 출력
                    cout << cache << '\n';
                } else {
                    // naive한 bfs 수행
                    auto [ans, size] = bfs(q[i].u);
                    if (size == N) flag = true, cache = ans;
                    cout << ans << '\n';
                }
            } else {
                flag = false;
                weight[q[i].u] = q[i].w;
                uf.wt[q[i].u] = q[i].w;
            }
        }
        l = r;
    }

    return 0;
}