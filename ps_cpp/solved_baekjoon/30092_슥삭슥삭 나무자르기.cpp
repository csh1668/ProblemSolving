#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 100'001, SGSZ = 1 << (33 - __builtin_clz(MAX));

struct HLD_SegTree {
    // HLD
    int sz[MAX], dep[MAX], par[MAX], top[MAX], in[MAX];
    vector<int> adj[MAX], inp[MAX];

    bitset<MAX> vis;
    void dfs_0(int cur = 1) {
        vis[cur] = true;
        for (auto& nxt : inp[cur]) {
            if (vis[nxt]) continue;
            vis[nxt] = true;
            adj[cur].push_back(nxt);
            dfs_0(nxt);
        }
    }

    void dfs_1(int cur = 1) {
        sz[cur] = 1;
        for (auto& nxt : adj[cur]) {
            dep[nxt] = dep[cur] + 1; par[nxt] = cur;
            dfs_1(nxt); sz[cur] += sz[nxt];
            if (sz[nxt] > sz[adj[cur][0]]) swap(nxt, adj[cur][0]);
        }
    }

    int _order = 0;
    void dfs_2(int cur = 1) {
        in[cur] = ++_order;
        for (auto& nxt : adj[cur]) {
            top[nxt] = (nxt == adj[cur][0] ? top[cur] : nxt);
            dfs_2(nxt);
        }
    }

    // SegTree with Lazy
    int tree[SGSZ], lazy[SGSZ];

    void propagate(int s, int e, int i) {
        if (lazy[i]) {
            tree[i] += lazy[i] * (e - s + 1);
            if (s != e) {
                lazy[i << 1] += lazy[i];
                lazy[i << 1 | 1] += lazy[i];
            }
            lazy[i] = 0;
        }
    }

    void _update(int s, int e, int l, int r, int v, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return;
        if (l <= s && e <= r) {
            lazy[i] += v;
            propagate(s, e, i);
            return;
        }
        int m = (s + e) >> 1;
        _update(s, m, l, r, v, i << 1); _update(m + 1, e, l, r, v, i << 1 | 1);
        tree[i] = tree[i << 1] + tree[i << 1 | 1];
    }

    int _query(int s, int e, int l, int r, int i = 1) {
        propagate(s, e, i);
        if (r < s || e < l) return 0;
        if (l <= s && e <= r) return tree[i];
        int m = (s + e) >> 1;
        return _query(s, m, l, r, i << 1) + _query(m + 1, e, l, r, i << 1 | 1);
    }

    void update(int a, int b, int v) {
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            _update(1, MAX, in[top[a]], in[a], v);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        _update(1, MAX, in[a] + 1, in[b], v);
    }

    int query(int a, int b) {
        int ret = 0;
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            ret += _query(1, MAX, in[top[a]], in[a]);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        return ret + _query(1, MAX, in[a] + 1, in[b]);
    }
} hld;

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, q; cin >> n >> q;
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();

    while (q--) {
        int a, b, c, d; cin >> a >> b >> c >> d;
        hld.update(a, b, 1);
        int res = hld.query(c, d);
        cout << (res ? "NO" : "YES") << '\n';
        hld.update(a, b, -1);
    }

    return 0;
}