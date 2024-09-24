#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

constexpr int SZ = 100'100, SGSZ = 1 << (33 - __builtin_clz(SZ));

struct node {
    ll v, lazy;
    node(ll v = 0) : v(v) {}
};

struct HLD_LazySegTree {
    /*
    HLD
    sz[i] = i를 루트로 하는 서브 트리의 크기
    dep[i] = i의 깊이
    par[i] = i의 부모 노드
    top[i] = i가 속한 체인의 top 노드
    in[i], out[i] = 오일러 경로
    adj[i] = i의 자식 노드들
    inp[i] = 입력으로 주어진 양방향 간선 정보
    */
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ], out[SZ];
    vector<int> adj[SZ], inp[SZ];

    // 양방향 간선 정보를 이용해서 단방향 트리 만들기
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

    // 트리의 위치 정보 초기화
    void dfs_1(int v = 1) {
        sz[v] = 1;
        for (auto& i : adj[v]) {
            dep[i] = dep[v] + 1; par[i] = v;
            dfs_1(i); sz[v] += sz[i];
            // 서브 트리가 가장 큰 자식을 맨 앞으로 보내기
            if (sz[i] > sz[adj[v][0]]) swap(i, adj[v][0]);
        }
    }

    // 오일러 경로 테크닉
    int _order = 0;
    void dfs_2(int v = 1) {
        in[v] = ++_order;
        for (auto& i : adj[v]) {
            // i가 v의 첫 번째 자식이면 체인에 연결
            // 아니면 top[i] = i인 새 체인 생성
            top[i] = (i == adj[v][0] ? top[v] : i);
            dfs_2(i);
        }
        out[v] = _order; // 이건 사실 필요 없음
    }

    // LazySegTree
    node tree[SGSZ];

    void propagate(int l, int r, int i) {
        if (tree[i].lazy == 0) return;
        tree[i].v += tree[i].lazy * (r - l + 1);
        if (l < r) {
            tree[i << 1].lazy += tree[i].lazy;
            tree[i << 1 | 1].lazy += tree[i].lazy;
        }
        tree[i].lazy = 0;
    }

    void _update(int l, int r, int s, int e, ll v, int i = 1) {
        propagate(l, r, i);
        if (e < l || r < s) return;
        if (s <= l && r <= e) {
            tree[i].lazy += v;
            propagate(l, r, i);
            return;
        }
        int m = (l + r) >> 1;
        _update(l, m, s, e, v, i << 1); _update(m + 1, r, s, e, v, i << 1 | 1);
        tree[i].v = tree[i << 1].v + tree[i << 1 | 1].v;
    }

    ll _query(int l, int r, int s, int e, int i = 1) {
        propagate(l, r, i);
        if (e < l || r < s) return 0;
        if (s <= l && r <= e) return tree[i].v;
        int m = (l + r) >> 1;
        return _query(l, m, s, e, i << 1) + _query(m + 1, r, s, e, i << 1 | 1);
    }

    void update(int a, int b, ll v) {
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            // a가 더 깊은 노드로 만들기..
            _update(1, SZ, in[top[a]], in[a], v);
            a = par[top[a]];
        }
        if (in[a] > in[b]) swap(a, b);
        _update(1, SZ, in[a] + 1, in[b], v);
    }

    ll query(int a, int b) {
        ll ret = 0;
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            // a가 더 깊은 노드로 만들기..
            ret += _query(1, SZ, in[top[a]], in[a]);
            a = par[top[a]];
        }
        if (in[a] > in[b]) swap(a, b);
        ret += _query(1, SZ, in[a] + 1, in[b]);
        return ret;
    }
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, m; cin >> n >> m;
    assert(n < SZ);
    HLD_LazySegTree hld;
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();

    while (m--) {
        char op; int u, v; cin >> op >> u >> v;
        if (op == 'P') {
            hld.update(u, v, 1);
        } else {
            cout << hld.query(u, v) << '\n';
        }
    }

    return 0;
}