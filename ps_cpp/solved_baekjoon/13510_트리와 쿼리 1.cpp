#include <bits/stdc++.h>

using namespace std;

typedef long long ll;

constexpr int MAXN = 101'010;

template<const int MAXN, typename T> struct HLD_SegTree {
    /*
    HLD
    sz[i] = i를 루트로 하는 서브트리의 크기
    dep[i] = i의 깊이
    par[i] = i의 부모 노드
    top[i] = i가 속한 체인의 top 노드
    in[i], out[i] = dfs 순서
    adj[i] = i의 자식 노드들
    */
    int sz[MAXN], dep[MAXN], par[MAXN], top[MAXN], in[MAXN], out[MAXN];
    vector<int> adj[MAXN], inp[MAXN];

    bitset<MAXN> vis;
    void dfs_init(int v = 1) {
        vis[v] = true;
        for(auto& i : inp[v]){
            if (vis[i]) continue;
            vis[i] = true;
            adj[v].push_back(i);
            dfs_init(i);
        }
    }

    // sz, dep, par 초기화
    void dfs_sz(int v = 1) {
        sz[v] = 1;
        for(auto& i : adj[v]) {
            dep[i] = dep[v] + 1; par[i] = v;
            dfs_sz(i); sz[v] += sz[i];
            // 서브 트리가 가장 큰 자식을 맨 앞으로 보내기
            if (sz[i] > sz[adj[v][0]]) swap(i, adj[v][0]);
        }
    }

    // in, out, top 초기화
    int _order = 0;
    void dfs_hld(int v = 1) {
        in[v] = ++_order;
        for(auto& i : adj[v]) {
            top[i] = (i == adj[v][0] ? top[v] : i);
            dfs_hld(i);
        }
        out[v] = _order;
    }

    // SegTree
    const int sgsz = 1 << (33 - __builtin_clz(MAXN));
    T tree[1 << (33 - __builtin_clz(MAXN))];

    void _update(int x, T v) {
        x |= (sgsz >> 1); tree[x] = v;
        while(x >>= 1) tree[x] = tree[x << 1] + tree[x << 1 | 1];
    }

    T _query(int l, int r) {
        l |= (sgsz >> 1); r |= (sgsz >> 1);
        T ret = 0;
        while(l <= r) {
            if (l & 1) ret += tree[l++];
            if (~r & 1) ret += tree[r--];
            l >>= 1; r >>= 1;
        }
        return ret;
    }

    void update(int x, T v) { _update(in[x], v); }

    T query(int a, int b) {
        T ret = 0;
        while(top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            int st = top[a];
            ret += _query(in[st], in[a]);
            a = par[st];
        }
        if (dep[a] > dep[b]) swap(a, b);
        ret += _query(in[a] + 1, in[b]);
        return ret;
    }
};

struct max_node {
    int v;
    max_node operator+(const max_node& rhs) const { return {max(v, rhs.v)}; }
    max_node operator+=(const max_node& rhs) { return *this = *this + rhs; }
    max_node(int v = 0) : v(v) {}
};

struct edge {
    int u, v, w;
} edges[MAXN];

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, q; cin >> n;
    HLD_SegTree<MAXN, max_node> hld;

    for(int i = 1; i < n; ++i) {
        int &u = edges[i].u, &v = edges[i].v, &w = edges[i].w;
        cin >> u >> v >> w;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_init(); hld.dfs_sz(); hld.dfs_hld();

    for(int i = 1; i < n; ++i) {
        int u = edges[i].u, v = edges[i].v, w = edges[i].w;
        if (hld.par[v] == u) swap(u, v);
        hld.update(u, w);
    }

    cin >> q;
    while (q--) {
        int op, a, b; cin >> op >> a >> b;
        if (op == 1) {
            int u = edges[a].u, v = edges[a].v;
            if (hld.dep[u] < hld.dep[v]) swap(u, v);
            // u가 자식 노드
            hld.update(u, b);
        }
        else if (a == b) cout << 0 << '\n';
        else cout << hld.query(a, b).v << '\n';
        // 틀릴 경우 a == b인 경우 0을 출력하도록 해야함
    }

    return 0;
}