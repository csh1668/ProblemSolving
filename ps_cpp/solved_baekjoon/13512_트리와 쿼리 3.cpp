#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

constexpr int MAXN = 100'001, WHITE = 1e9;

struct HLD_SegTree {
    /*
    HLD
    sz[i] = i를 루트로 하는 서브 트리의 크기
    dep[i] = i의 깊이
    par[i] = i의 부모 노드
    top[i] = i가 속한 체인의 top 노드
    in[i], out[i] = dfs 순서
    adj[i] = i의 자식 노드들
    inp[i] = 입력으로 주어진 양방향 간선 정보
    */
    int sz[MAXN], dep[MAXN], par[MAXN], top[MAXN], in[MAXN], out[MAXN];
    vector<int> adj[MAXN], inp[MAXN];

    // adj 초기화
    bitset<MAXN> vis;
    void dfs_0(int v = 1) {
        vis[v] = true;
        for (auto& i : inp[v]) {
            if (vis[i]) continue;
            vis[i] = true;
            adj[v].push_back(i);
            dfs_0(i);
        }
    }

    // sz, dep, par 초기화
    void dfs_1(int v = 1) {
        sz[v] = 1;
        for (auto& i : adj[v]) {
            dep[i] = dep[v] + 1; par[i] = v;
            dfs_1(i); sz[v] += sz[i];
            // 서브 트리가 가장 큰 자식을 맨 앞으로 보내기
            if (sz[i] > sz[adj[v][0]]) swap(i, adj[v][0]);
        }
    }

    // in, out, top 초기화. 마치 오일러 경로처럼...
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

    // SegTree
    const int sgsz = 1 << (33 - __builtin_clz(MAXN));
    int tree[1 << (33 - __builtin_clz(MAXN))];

    void _update(int x, int v) {
        x |= (sgsz >> 1); tree[x] = (tree[x] == WHITE ? v : WHITE);
        while (x >>= 1) tree[x] = tree[x << 1] != WHITE ? tree[x << 1] : (tree[x << 1 | 1] != WHITE ? tree[x << 1 | 1] : WHITE);
    }

    int _query(int l, int r) {
        l |= (sgsz >> 1); r |= (sgsz >> 1);
        int ret = WHITE;
        while (l <= r) {
            if ((l & 1) && tree[l++] != WHITE) return tree[l - 1];
            if ((~r & 1) && tree[r--] != WHITE) ret = tree[r + 1];
            l >>= 1; r >>= 1;
        }
        return ret;
    }

    void update(int x) { _update(in[x], x); }

    int query(int a, int b) {
        int ret = WHITE;
        while (top[a] ^ top[b]) {
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            int tmp = _query(in[top[a]], in[a]);
            if (tmp != WHITE) ret = tmp;
            a = par[top[a]];
        }
        if (dep[a] > dep[b]) swap(a, b);
        int tmp = _query(in[a], in[b]);
        if (tmp != WHITE) ret = tmp;
        return ret;
    }
};

int main()
{
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    HLD_SegTree hld;

    int n, m; cin >> n;
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        hld.inp[u].push_back(v);
        hld.inp[v].push_back(u);
    }
    hld.dfs_0(); hld.dfs_1(); hld.dfs_2();
    fill(hld.tree, hld.tree + hld.sgsz, WHITE);

    cin >> m;
    while (m--) {
        int op, x; cin >> op >> x;
        if (op == 1) hld.update(x);
        else {
            auto ans = hld.query(1, x);
            cout << (ans == WHITE ? -1 : ans) << '\n';
        }
    }

    return 0;
}