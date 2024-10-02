#include <bits/stdc++.h>

using namespace std;
constexpr int SZ = 101'010, SGSZ = 1 << (33 - __builtin_clz(SZ)), INF = 1'000'000'000;

struct HLD_SegTree {
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ], init_w[SZ];
    vector<int> adj[SZ], inp[SZ];

    bitset<SZ> vis;
    void dfs_0(int v = 1) {
        vis[v] = true;
        for (auto& i : inp[v]) {
            if (vis[i]) continue;
            vis[i] = true;
            adj[v].push_back(i);
            dfs_0(i);
        }
    }

    void dfs_1(int v = 1) {
        sz[v] = 1;
        for (auto& i : adj[v]) {
            dep[i] = dep[v] + 1; par[i] = v;
            dfs_1(i); sz[v] += sz[i];
            if (sz[i] > sz[adj[v][0]]) swap(i, adj[v][0]);
        }
    }

    int _order = 0;
    void dfs_2(int v = 1) {
        in[v] = ++_order;
        for (auto& i : adj[v]) {
            top[i] = (i == adj[v][0] ? top[v] : i);
            dfs_2(i);
        }
        // out[v] = _order;
    }

    struct node {
        int lsum, rsum, mxsum, sum;
        node(int v = 0) : lsum(v), rsum(v), mxsum(v), sum(v) {}
        node operator+(const node& r) const {
            node ret;
            ret.lsum = max({lsum, sum + r.lsum, 0});
            ret.rsum = max({r.rsum, r.sum + rsum, 0});
            ret.mxsum = max({mxsum, r.mxsum, rsum + r.lsum, 0});
            ret.sum = sum + r.sum;
            return ret;
        }
        int get() { return max({lsum, rsum, mxsum}); }
    };

    node tree[SGSZ];
    int lazy[SGSZ];

    void init() {
        dfs_0(); dfs_1(); dfs_2();
        fill(lazy, lazy + SGSZ, INF);
    }
    
    void propagate(int s, int e, int i) {
        if (lazy[i] == INF) return;
        tree[i].sum = lazy[i] * (e - s + 1);
        tree[i].lsum = tree[i].rsum = tree[i].mxsum = max(0, tree[i].sum);
        if (s ^ e) {
            lazy[i << 1] = lazy[i];
            lazy[i << 1 | 1] = lazy[i];
        }
        lazy[i] = INF;
    }

    void _update(int s, int e, int l, int r, int w, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return;
        if (l <= s && e <= r) {
            lazy[i] = w;
            propagate(s, e, i);
            return;
        }
        int m = (s + e) >> 1;
        _update(s, m, l, r, w, i << 1); _update(m + 1, e, l, r, w, i << 1 | 1);
        tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }

    node _query(int s, int e, int l, int r, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return node();
        if (l <= s && e <= r) return tree[i];
        int m = (s + e) >> 1;
        return _query(s, m, l, r, i << 1) + _query(m + 1, e, l, r, i << 1 | 1);
    }

    void update(int a, int b, int w) {
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            _update(1, SZ, in[top[a]], in[a], w);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        _update(1, SZ, in[a], in[b], w);
    }

    int query(int a, int b) {
        node r1 = node(), r2 = node();
        while (top[a] ^ top[b]) {
            if (dep[top[a]] > dep[top[b]]) {
                r1 = _query(1, SZ, in[top[a]], in[a]) + r1;
                a = par[top[a]];
            } else {
                r2 = _query(1, SZ, in[top[b]], in[b]) + r2;
                b = par[top[b]];
            }
        }
        if (dep[a] > dep[b]) r1 = _query(1, SZ, in[b], in[a]) + r1;
        else r2 = _query(1, SZ, in[a], in[b]) + r2;
        swap(r1.lsum, r1.rsum);
        return (r1 + r2).get();
    }
};



int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int N, Q; cin >> N;
    HLD_SegTree hld;
    for (int i = 1; i <= N; ++i) cin >> hld.init_w[i];
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.init(); cin >> Q;

    for (int i = 1; i <= N; ++i) hld.update(i, i, hld.init_w[i]);

    while (Q--) {
        int op, u, v; cin >> op >> u >> v;
        if (op == 1) {
            cout << hld.query(u, v) << '\n';
        } else {
            int w; cin >> w;
            hld.update(u, v, w);
        }
    }

    return 0;
}