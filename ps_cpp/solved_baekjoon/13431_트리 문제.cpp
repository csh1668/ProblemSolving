#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 100'005, LOG = 20;

vector<pair<int, int>> G[MAXN];
int dep[MAXN], par[MAXN][LOG], cen_par[MAXN], sz[MAXN], cnt[MAXN];
bool vis[MAXN], color[MAXN];
ll dist[MAXN][LOG], ans[MAXN];
unordered_map<int, ll> mp[MAXN];

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

void decompose(int u, int p) {
    int cen = get_centroid(u, -1, get_size(u, -1) / 2);
    cen_par[cen] = p;
    vis[cen] = true;
    for (auto &[v, w] : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}

void init_table(int u, int p, ll w = 0) {
    dep[u] = dep[p] + 1;
    dist[u][0] = w;
    par[u][0] = p;
    for (int i = 1; i < LOG; ++i) {
        par[u][i] = par[par[u][i - 1]][i - 1];
        dist[u][i] = dist[u][i - 1] + dist[par[u][i - 1]][i - 1];
    }
    for (auto &[v, w] : G[u]) {
        if (v == p) continue;
        init_table(v, u, w);
    }
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = LOG - 1; i >= 0; --i) {
        if (dep[u] - dep[v] >= (1 << i)) u = par[u][i];
    }
    if (u == v) return u;
    for (int i = LOG - 1; i >= 0; --i) {
        if (par[u][i] != par[v][i]) {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}

ll query_dist(int u, int v) {
    ll ret = 0;
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = LOG - 1; i >= 0; --i) {
        if (dep[u] - dep[v] >= (1 << i)) {
            ret += dist[u][i];
            u = par[u][i];
        }
    }
    if (u == v) return ret;
    for (int i = LOG - 1; i >= 0; --i) {
        if (par[u][i] != par[v][i]) {
            ret += dist[u][i] + dist[v][i];
            u = par[u][i];
            v = par[v][i];
        }
    }
    return ret + dist[u][0] + dist[v][0];
}

void update(int u) {
    if (color[u]) return;
    color[u] = true;
    cnt[u]++;
    int cur = cen_par[u], prev = u;
    while (cur != prev) {
        ll d = query_dist(u, cur);
        ans[cur] += d;
        mp[cur][prev] += d;
        cnt[cur]++;
        prev = cur;
        cur = cen_par[cur];
    }
}

ll query(int u) {
    ll ret = ans[u];
    int cur = cen_par[u], prev = u;
    while (cur != prev) {
        ret += ans[cur] - mp[cur][prev] + (cnt[cur] - cnt[prev]) * query_dist(u, cur);
        prev = cur;
        cur = cen_par[cur];
    }
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    for (int i = 0; i < N - 1; ++i) {
        int u, v, w; cin >> u >> v >> w; ++u, ++v;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    decompose(1, 0);
    init_table(1, 0);

    while (Q--) {
        int op, x; cin >> op >> x; ++x;
        if (op == 1) update(x);
        else cout << query(x) << '\n';
    }

    return 0;
}