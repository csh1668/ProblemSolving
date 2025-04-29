#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
// #pragma GCC target("avx2")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
typedef long long int ll;
constexpr int MAXN = 200'002;
const int RAND = chrono::high_resolution_clock::now().time_since_epoch().count();

inline size_t hashing(size_t x) {
    x += 0x9e3779b97f4a7c15 + RAND;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

template<typename T> struct my_hash {
    size_t operator()(const T &x) const {
        return hashing(x);
    }
};

template<typename T, typename U> using hashmap = gp_hash_table<T, U, my_hash<T>, std::equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;


int sz[MAXN], cen_par[MAXN];
bitset<MAXN> vis;
vector<pair<int, ll>> G[MAXN], Q[MAXN];
// nodes_xor[i][d]: 센트로이드 i에서 거리가 d인 노드들을 XOR한 값
hashmap<ll, int> nodes_xor[MAXN];
// dist[i][j]: 센트로이드 i에서 j까지의 거리
hashmap<int, ll> dist[MAXN];
// subtree[i][j]: 센트로이드 i의 자식 노드 j를 루트로 하는 서브트리에서 i까지의 거리가 d인 노드들을 XOR한 값
hashmap<int, hashmap<ll, int>> subtree[MAXN];

int get_size(int u, int p = -1) {
    sz[u] = 1;
    for (auto &[v, _] : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int cap, int p = -1) {
    for (auto &[v, _] : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, cap, u);
    }
    return u;
}

void calc(int u, int p, ll d, int cen, int branch) {
    nodes_xor[cen][d] ^= u;
    subtree[cen][branch][d] ^= u;
    dist[cen][u] = d;
    for (auto &[v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        calc(v, u, d + w, cen, branch);
    }
}

void decompose(int u = 1, int p = 0) {
    int cap = get_size(u);
    int cen = get_centroid(u, cap / 2);
    vis[cen] = true;
    cen_par[cen] = p;

    nodes_xor[cen].resize(cap + 1); nodes_xor[cen].set_loads({0.25, 0.5});
    subtree[cen].resize(G[cen].size() + 1); subtree[cen].set_loads({0.25, 0.5});
    dist[cen].resize(cap + 1); dist[cen].set_loads({0.25, 0.5});
    nodes_xor[cen][0] ^= cen;
    for (auto &[v, w] : G[cen]) {
        if (vis[v]) continue;
        int sub_cap = get_size(v);
        int sub_cen = get_centroid(v, sub_cap / 2);
        subtree[cen][sub_cen].resize(sub_cap + 1); subtree[cen][sub_cen].set_loads({0.25, 0.5});
        calc(v, cen, w, cen, sub_cen);
    }

    for (auto &[v, _] : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}

int query(int u, ll d) {
    int ans = 0;
    int v = u; // 현재 센트로이드
    int p = u; // 부모 센트로이드
    while (1) {
        ll dd = d - (dist[v].find(u) != dist[v].end() ? dist[v][u] : 0);
        if (nodes_xor[v].find(dd) != nodes_xor[v].end())
            ans ^= nodes_xor[v][dd];
        if (v != p && subtree[v][p].find(dd) != subtree[v][p].end())
            ans ^= subtree[v][p][dd];

        if (cen_par[v] == 0) break;
        p = v;
        v = cen_par[v];
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; ll w; cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    decompose();

    for (int i = 0; i < M; ++i) {
        int u; ll d; cin >> u >> d;
        cout << query(u, d) << '\n';
    }

    return 0;
}