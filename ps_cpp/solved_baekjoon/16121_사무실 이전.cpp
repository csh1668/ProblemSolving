#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 300'003, MOD = 998'244'353;

int sz[MAXN];
bitset<MAXN> vis, a, b;
vector<int> G[MAXN];
ll a_cnt, b_cnt, a_sum, b_sum, a_sq_sum, b_sq_sum, ans;

int get_size(int u, int p = -1) {
    sz[u] = 1;
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int cap, int p = -1) {
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, cap, u);
    }
    return u;
}

void calc(int u, ll d = 1, int p = -1) {
    if (a[u]) {
        ans = (ans + b_sq_sum + b_cnt * d * d + 2LL * b_sum * d) % MOD;
    }
    if (b[u]) {
        ans = (ans + a_sq_sum + a_cnt * d * d + 2LL * a_sum * d) % MOD;
    }
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        calc(v, d + 1, u);
    }
}

void update(int u, ll d = 1, int p = -1) {
    if (a[u]) {
        a_cnt++;
        a_sum = (a_sum + d) % MOD;
        a_sq_sum = (a_sq_sum + d * d) % MOD;
    } 
    if (b[u]) {
        b_cnt++;
        b_sum = (b_sum + d) % MOD;
        b_sq_sum = (b_sq_sum + d * d) % MOD;
    }
    for (auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        update(v, d + 1, u);
    }
}

void solve(int u = 1) {
    int cap = get_size(u) / 2;
    int cen = get_centroid(u, cap);
    vis[cen] = 1;
    a_cnt = b_cnt = a_sum = b_sum = a_sq_sum = b_sq_sum = 0;
    if (a[cen]) {
        a_cnt++;
    }
    if (b[cen]) {
        b_cnt++;
    }

    for (auto &v : G[cen]) {
        if (vis[v]) continue;
        calc(v, 1, cen);
        update(v, 1, cen);
    }

    for (auto &v : G[cen]) {
        if (vis[v]) continue;
        solve(v);
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
    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int u; cin >> u;
        a[u] = 1;
    }
    int K; cin >> K;
    for (int i = 0; i < K; ++i) {
        int u; cin >> u;
        b[u] = 1;
    }


    solve();

    cout << ans << '\n';

    return 0;
}