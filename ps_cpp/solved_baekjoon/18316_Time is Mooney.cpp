// #pragma GCC optimize("O3")

#include <bits/stdc++.h>

using namespace std;

constexpr int MAX = 210;
int m[1001], dists[1001][1001];
vector<int> g[1001];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M, C; cin >> N >> M >> C;
    for (int i = 1; i <= N; ++i) cin >> m[i];
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v;
        g[u].emplace_back(v);
    }

    fill(&dists[0][0], &dists[1000][1001], -1);
    dists[1][0] = 0;
    
    for (int k = 1; k <= 1000; ++k) {
        for (int u = 1; u <= N; ++u) {
            dists[u][k] = dists[u][k - 1];
            for (int& v : g[u]) {
                if (dists[v][k - 1] != -1) {
                    dists[u][k] = max(dists[u][k], dists[v][k - 1] + m[v]);
                }
            }
        }
    }
    int ans = 0;
    for (int k = 1; k <= 1000; ++k) {
        if (dists[1][k] == -1) continue;
        ans = max(ans, dists[1][k] - C * k * k);
    }

    cout << ans;

    return 0;
}