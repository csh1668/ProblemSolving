#include <bits/stdc++.h>

using namespace std;
using ll = long long;
constexpr int SZ = 202'020, SGSZ = 1 << (33 - __builtin_clz(SZ)), INF = -1'000'000'000;

struct HLD_SegTree {
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ];
    vector<int> adj[SZ], inp[SZ];

    bitset<SZ> vis;
    void dfs_0(int u) {
        vis[u] = true;
        for (auto& v : inp[u]) {
            if (vis[v]) continue;
            adj[u].push_back(v);
            dfs_0(v);
        }
    }

    void dfs_1(int u) {
        sz[u] = 1;
        for (auto& v : adj[u]) {
            dep[v] = dep[u] + 1; par[v] = u;
            dfs_1(v); sz[u] += sz[v];
            if (sz[v] > sz[adj[u][0]]) swap(v, adj[u][0]);
        }
    }

    int _order = 0;
    void dfs_2(int u) {
        in[u] = ++_order;
        for (auto& v : adj[u]) {
            top[v] = (v == adj[u][0] ? top[u] : v);
            dfs_2(v);
        }
    }

    struct node {
        ll lsum, rsum, mxsum, sum;
        node(int v) : lsum(v), rsum(v), mxsum(v), sum(v) {}
        node() : lsum(INF), rsum(INF), mxsum(INF), sum(0) {}
        node operator+(const node& r) const {
            node ret;
            ret.lsum = max(lsum, sum + r.lsum);
            ret.rsum = max(r.rsum, r.sum + rsum);
            ret.mxsum = max({mxsum, r.mxsum, r.lsum + rsum});
            ret.sum = sum + r.sum;
            return ret;
        }
        ll get() const { return max({lsum, rsum, mxsum}); }
    } tree[SGSZ];
    int lazy[SGSZ];

    void init() {
        dfs_0(1); dfs_1(1); dfs_2(1);
        fill(lazy, lazy + SGSZ, INF);
    }

    void propagate(int s, int e, int i = 1) {
        if (lazy[i] == INF) return;
        tree[i].sum = lazy[i] * (e - s + 1);
        tree[i].lsum = tree[i].rsum = tree[i].mxsum = max((ll)lazy[i], tree[i].sum);
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

    ll query(int a, int b) {
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

int init_w[SZ];

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int N, Q; cin >> N >> Q;
    HLD_SegTree hld;
    for (int i = 1; i <= N; ++i) cin >> init_w[i];
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.init();

    for (int i = 1; i <= N; ++i) hld.update(i, i, init_w[i]);

    while (Q--) {
        int t, a, b, c; cin >> t >> a >> b >> c;
        if (t == 1) {
            hld.update(a, b, c);
        } else {
            cout << hld.query(a, b) << '\n';
        }
    }


    return 0;
}