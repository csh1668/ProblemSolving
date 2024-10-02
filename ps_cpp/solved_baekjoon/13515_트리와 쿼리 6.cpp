#include <bits/stdc++.h>

using namespace std;
constexpr int SZ = 100'001, SQ = 317;

int color[SZ], change[SZ], vis[SZ], up[SZ], white[SZ], black[SZ], turn;
vector<int> adj[SZ], inp[SZ];

struct UnionFind {
    int par[SZ], sz[SZ];
    
    void init() { iota(par, par + SZ, 0); fill(sz, sz + SZ, 1); }

    int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }

    void merge(int u, int v) {
        u = find(u), v = find(v); if (u == v) return;
        if (sz[u] < sz[v]) swap(u, v);
        par[v] = u; sz[u] += sz[v];
    }
} uf;

struct Query { int op, u; } q[SZ];

// 부모 자식 관계 설정
void dfs_0(int u = 1, int p = 0) {
    for (auto& v: inp[u]) {
        if (v == p) continue;
        up[v] = u;
        dfs_0(v, u);
    }
}

int bfs(int u) {
    u = uf.find(u); int ans = 0;
    queue<int> Q; Q.push(u);
    vis[u] = ++turn;
    while (!Q.empty()) {
        u = Q.front(); Q.pop();
        ans += uf.sz[uf.find(u)];
        for (auto& v : adj[u]) {
            if (vis[v] == turn) continue;
            if (color[u] == color[v]) {
                vis[v] = turn;
                Q.push(v);
            }
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        inp[u].push_back(v); inp[v].push_back(u);
    }
    dfs_0();
    cin >> M;
    for (int i = 0; i < M; ++i) cin >> q[i].op >> q[i].u;

    int tmp = 0;
    for (int l = 0; l < M;) {
        fill(change, change + SZ, 0);
        uf.init();
        int r = min(l + SQ, M);
        // l ~ r 쿼리 중 색이 바뀌는 정점들 체크
        for (int i = l; i < r; ++i) {
            if (q[i].op == 1) change[q[i].u] = true;
        }
        // 부모 자식 간 색이 같은 정점들 합치기
        for (int u = 1; u <= N; ++u) {
            adj[u].clear();
            if (change[u] || change[up[u]] || color[u] != color[up[u]]) continue;
            uf.merge(u, up[u]);
        }
        // 

        // 쿼리 중 색이 바뀌지 않는 정점들끼리 (색이 같으면) 합치기
        for (int u = 1; u <= N; ++u) {
            adj[u].clear();
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
        bool flag = false;
        for (int i = l; i < r; ++i) {
            if (q[i].op == 1) {
                flag = false;
                color[q[i].u] ^= true;
                // 아무 정점에 쿼리 수행 후 전체인지 확인
            } else {
                // naive한 bfs 수행
                if (flag) {
                    cout << N << '\n';
                } else {
                    int ans = bfs(q[i].u);
                    if (ans == N) flag = true;
                    cout << ans << '\n';
                }
            }
        }
        l = r;
    }

    return 0;
}