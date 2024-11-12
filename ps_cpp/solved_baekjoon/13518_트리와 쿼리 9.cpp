#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 100'001, MAXW = 1'000'001, SQ = 317;
vector<int> adj[MAX], g[MAX], vertex(1);

int in[MAX], out[MAX], dep[MAX], par[MAX], weight[MAX], order = 0;
bool vis[MAX];
void dfs(int u, int p) {
    vis[u] = true;
    in[u] = ++order;
    dep[u] = dep[p] + 1;
    par[u] = p;
    vertex.emplace_back(u);
    for (auto& v : adj[u]) {
        if (v == p) continue;
        g[u].push_back(v);
        if (!vis[v]) dfs(v, u);
    }
    out[u] = ++order;
    vertex.emplace_back(u);
}

int table[MAX][18];
void init_table(int N) {
    for (int i = 1; i <= N; ++i) table[i][0] = par[i];
    for (int j = 1; j < 18; ++j) {
        for (int i = 1; i <= N; ++i) {
            table[i][j] = table[table[i][j - 1]][j - 1];
        }
    }
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = 17; i >= 0; --i) {
        if (dep[u] - dep[v] >= (1 << i)) u = table[u][i];
    }
    if (u == v) return u;
    for (int i = 17; i >= 0; --i) {
        if (table[u][i] != table[v][i]) {
            u = table[u][i];
            v = table[v][i];
        }
    }
    return table[u][0];
}

struct query {
    int x, y, idx, lca;

    bool operator<(const query& other) const {
        if (x / SQ != other.x / SQ) return x / SQ < other.x / SQ;
        return y < other.y;
    }
} queries[MAX];

int res[MAX], cnt[MAXW], on[MAXW], ans = 0;

void add(const int& k) {
    auto& v = vertex[k];
    auto& w = weight[v];
    on[v]++;
    if (on[v] == 1) {
        cnt[w]++;
        if (cnt[w] == 1) ans++;
    }
    if (on[v] == 2) {
        cnt[w]--;
        if (cnt[w] == 0) ans--;
    }
}

void sub(const int& k) {
    auto& v = vertex[k];
    auto& w = weight[v];
    on[v]--;
    if (on[v] == 1) {
        cnt[w]++;
        if (cnt[w] == 1) ans++;
    }
    if (on[v] == 0) {
        cnt[w]--;
        if (cnt[w] == 0) ans--;
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 1; i <= N; ++i) cin >> weight[i];

    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1, 0);
    init_table(N);
    
    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        if (in[u] > in[v]) swap(u, v);
        int l = lca(u, v);

        auto& [x, y, idx, lca] = queries[i];
        if (l == u) {
            x = in[u]; y = in[v]; idx = i; lca = -1;
        } else {
            assert(l != v);
            x = out[u]; y = in[v]; idx = i; lca = in[l];
        }
    }

    sort(queries, queries + M);
    int x, y;

    for (int i = 0; i < M; ++i) {
        auto& [l, r, idx, lca] = queries[i];
        if (!i) x = l, y = l - 1;
        while (y < r) add(++y);
        while (y > r) sub(y--);
        while (x > l) add(--x);
        while (x < l) sub(x++);

        if (lca != -1) add(lca);
        res[idx] = ans;
        if (lca != -1) sub(lca); 
    }

    for (int i = 0; i < M; ++i) cout << res[i] << '\n';

    return 0;
}