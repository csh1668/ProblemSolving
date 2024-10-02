#include <bits/stdc++.h>

using namespace std;
typedef unsigned int uint;

constexpr int SZ = 500'001, SGSZ = 1 << (33 - __builtin_clz(SZ));
// constexpr int SZ = 1'000, SGSZ = 1 << (33 - __builtin_clz(SZ));

struct node {
    // v: 구간합, m: multiply lazy, a: add lazy
    uint v, m, a;
    node(uint v = 0) : v(v), m(1), a(0) {}
};

struct HLD_LazySegTree {
    // HLD
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ], out[SZ];
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
        out[v] = _order;
    }

    // LazySegTree
    node tree[SGSZ];

    void propagate(int s, int e, int i) {
        if (tree[i].m == 1 && tree[i].a == 0) return;
        tree[i].v = tree[i].v * tree[i].m + tree[i].a * (e - s + 1);
        if (s < e) {
            tree[i << 1].m *= tree[i].m;
            tree[i << 1].a = tree[i << 1].a * tree[i].m + tree[i].a;
            tree[i << 1 | 1].m *= tree[i].m;
            tree[i << 1 | 1].a = tree[i << 1 | 1].a * tree[i].m + tree[i].a;
        }
        tree[i].m = 1; tree[i].a = 0;
    }

    void _update(int s, int e, int l, int r, bool add, uint v, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return;
        if (l <= s && e <= r) {
            if (add) tree[i].a += v;
            else tree[i].m *= v, tree[i].a *= v;
            propagate(s, e, i);
            return;
        }
        int m = (s + e) >> 1;
        _update(s, m, l, r, add, v, i << 1);
        _update(m + 1, e, l, r, add, v, i << 1 | 1);
        tree[i].v = tree[i << 1].v + tree[i << 1 | 1].v;
    }

    uint _query(int s, int e, int l, int r, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return 0;
        if (l <= s && e <= r) return tree[i].v;
        int m = (s + e) >> 1;
        return _query(s, m, l, r, i << 1) + _query(m + 1, e, l, r, i << 1 | 1);
    }

    void update(int a, int b, bool add, uint v) {
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            _update(1, SZ, in[top[a]], in[a], add, v);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        _update(1, SZ, in[a], in[b], add, v);
    }

    uint query(int a, int b) {
        uint ret = 0;
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            ret += _query(1, SZ, in[top[a]], in[a]);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        ret += _query(1, SZ, in[a], in[b]);
        return ret;
    }
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, q; cin >> n >> q;
    HLD_LazySegTree hld;
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();

    while (q--) {
        int op; cin >> op;
        if (op == 1) {
            int x, v; cin >> x >> v;
            hld._update(1, SZ, hld.in[x], hld.out[x], true, v);
        } else if (op == 2) {
            int x, y, v; cin >> x >> y >> v;
            hld.update(x, y, true, v);
        } else if (op == 3) {
            int x, v; cin >> x >> v;
            hld._update(1, SZ, hld.in[x], hld.out[x], false, v);
        } else if (op == 4) {
            int x, y, v; cin >> x >> y >> v;
            hld.update(x, y, false, v);
        } else if (op == 5) {
            int x; cin >> x;
            cout << hld._query(1, SZ, hld.in[x], hld.out[x]) << '\n';
        } else {
            int x, y; cin >> x >> y;
            cout << hld.query(x, y) << '\n';
        }
    }

    return 0;
}