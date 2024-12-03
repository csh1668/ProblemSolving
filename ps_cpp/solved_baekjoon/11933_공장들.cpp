#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
typedef long long int ll;
constexpr int MAXN = 500'005, MAXQ = 100'005;
constexpr ll INF = 1e18;
const int RAND = chrono::high_resolution_clock::now().time_since_epoch().count();

inline size_t hashing(size_t x) {
    x += 0x9e3779b97f4a7c15 + RAND;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

struct my_hash {
    inline std::size_t operator()(const int& x) const {
        return hashing(x);
    }
};

template<typename T, typename U> using hashset = gp_hash_table<T, U, my_hash, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;

int sz[MAXN], vis[MAXN];
ll ans[MAXQ], A[MAXQ], B[MAXQ];
vector<pair<int, int>> G[MAXN];
vector<int> color[MAXN];
hashset<int, ll> mp;

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto &[v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto &[v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

void calc(int u, int p, ll dist) {
    for (auto& c : color[u]) {
        if (c > 0) {
            ans[c] = min(ans[c], dist + B[c]);
        } else {
            ans[-c] = min(ans[-c], dist + A[-c]);
        }
        // if (mp.find(-c) != mp.end()) {
        //     ans[abs(c)] = min(ans[abs(c)], dist + mp[-c]);
        // }
    }
    for (auto &[v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        calc(v, u, dist + w);
    }
}

vector<int> tmp;
void update(int u, int p, ll dist) {
    for (auto& c : color[u]) {
        if (c > 0) {
            A[c] = min(A[c], dist);
        } else {
            B[-c] = min(B[-c], dist);
        }
        tmp.push_back(c);
        // if (mp.find(c) != mp.end()) {
        //     mp[c] = min(mp[c], dist);
        // } else {
        //     mp[c] = dist;
        // }
    }
    for (auto &[v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        update(v, u, dist + w);
    }
}

void solve(int u) {
    int cen = get_centroid(u, -1, get_size(u, -1) / 2);
    vis[cen] = 1;

    // mp.clear();
    // for (auto& c : color[cen]) {
    //     mp[c] = 0;
    // }
    for (auto& i : tmp) {
        if (i > 0) A[i] = INF;
        else B[-i] = INF;
    }
    tmp.clear();
    for (auto& c : color[cen]) {
        if (c > 0) {
            A[c] = 0;
        } else {
            B[-c] = 0;
        }
        tmp.push_back(c);
    }

    for (auto &[v, w] : G[cen]) {
        if (vis[v]) continue;
        calc(v, cen, w);
        update(v, cen, w);
    }

    for (auto &[v, w] : G[cen]) {
        if (vis[v]) continue;
        solve(v);
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    for (int i = 0; i < N - 1; ++i) {
        int u, v, w; cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    for (int i = 1; i <= Q; ++i) {
        int sc, tc; cin >> sc >> tc;
        for (int j = 0; j < sc; ++j) {
            int c; cin >> c;
            color[c].push_back(i);
        }
        for (int j = 0; j < tc; ++j) {
            int c; cin >> c;
            color[c].push_back(-i);
        }
    }

    // mp.resize(MAXQ << 1);
    fill(ans, ans + Q + 1, INF);
    fill(A, A + MAXQ, INF);
    fill(B, B + MAXQ, INF);
    solve(0);

    for (int i = 1; i <= Q; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}