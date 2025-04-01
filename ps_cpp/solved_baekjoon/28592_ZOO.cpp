#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 100'010, SQ = 317;
vector<int> adj[MAX], g[MAX], vertex(1);

int in[MAX], out[MAX], dep[MAX], par[MAX], weight[MAX], tmp_weight[MAX], order = 0;
bool vis[MAX];
void dfs(int u, int p) {
    vis[u] = true; in[u] = ++order; dep[u] = dep[p] + 1; par[u] = p;
    vertex.emplace_back(u);
    for (auto& v : adj[u]) {
        if (v == p) continue;
        g[u].push_back(v);
        if (!vis[v]) dfs(v, u);
    }
    out[u] = ++order; vertex.emplace_back(u);
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
    int l, r, idx, lca;
    bool operator<(const query& other) const {
        if (l / SQ != other.l / SQ) return l / SQ < other.l / SQ;
        return r < other.r;
    }
} queries[MAX];

int res[MAX], cnt[MAX], ccnt[MAX], on[MAX], ans = 0;

void add_internal(int v, int w) {
    ccnt[cnt[w]]--;
    cnt[w]++;
    ccnt[cnt[w]]++;
    ans = max(ans, cnt[w]);
}

void sub_internal(int v, int w) {
    if (cnt[w] == ans && ccnt[cnt[w]] == 1) ans--;
    ccnt[cnt[w]]--;
    cnt[w]--;
    ccnt[cnt[w]]++;
}

void add(const int k) {
    auto v = vertex[k];
    auto w = weight[v];
    on[v]++;
    if (on[v] == 1) add_internal(v, w);
    else if (on[v] == 2) sub_internal(v, w);
    else assert(0);
}

void sub(const int k) {
    auto v = vertex[k];
    auto w = weight[v];
    on[v]--;
    if (on[v] == 1) add_internal(v, w);
    else if (on[v] == 0) sub_internal(v, w);
    else assert(0);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    for (int i = 1; i <= N; ++i) cin >> weight[i], tmp_weight[i] = weight[i];

    sort(tmp_weight + 1, tmp_weight + N + 1);
    for (int i = 1; i <= N; ++i) {
        weight[i] = lower_bound(tmp_weight + 1, tmp_weight + N + 1, weight[i]) - tmp_weight + 1;
    }

    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v), adj[v].push_back(u);
    }
    dfs(1, 0); init_table(N);

    for (int i = 0; i < Q; ++i) {
        int u, v; cin >> u >> v;
        if (in[u] > in[v]) swap(u, v);
        int l = lca(u, v);
        auto& [left, right, idx, lca] = queries[i];
        if (l == u) {
            left = in[u], right = in[v], lca = -1;
        } else {
            assert(l != v);
            left = out[u], right = in[v], lca = in[l];
        }
        idx = i;
    }

    sort(queries, queries + Q);

    int x, y;
    for (int i = 0; i < Q; ++i) {
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

    for (int i = 0; i < Q; ++i) cout << res[i] << '\n';
    

    return 0;
}