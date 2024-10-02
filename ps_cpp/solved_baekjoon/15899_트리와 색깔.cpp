#include <bits/stdc++.h>

using namespace std;

typedef long long ll;
constexpr int SZ = 200'005, MOD = 1'000'000'007, SGSZ = 1 << (33 - __builtin_clz(SZ));

int in[SZ], out[SZ], color[SZ], arr[SZ];
vector<int> adj[SZ];

int _order = 0;
void dfs(int u) {
    in[u] = ++_order;
    arr[_order] = color[u];
    for (auto& v : adj[u]) {
        if (!in[v]) {
            dfs(v);
        }
    }
    out[u] = _order;
}

vector<int> mst[SGSZ];

vector<int>& init(int s, int e, int i = 1) {
    if (s == e) {
        mst[i].push_back(arr[s]);
        return mst[i];
    }
    int m = s + e >> 1;
    auto &l = init(s, m, i << 1), &r = init(m + 1, e, i << 1 | 1);
    mst[i].resize(l.size() + r.size());
    merge(l.begin(), l.end(), r.begin(), r.end(), mst[i].begin());
    return mst[i];
}

int query(int s, int e, int l, int r, int k, int i = 1) {
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return upper_bound(mst[i].begin(), mst[i].end(), k) - mst[i].begin();
    int m = s + e >> 1;
    return query(s, m, l, r, k, i << 1) + query(m + 1, e, l, r, k, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int n, m, c; cin >> n >> m >> c;
    for (int i = 1; i <= n; ++i) cin >> color[i];
    for (int i = 0; i < n - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    dfs(1);
    init(1, n);

    ll ans = 0;
    while (m--) {
        int v, c; cin >> v >> c;
        ans += query(1, n, in[v], out[v], c);
        ans %= MOD;
    }
    cout << ans << endl;

    return 0;
}