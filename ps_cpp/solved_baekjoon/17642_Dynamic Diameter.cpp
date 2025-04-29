#pragma GCC optimize("03")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

#pragma GCC target("avx2")

using namespace std;
using namespace std;
using namespace __gnu_pbds;
typedef long long int ll;
template<typename T, typename U> using hashmap = gp_hash_table<T, U, hash<T>, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;
constexpr int MAXN = 100'002;

int sz[MAXN], cen_par[MAXN];
bool vis[MAXN];
// G[u] = (v, j, d) u에서 v로 가는 j번째 간선의 (최초) 가중치가 d이다
vector<tuple<int, int, ll>> G[MAXN];
// E2V[u][j] = v : 센트로이드 u의 서브 트리에서 j번째 간선을 자식으로 가지는 정점이 v이다 (edge to vertex)
hashmap<int, int> E2V[MAXN];
// E2L[j] = u : j번째 간선을 가지는 리프 정점이 u이다 (edge to leaf)
int E2L[MAXN];
// 센트로이드에서 각 정점의 오일러 투어 방문 순서 (Start, End)
hashmap<int, int> S[MAXN], E[MAXN];
// C2R[i][j] = k : 센트로이드 트리 상의 i의 자식 노드 j -> 실제 트리 상에서 j가 있는 서브트리로 가는 i의 인접 자식 노드는 k이다 (subtree centroid to subtree root)
hashmap<int, int> C2R[MAXN];
// j번째 간선의 가중치
ll prev_w[MAXN];

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto &[v, j, d] : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto &[v, j, d] : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

struct SegTree {
    struct Node {
        ll lmx, psum;
        Node() : lmx(LLONG_MIN), psum(0) {}
        Node(ll v) : lmx(v), psum(v) {}
        Node operator+(const Node &o) const {
            Node res;
            res.lmx = max(lmx, psum + o.lmx);
            res.psum = psum + o.psum;
            return res;
        }
    };
    int offset;
    vector<Node> T;
    SegTree(const vector<ll> &V = {}) {
        int N = V.size();
        if (N == 0) return;
        offset = 1 << (32 - __builtin_clz(N));
        T.resize(offset * 2);
        for (int i = 0; i < N; ++i) {
            T[offset | i] = Node(V[i]);
        }
        for (int i = offset - 1; i > 0; --i) {
            T[i] = T[i << 1] + T[i << 1 | 1];
        }
    }
    void update_add(int i, ll v) {
        i |= offset;
        T[i].lmx += v; T[i].psum += v;
        while (i > 1) {
            i >>= 1;
            T[i] = T[i << 1] + T[i << 1 | 1];
        }
    }
    Node query(int l, int r) {
        Node lhs = Node(0), rhs = Node(0);
        for (l |= offset, r |= offset; l <= r; l >>= 1, r >>= 1) {
            if (l & 1) lhs = lhs + T[l++];
            if (!(r & 1)) rhs = T[r--] + rhs;
        }

        return lhs + rhs;
    }
    // 비재귀 구현이 잘 안돼서 아래걸로하다가 결국 구현이 잘되서 주석처리
    // int N;
    // vector<Node> T;
    // SegTree(const vector<ll> &V = {}) {
    //     N = V.size();
    //     if (N == 0) return;
    //     T.resize(1 << (32 - __builtin_clz(N) + 1));
    //     // test
    //     // for (int i = 0; i < N; ++i) {
    //     //     cout << format("V[{}] = {}\n", i, V[i]);
    //     // } cout << flush;
    //     _build(0, N - 1, V);
    // }
    // Node &_build(int l, int r, const vector<ll> &V, int i = 1) {
    //     if (l == r) return T[i] = Node(V[l]);
    //     int m = (l + r) >> 1;
    //     return T[i] = _build(l, m, V, i << 1) + _build(m + 1, r, V, i << 1 | 1);
    // }
    // void _update_add(int l, int r, int pos, ll v, int i = 1) {
    //     if (l == r) {
    //         T[i] = Node(T[i].lmx + v);
    //         return;
    //     }
    //     int m = (l + r) >> 1;
    //     if (pos <= m) {
    //         _update_add(l, m, pos, v, i << 1);
    //     } else {
    //         _update_add(m + 1, r, pos, v, i << 1 | 1);
    //     }
    //     T[i] = T[i << 1] + T[i << 1 | 1];
    // }
    // Node _query(int l, int r, int ql, int qr, int i = 1) {
    //     if (l > qr || r < ql) return Node(0);
    //     if (ql <= l && r <= qr) return T[i];
    //     int m = (l + r) >> 1;
    //     return _query(l, m, ql, qr, i << 1) + _query(m + 1, r, ql, qr, i << 1 | 1);
    // }
    // void update_add(int pos, ll v) { _update_add(0, N - 1, pos, v); }
    // Node query(int l, int r) { return _query(0, N - 1, l, r); }
} T[MAXN];

int _order = 0;
vector<ll> V;
void dfs(int u, int p, int cen, ll last) {
    S[cen][u] = ++_order;
    V[_order] = last;
    for (auto &[v, j, nd] : G[u]) {
        if (v == p || vis[v]) continue;
        E2V[cen][j] = v;
        E2L[j] = v; // 가장 마지막으로 업데이트 되는 정점이 리프 정점이다
        dfs(v, u, cen, nd);
    }
    E[cen][u] = ++_order;
    V[_order] = -last;
}

void decompose(int u = 1, int p = 0) {
    int cap = get_size(u, p);
    int cen = get_centroid(u, p, cap / 2);
    vis[cen] = true;
    cen_par[cen] = p;
    C2R[p][cen] = u;
    _order = 0;

    S[cen].resize(cap * 2 + 1); E[cen].resize(cap * 2 + 1); E2V[cen].resize(cap);
    S[cen].set_loads({0.2, 0.4}); E[cen].set_loads({0.2, 0.4}); E2V[cen].set_loads({0.2, 0.4});
    V.clear(); V.resize(cap * 2 + 1);
    dfs(cen, p, cen, 0);
    T[cen] = SegTree(V);
    // test
    // for (int i = 0; i < V.size(); ++i) {
    //     cout << format("V[{}] = {}\n", i, V[i]);
    // } cout << flush;

    for (auto &[v, j, d] : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}

struct MyHeap {
    priority_queue<ll> pq, del; int sz;
    void push(ll v) { pq.push(v); ++sz; }
    void erase(ll v) { del.push(v); --sz; }
    void _update() {
        while (!del.empty() && !pq.empty() && pq.top() == del.top()) {
            pq.pop(); del.pop();
        }
    }
    int size() { return sz; }
    ll top() {
        _update();
        return pq.top();
    }
    ll pop() {
        _update();
        ll ret = pq.top(); pq.pop(); --sz;
        return ret;
    }
};

// 각 센트로이드에서의 마지막 정답
ll prev_ans[MAXN];
// 각 센트로이드에서의 정답들
// multiset<ll> ans;
MyHeap ans;
// 센트로이드의 인접 노드 서브트리에서의 정답들
// multiset<ll> ans_subtree[MAXN];
MyHeap ans_subtree[MAXN];

template<typename T> void erase_one(multiset<T> &s, T v) {
    auto it = s.find(v);
    // assert(it != s.end());
    s.erase(it);
}

void update_ans(int u) {
    if (prev_ans[u] != LLONG_MIN) {
        // erase_one(ans, prev_ans[u]);
        ans.erase(prev_ans[u]);
    }

    if (ans_subtree[u].size() >= 2) {
        // ll mx = *ans_subtree[u].rbegin();
        // ll smx = *next(ans_subtree[u].rbegin());
        // ans.insert(mx + smx);
        ll mx = ans_subtree[u].pop();
        ll smx = ans_subtree[u].top();
        ans.push(mx + smx);
        ans_subtree[u].push(mx);
        prev_ans[u] = mx + smx;
    } else if (ans_subtree[u].size() == 1) {
        // ll mx = *ans_subtree[u].rbegin();
        // ans.insert(mx);
        ll mx = ans_subtree[u].top();
        ans.push(mx);
        prev_ans[u] = mx;
    } else {
        assert(0);
    }
}

void init(int cen) {
    auto &sg = T[cen];
    for (auto it = C2R[cen].begin(); it != C2R[cen].end(); ++it) {
        auto &[sub_cen, sub_root] = *it;
        auto res = sg.query(S[cen][sub_root], E[cen][sub_root]);
        // ans_subtree[cen].insert(res.lmx);
        ans_subtree[cen].push(res.lmx);
    }
    if (C2R[cen].size() > 0) {
        update_ans(cen);
    }
}

// j번째 간선의 가중치를 v로 업데이트한다
void update(int j, ll udt) {
    int u = E2L[j];
    ll delta = udt - prev_w[j];
    int v = u, p = u;
    while (1) {
        auto &sg = T[v];
        if (v == u) {
            // do nothing
        } else {
            int subtree_root = C2R[v][p];
            auto res = sg.query(S[v][subtree_root], E[v][subtree_root]);
            // erase_one(ans_subtree[v], res.lmx);
            ans_subtree[v].erase(res.lmx);
            sg.update_add(S[v][u], delta);
            sg.update_add(E[v][u], -delta);
            res = sg.query(S[v][subtree_root], E[v][subtree_root]);
            // ans_subtree[v].insert(res.lmx);
            ans_subtree[v].push(res.lmx);
            update_ans(v);
        }

        if (cen_par[v] == 0) break;
        p = v;
        v = cen_par[v];
        u = E2V[v][j];
    }
    prev_w[j] = udt;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    auto start = chrono::high_resolution_clock::now();
    string fnum = "4.016";
    freopen(format("diameter-full/{}.in", fnum).c_str(), "r", stdin);
    ifstream fout(format("diameter-full/{}.out", fnum));
    #endif

    fill(prev_ans, prev_ans + MAXN, LLONG_MIN);

    int N, Q; ll W; cin >> N >> Q >> W;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; ll w; cin >> u >> v >> w;
        G[u].emplace_back(v, i, w);
        G[v].emplace_back(u, i, w);
        prev_w[i] = w;
    }

    decompose();

    for (int i = 1; i <= N; ++i) {
        init(i);
    }

    // cout << *ans.rbegin() << "\n";

    ll last = 0;
    int counter = 0;
    while (Q--) {
        int d; ll e; cin >> d >> e;
        d = (d + last) % (N - 1);
        e = (e + last) % W;


        update(d, e);
        // cout << (last = *ans.rbegin()) << "\n";
        cout << (last = ans.top()) << "\n";
        // cout << (last = ans.find_last()->data) << "\n";

        // int j; ll w; cin >> j >> w;
        // update(j, w);
        // cout << format("{}\n", *ans.rbegin());
        // cout << flush;

        #ifndef ONLINE_JUDGE
        ll a; fout >> a;
        if (a != last) {
            cout << format("Error: {} != {}\n", a, last);
            cout << flush;
            return 1;
        }
        #endif
    }

    #ifndef ONLINE_JUDGE
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << format("Execution time: {} ms\n", duration.count());
    #endif


    // test
    // for (int i = 1; i <= N; ++i) {
    //     cout << format("S[1][{}] = {}\n", i, S[1][i]);
    //     cout << format("E[1][{}] = {}\n", i, E[1][i]);
    //     cout << format("E2V[1][{}] = {}\n", i, E2V[1][i]);
    // }
    // cout << flush;

    // while (1) {
    //     int v; cin >> v;
    //     if (v == 0) break;
    //     auto ret = T[1].query(S[1][v], E[1][v]);
    //     cout << format("{}(lmx, psum) = ({}, {})\n", v, ret.lmx, ret.psum);
    //     cout << flush;
    // }

    return 0;
}