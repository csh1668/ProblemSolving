#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
typedef long long int ll;
template<typename T, typename U> using hashmap = gp_hash_table<T, U, hash<T>, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;
constexpr int MAXN = 100'002;

int sz[MAXN], cen_par[MAXN];
bitset<MAXN> vis, is_black;
// ans: 정답 전역변수
// last_ans: 각 센트로이드에서의 마지막 정답
ll ans, last_ans[MAXN];
vector<int> G[MAXN];
hashmap<int, int> S[MAXN], E[MAXN];

// RB[i][j]: 센트로이드 i의 자식 노드 j의 red, black 개수
hashmap<int, pair<ll, ll>> RB[MAXN];
// RB_sum[i]: 센트로이드 i의 자식 노드들의 red * black의 합
ll RB_sum[MAXN];

// [i][j]: 센트로이드 트리 상의 i의 자식 노드 j -> 실제 트리 상에서 j가 있는 서브트리로 가는 i의 인접 자식 노드
hashmap<int, int> subcen_to_subpar[MAXN];

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

struct SegTree {
    struct Node {
        ll mn_cov, mn_cnt, child_cnt;
        Node() : mn_cov(INT_MAX), mn_cnt(0), child_cnt(0) {} 
        Node(int a, int b) : mn_cov(0), mn_cnt(a), child_cnt(b) {}
        Node operator+(const Node &o) const {
            Node ret;
            ret.mn_cov = min(mn_cov, o.mn_cov);
            ret.mn_cnt = (mn_cov == ret.mn_cov ? mn_cnt : 0) + (o.mn_cov == ret.mn_cov ? o.mn_cnt : 0);
            ret.child_cnt = (mn_cov == ret.mn_cov ? child_cnt : 0) + (o.mn_cov == ret.mn_cov ? o.child_cnt : 0);
            return ret;
        }
    };
    int N;
    vector<Node> T; vector<int> C; vector<ll> L;
    SegTree(int n = 1, const vector<int>& childs = vector<int>(1)) : N(n), T((1 << (32 - __builtin_clz(n) + 1))), L(1 << (32 - __builtin_clz(n) + 1)), C(childs) {
        _build(1, n);
    }
    Node& _build(int l, int r, int i = 1) {
        if (l == r) return T[i] = Node(1, C[l]);
        int m = (l + r) >> 1;
        return T[i] = _build(l, m, i << 1) + _build(m + 1, r, i << 1 | 1);
    }
    void _push(int l, int r, int i) {
        if (L[i] == 0) return;
        T[i].mn_cov += L[i];
        if (l ^ r) {
            L[i << 1] += L[i];
            L[i << 1 | 1] += L[i];
        }
        L[i] = 0;
    }
    void _update(int l, int r, int ql, int qr, bool cover, int i = 1) {
        _push(l, r, i);
        if (ql > r || qr < l) return;
        if (ql <= l && r <= qr) {
            if (cover) L[i]++;
            else L[i]--;
            _push(l, r, i);
            return;
        }
        int m = (l + r) >> 1;
        _update(l, m, ql, qr, cover, i << 1); _update(m + 1, r, ql, qr, cover, i << 1 | 1);
        T[i] = T[i << 1] + T[i << 1 | 1];
    }
    Node _query(int l, int r, int ql, int qr, int i = 1) {
        _push(l, r, i);
        if (ql > r || qr < l) return Node();
        if (ql <= l && r <= qr) return T[i];
        int m = (l + r) >> 1;
        return _query(l, m, ql, qr, i << 1) + _query(m + 1, r, ql, qr, i << 1 | 1);
    }
    void update(int l, int r, bool cover) { _update(1, N, l, r, cover); }
    Node query(int l, int r) { return _query(1, N, l, r); }

    pair<ll, ll> get_red_black() {
        auto res = query(1, N);
        int red = res.mn_cnt - 1; // 루트 노드 제외
        int black = res.child_cnt - res.mn_cnt + 1;
        return {red, black};
    }
    pair<ll, ll> get_red_black(int l, int r) {
        auto res = Node(1, 1) + query(l, r);
        int red = res.mn_cnt - 1; // 루트 노드 제외
        int black = res.child_cnt - res.mn_cnt + 1;
        return {red, black};
    }
} T[MAXN];

int _order = 0;
vector<int> child;
void dfs(int u, int p, int cen) {
    S[cen][u] = ++_order;
    // child[_order] = G[u].size() - 1;
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        child[_order]++;
    }
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        dfs(v, u, cen);
    }
    E[cen][u] = _order;
}

void decompose(int u = 1, int p = 0) {
    int cap = get_size(u, p);
    int cen = get_centroid(u, p, cap / 2);
    vis[cen] = true;
    cen_par[cen] = p;
    subcen_to_subpar[p][cen] = u;
    _order = 0;
    S[cen].resize(cap); E[cen].resize(cap); RB[cen].resize(cap);
    S[cen].set_loads({0.25, 0.5}); E[cen].set_loads({0.25, 0.5}); RB[cen].set_loads({0.25, 0.5});
    get_size(cen, 0);
    child.clear(); child.resize(cap + 1);

    // for (auto &v : G[cen]) {
    //     if (vis[v]) continue;
    //     int sub_cap = get_size(v, cen);
    //     int sub_cen = get_centroid(v, cen, sub_cap / 2);
    //     RB[cen][sub_cen] = {0, 0};
    //     // RB[cen][v] = {sz[v], 0};
    // }

    dfs(cen, 0, cen);
    T[cen] = SegTree(cap, child);

    for (auto &v : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}

void update_RB(int par, int u, pair<ll, ll> rb) {
    RB_sum[par] -= RB[par][u].first * RB[par][u].second;
    RB[par][u] = rb;
    RB_sum[par] += rb.first * rb.second;
}

void update(int u) {
    is_black[u] = !is_black[u];

    int v = u, p = u;
    while (1) {
        auto &st = T[v];
        auto &s = S[v], &e = E[v];
        int l = s[u], r = e[u];
        auto &last = last_ans[v];
        if (v == u) {
            // u가 현재 센트로이드의 루트인 경우 세그트리 업데이트를 하지 않는다
            auto [red, black] = st.get_red_black();
            // update_RB(v, p, {red, black});
            ans -= last;
            if (is_black[v]) {
                last = red;
            } else {
                last = (red + 1) * black - RB_sum[v];
            }
        } else {
            ans -= last;
            st.update(l, r, is_black[u]);
            auto [red, black] = st.get_red_black();
            int subtree_root = subcen_to_subpar[v][p];
            auto [sub_red, sub_black] = st.get_red_black(s[subtree_root], e[subtree_root]);
            update_RB(v, p, {sub_red, sub_black});

            if (is_black[v]) {
                last = red;
            } else {
                last = (red + 1) * black - RB_sum[v];
            }
        }

        ans += last;

        if (cen_par[v] == 0) break;
        p = v;
        v = cen_par[v];
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    decompose();

    string s; cin >> s;
    for (int i = 1; i <= N; ++i) {
        if (s[i - 1] == 'B') {
            update(i);
        }
    }

    cout << ans << '\n';
    
    int Q; cin >> Q;
    while (Q--) {
        int u; cin >> u;
        update(u);
        cout << ans << '\n';
    }

    // // test
    // int N; cin >> N;
    // vector<int> childs(N + 1);
    // for (int i = 1; i <= N; ++i) {
    //     cin >> childs[i];
    // }

    // SegTree st(N, childs);

    // cout << "1 l r: cover\n";
    // cout << "2 l r: uncover\n";
    // cout << "3 l r: query\n";
    // cout << "4: exit\n";
    // cout << "------------------------\n"; cout << flush;

    // while (1) {
    //     int op; cin >> op;
    //     if (op == 1) {
    //         int l, r; cin >> l >> r;
    //         st.update(l, r, true);
    //     } else if (op == 2) {
    //         int l, r; cin >> l >> r;
    //         st.update(l, r, false);
    //     } else if (op == 3) {
    //         int l, r; cin >> l >> r;
    //         auto res = st.query(l, r);

    //         int red = res.mn_cnt;
    //         int black = res.child_cnt - res.mn_cnt + 1;

    //         cout << format("mn_cov: {}, mn_cnt: {}, child_cnt: {}, red: {}, black: {}\n", res.mn_cov, res.mn_cnt, res.child_cnt, red, black);
    //     } else {
    //         break;
    //     }
    //     cout << flush;
    // }
    

    return 0;
}