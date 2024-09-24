#include <bits/stdc++.h>

using namespace std;

const int SZ = 200'001;

struct HLD_SegTree {
    /*
    HLD
    sz[i] = i를 루트로 하는 서브 트리의 크기
    dep[i] = i의 깊이
    par[i] =  i의 부모 노드
    top[i] = i가 속한 체인의 top 노드
    in[i], out[i] = 오일러 경로 순서
    adj[i] = i의 자식 노드들
    inp[i] = 입력으로 주어진 양방향 간선 정보
    */
    int sz[SZ], dep[SZ], par[SZ], top[SZ], in[SZ], out[SZ];
    vector<int> adj[SZ], inp[SZ];

    // adj 리스트 초기화
    bitset<SZ> vis;
    void _dfs_0(int v = 1) {
        vis[v] = true;
        for (auto& i : inp[v]) {
            if (vis[i]) continue;
            vis[i] = true;
            adj[v].push_back(i);
            _dfs_0(i);
        }
    }

    // sz, dep, par 초기화
    void _dfs_1(int v = 1) {
        sz[v] = 1;
        for (auto& i : adj[v]) {
            dep[i] = dep[v] + 1; par[i] = v;
            _dfs_1(i); sz[v] += sz[i];
            // 서브 트리가 가장 큰 자식을 맨 앞으로 보내기
            if (sz[i] > sz[adj[v][0]]) swap(i, adj[v][0]);
        }
    }

    // in, out, top 초기화
    int _order = 0;
    void _dfs_2(int v = 1) {
        in[v] = ++_order;
        for (auto& i : adj[v]) {
            // i가 v의 첫 번째 자식이면 현재 체인에 연결
            // 아니면 top[i] = i인 새 체인 생성
            top[i] = (i == adj[v][0] ? top[v] : i);
            _dfs_2(i);
        }
        out[v] = _order; // 오일러 경로 out
    }

    void init() {
        _dfs_0(); _dfs_1(); _dfs_2();
    }

    // SegTree manages the range sum
    const int sgsz = 1 << (33 - __builtin_clz(SZ));
    int tree[1 << (33 - __builtin_clz(SZ))];

    // 값이 0일 경우 1로 만들기
    void _update(int x) {
        x |= (sgsz >> 1);
        tree[x] = 1;
        while (x >>= 1) tree[x] = tree[x << 1] | tree[x << 1 | 1];
    }

    // 구간 합 구하기
    int _query(int l, int r) {
        l |= (sgsz >> 1); r |= (sgsz >> 1);
        int ret = 0;
        while (l <= r) {
            if (l & 1) ret |= tree[l++];
            if (~r & 1) ret |= tree[r--];
            l >>= 1; r >>= 1;
        }
        return ret;
    }

    void update(int x) { _update(in[x]); }

    int query(int a, int b) {
        int ret = 0;
        while (top[a] ^ top[b]) { // 두 노드가 같은 체인에 속할 때까지..
            if (dep[top[a]] < dep[top[b]]) swap(a, b);
            // a가 더 깊은 노드
            int st = top[a];
            ret |= _query(in[st], in[a]);
            a = par[st];
        }
        if (dep[a] > dep[b]) swap(a, b);
        ret |= _query(in[a] + 1, in[b]);
        return ret;
    }
};


int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, q; cin >> n >> q;
    HLD_SegTree hld;
    for (int i = 2; i <= n; ++i) {
        int p; cin >> p;
        hld.inp[p].push_back(i);
        hld.inp[i].push_back(p);
    }
    hld.init();

    while (q--) {
        int b, c, d; cin >> b >> c >> d;
        int ans = hld.query(b, c);

        if (ans == 0) {
            cout << "YES\n";
            if (d == 1) hld.update(b);
        }
        else {
            cout << "NO\n";
            if (d == 1) hld.update(c);
        }
    }

    return 0;
}