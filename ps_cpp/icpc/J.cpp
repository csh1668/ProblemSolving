#include <bits/stdc++.h>
#include <functional>
#include <vector>
using namespace std;
using ll = long long;
typedef long long lint;


int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    
    int N, M; cin >> N >> M;
    vector<int> L(N);
    for (int i = 0; i < N; ++i) {
        cin >> L[i];
    }

    vector<bitset<1000>> adj(N);
    vector<pair<int, int>> edges;
    for (int i = 0; i < M; ++i) {
        int u, v; cin >> u >> v; u--, v--;
        adj[u][v] = 1;
        adj[v][u] = 1;
        edges.push_back({u, v});
    }

    int mx = 0;
    for (auto& [u, v] : edges) {
        auto &bu = adj[u]; auto &bv = adj[v];
        auto adj = bu & bv;
        if (adj.count() < 2) continue;
        int fmx = 0, smx = 0;
        for (int i = 0; i < N; ++i) {
            if (adj[i]) {
                int cur = L[i];
                if (cur > fmx) {
                    smx = fmx;
                    fmx = cur;
                } else if (cur == fmx) {
                    smx = fmx;
                }  else if (cur > smx) {
                    smx = cur;
                }
            }
        }
        mx = max(mx, L[u] + L[v] + fmx + smx);
    }

    if (mx == 0) cout << -1;
    else cout << mx;

    return 0;
}