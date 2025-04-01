#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'005, LOG = 20;

vector<int> G[MAXN];
priority_queue<pair<int, int>> pq[MAXN];
int dep[MAXN], par[MAXN][LOG], cen_par[MAXN], sz[MAXN], vis[MAXN], color[MAXN];

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

void decompose(int u, int p) {
    int cen = get_centroid(u, -1, get_size(u, -1) / 2);
    cen_par[cen] = p;
    vis[cen] = 1;
    for (auto &v : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}

void init_table(int u, int p) {
    dep[u] = dep[p] + 1;
    par[u][0] = p;
    for (int i = 1; i < LOG; i++) {
        par[u][i] = par[par[u][i - 1]][i - 1];
    }
    for (auto &v : G[u]) {
        if (v == p) continue;
        init_table(v, u);
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

int dist(int u, int v) {
    return dep[u] + dep[v] - 2 * dep[lca(u, v)];
}

void update(int u) {
    color[u] ^= 1;
    int v = u;
    while (1) {
        auto &pq = ::pq[v];
        if (color[u]) {
            pq.emplace(-dist(u, v), u);
        } else {
            while (!pq.empty() && !color[pq.top().second]) pq.pop();
        }
        if (v == cen_par[v]) break;
        v = cen_par[v];
    }
}

int query(int u) {
    int ret = MAXN;
    int v = u;
    while (1) {
        auto &pq = ::pq[v];
        while (!pq.empty() && !color[pq.top().second]) pq.pop();
        if (!pq.empty()) ret = min(ret, -pq.top().first + dist(u, v));
        if (v == cen_par[v]) break;
        v = cen_par[v];
    }
    return ret == MAXN ? -1 : ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].emplace_back(v);
        G[v].emplace_back(u);
    }
    
    decompose(1, 0);
    init_table(1, 0);

    cin >> Q;
    while (Q--) {
        int op, u; cin >> op >> u;
        if (op == 1) update(u);
        else cout << query(u) << '\n';
    }

    return 0;
}