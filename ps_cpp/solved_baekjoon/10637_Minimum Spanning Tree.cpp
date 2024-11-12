#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int SZ = 100'005, SGSZ = 1 << (33 - __builtin_clz(SZ));

struct HLD_MinSegTree {
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ];
    vector<int> adj[SZ], graph[SZ];

    bool vis[SZ];
    void dfs_0(int u = 1) {
        vis[u] = true;
        for (auto& v : graph[u]) {
            if (vis[v]) continue;
            adj[u].push_back(v);
            dfs_0(v);
        }
    }

    void dfs_1(int u = 1) {
        sz[u] = 1;
        for (auto& v : adj[u]) {
            dep[v] = dep[u] + 1; par[v] = u;
            dfs_1(v);
            sz[u] += sz[v];
            if (sz[v] > sz[adj[u][0]]) swap(v, adj[u][0]);
        }
    }

    int _order = 0;
    void dfs_2(int u = 1) {
        in[u] = ++_order;
        for (auto& v : adj[u]) {
            top[v] = (v == adj[u][0] ? top[u] : v);
            dfs_2(v);
        }
    }

    struct node {
        int v;
        node(int v = INT_MAX) : v(v) {}
        node operator+(const node& rhs) const { return {min(v, rhs.v)}; }
    } tree[SGSZ], lazy[SGSZ];

    void push(int s, int e, int i) {
        if (lazy[i].v == INT_MAX) return;
        tree[i] = tree[i] + lazy[i];
        if (s != e) {
            lazy[i << 1] = lazy[i << 1] + lazy[i];
            lazy[i << 1 | 1] = lazy[i << 1 | 1] + lazy[i];
        }
        lazy[i] = INT_MAX;
    }

    void _update(int s, int e, int l, int r, node v, int i = 1) {
        push(s, e, i);
        if (r < s || e < l) return;
        if (l <= s && e <= r) {
            lazy[i] = lazy[i] + v;
            push(s, e, i);
            return;
        }
        int m = (s + e) >> 1;
        _update(s, m, l, r, v, i << 1); _update(m + 1, e, l, r, v, i << 1 | 1);
    }

    node _query(int s, int e, int l, int r, int i = 1) {
        push(s, e, i);
        if (r < s || e < l) return INT_MAX;
        if (l <= s && e <= r) return tree[i];
        int m = (s + e) >> 1;
        return _query(s, m, l, r, i << 1) + _query(m + 1, e, l, r, i << 1 | 1);
    }

    void update(int u, int v, node val) {
        while (top[u] ^ top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            _update(1, SZ, in[top[u]], in[u], val);
            u = par[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        _update(1, SZ, in[u] + 1, in[v], val);
    }

    node query(int u, int v) {
        node ret = INT_MAX;
        while (top[u] ^ top[v]) {
            if (dep[top[u]] < dep[top[v]]) swap(u, v);
            ret = ret + _query(1, SZ, in[top[u]], in[u]);
            u = par[top[u]];
        }
        if (dep[u] > dep[v]) swap(u, v);
        return ret + _query(1, SZ, in[u] + 1, in[v]);
    }
} hld;

struct UnionFind {
    int par[SZ], sz[SZ];
    UnionFind() { iota(par, par + SZ, 0); fill(sz, sz + SZ, 1); }
    int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        par[y] = x; sz[x] += sz[y];
        return true;
    }
} uf;

struct Edge {
    int w, u, v, idx, is_mst;
    bool operator<(const Edge& rhs) const { return w < rhs.w; }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    vector<Edge> edges;
    for (int i = 0; i < M; ++i) {
        int u, v, w; cin >> u >> v >> w;
        edges.push_back({w, u, v, i, false});
    }
    sort(edges.begin(), edges.end());


    ll mst = 0;
    vector<Edge> not_mst;
    for (int i = 0; i < M; ++i) {
        auto& [w, u, v, _, is_mst] = edges[i];
        if (uf.merge(u, v)) {
            hld.graph[u].push_back(v);
            hld.graph[v].push_back(u);
            mst += w;
            is_mst = true;
        } else {
            not_mst.push_back({w, u, v, i, false});
        }
    }

    // No mst exists
    if (uf.sz[uf.find(1)] < N) {
        while (M--) cout << "-1\n";
        // cout << -1 << '\n';
        return 0;
    }

    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();

    for (auto& [w, u, v, _, __] : not_mst) {
        hld.update(u, v, w);
    }

    sort(edges.begin(), edges.end(), [](auto& a, auto& b) { return a.idx < b.idx; });

    for (int i = 0; i < M; ++i) {
        auto& [w, u, v, idx, is_mst] = edges[i];
        if (!is_mst) cout << mst << '\n';
        else {
            auto res = hld.query(u, v);
            cout << (res.v != INT_MAX ? mst - w + res.v : -1) << '\n';
        }
    }

    return 0;
}