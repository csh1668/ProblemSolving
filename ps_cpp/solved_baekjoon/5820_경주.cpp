#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 200'005, MAXK = 1'000'005;

int K, sz[MAXN], vis[MAXN], len[MAXK], ans = MAXN;
vector<pair<int, int>> G[MAXN];

int get_size(int u, int p) {
    sz[u] = 1;
    for (const auto& [v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (const auto& [v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

void dfs1(int u, int p, int dist, int cnt) {
    if (dist > K) return;
    ans = min(ans, len[K - dist] + cnt);
    for (const auto& [v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        dfs1(v, u, dist + w, cnt + 1);
    }
}

vector<int> tmp;
void dfs2(int u, int p, int dist, int cnt) {
    if (dist > K) return;
    len[dist] = min(len[dist], cnt);
    tmp.push_back(dist);
    for (const auto& [v, w] : G[u]) {
        if (v == p || vis[v]) continue;
        dfs2(v, u, dist + w, cnt + 1);
    }
}

void solve(int u) {
    int cap = get_size(u, -1) / 2;
    int cen = get_centroid(u, -1, cap);
    vis[cen] = 1;

    // fill(len, len + K + 1, MAXN);
    for (int i : tmp) len[i] = MAXN;
    tmp.clear();
    len[0] = 0;

    for (const auto& [v, w] : G[cen]) {
        if (vis[v]) continue;
        dfs1(v, cen, w, 1);
        dfs2(v, cen, w, 1);
    }

    for (const auto& [v, w] : G[cen]) {
        if (vis[v]) continue;
        solve(v);
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N >> K;
    for (int i = 0; i < N - 1; i++) {
        int u, v, w; cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    fill(len, len + K + 1, MAXN);
    solve(0);

    cout << (ans == MAXN ? -1 : ans) << '\n';

    return 0;
}