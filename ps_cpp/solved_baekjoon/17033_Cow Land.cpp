#include <bits/stdc++.h>

using namespace std;

constexpr int MAXN = 100'001;

struct HLD_XORSegTree {
    int sz[MAXN], dep[MAXN], par[MAXN], top[MAXN], in[MAXN];
    vector<int> adj[MAXN], inp[MAXN];

    bitset<MAXN> vis;
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

    // XOR SegTree
    const int sgsz = 1 << (33 - __builtin_clz(MAXN));
    int tree[1 << (33 - __builtin_clz(MAXN))];

    void _update(int i, int v) {
        i |= (sgsz >> 1); tree[i] = v;
        while (i >>= 1) tree[i] = tree[i << 1] ^ tree[i << 1 | 1];
    }

    int _query(int l, int r) {
        l |= (sgsz >> 1); r |= (sgsz >> 1);
        int ret = 0;
        while (l <= r) {
            if (l & 1) ret ^= tree[l++];
            if (~r & 1) ret ^= tree[r--];
            l >>= 1; r >>= 1;
        }
        return ret;
    }

    void update(int x, int v) { _update(in[x], v); }

    int query(int a, int b) {
        int ret = 0;
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            ret ^= _query(in[top[a]], in[a]);
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        ret ^= _query(in[a], in[b]);
        return ret;
    }
};

int e[MAXN];

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, q; cin >> n >> q;
    HLD_XORSegTree hld;
    for (int i = 1; i <= n; ++i) cin >> e[i];
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();
    for (int i = 1; i <= n; ++i) hld.update(i, e[i]);

    while (q--) {
        int op, a, b; cin >> op >> a >> b;
        if (op == 1) {
            hld.update(a, b);
        } else {
            cout << hld.query(a, b) << '\n';
        }
    }

    return 0;
}