#include <bits/stdc++.h>

using namespace std;

int N, A, B, ans;
vector<pair<int, int>> G[100001];

void dfs(int u = A, int p = -1, int d = 0, int mx = 0) {
    if (u == B) {
        ans = d - mx;
        return;
    }

    for (auto &[v, w] : G[u]) {
        if (v == p) continue;
        dfs(v, u, d + w, max(mx, w));
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> A >> B;
    for (int i = 0; i < N - 1; ++i) {
        int u, v, w; cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    dfs();

    cout << ans << '\n';

    return 0;
}