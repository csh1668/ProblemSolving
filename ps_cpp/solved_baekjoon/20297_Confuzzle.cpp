#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'005;

int sz[MAXN], vis[MAXN], color[MAXN], dist[MAXN], ans = MAXN;
vector<int> G[MAXN];

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

void dfs1(int u, int p, int cnt) {
    ans = min(ans, dist[color[u]] + cnt);
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        dfs1(v, u, cnt + 1);
    }
}

vector<int> tmp;
void dfs2(int u, int p, int cnt) {
    dist[color[u]] = min(dist[color[u]], cnt);
    tmp.push_back(color[u]);
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        dfs2(v, u, cnt + 1);
    }
}

void solve(int u) {
    int cap = get_size(u, -1) / 2;
    int cen = get_centroid(u, -1, cap);
    vis[cen] = 1;

    // fill(dist, dist + MAXN, MAXN);
    for (auto& i : tmp) dist[i] = MAXN;
    tmp.clear();
    dist[color[cen]] = 0;
    tmp.push_back(color[cen]);

    for (auto& v : G[cen]) {
        if (vis[v]) continue;
        dfs1(v, cen, 1);
        dfs2(v, cen, 1);
    }

    for (auto& v : G[cen]) {
        if (vis[v]) continue;
        solve(v);
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 1; i <= N; ++i) cin >> color[i];
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    fill(dist, dist + MAXN, MAXN);
    solve(1);

    cout << ans << '\n';

    return 0;
}