#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

struct Edge { int u, v; ll w; };

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    vector<int> X(N + 1), Y(N + 1);
    vector<vector<int>> Z(N + 1, vector<int>(N + 1));
    for (int i = 1; i <= N; ++i) cin >> X[i];
    for (int i = 1; i <= N; ++i) cin >> Y[i];
    for (int i = 1; i <= N; ++i) for (int j = 1; j <= N; ++j) cin >> Z[i][j];

    vector<Edge> edges; // 0번이 루트 정점
    for (int i = 1; i <= N; ++i) {
        edges.emplace_back(0, i, Y[i]);
        for (int j = 1; j <= N; ++j) {
            if (i == j) continue;
            edges.emplace_back(i, j, Z[i][j]);
        }
    }

    ll ans = 0;
    vector<ll> min_w(N + 1);
    vector<int> par(N + 1), comp(N + 1);
    int cur_N = N;
    while (1) {
        fill(min_w.begin(), min_w.end(), LLONG_MAX);
        for (const auto &[u, v, w] : edges) {
            if (u != v && w < min_w[v]) {
                min_w[v] = w;
                par[v] = u;
            }
        }
        min_w[0] = 0;
        for (int i = 1; i <= cur_N; ++i) ans += min_w[i];

        fill(comp.begin(), comp.end(), -1);
        int cnt = 0;
        comp[0] = cnt++;
        for (int i = 0; i <= cur_N; ++i) {
            if (comp[i] != -1) continue;
            int cur = i;
            // 사이클 찾기
            while (comp[cur] == -1) {
                comp[cur] = -2; // 방문했음을 표시
                cur = par[cur];
            }

            if (comp[cur] == -2) { // 사이클을 발견할 경우
                while (comp[cur] == -2) {
                    comp[cur] = cnt; // 모두 같은 컴포넌트로 묶음
                    cur = par[cur];
                }
                ++cnt;
            }

            cur = i;
            while (comp[cur] == -2) {
                comp[cur] = cnt++;
                cur = par[cur];
            }
        }

        if (cnt == cur_N + 1) break;

        cur_N = cnt - 1;
        for (auto &[u, v, w] : edges) {
            w -= min_w[v];
            u = comp[u];
            v = comp[v];
            assert(u < cnt && v < cnt);
        }
        // root = comp[root];
    }

    for (int i = 1; i <= N; ++i) {
        int min_cost = Y[i];
        for (int j = 1; j <= N; ++j) {
            min_cost = min(min_cost, Z[j][i]);
        }
        ans += min_cost * (ll)(X[i] - 1);
    }

    cout << ans;

    return 0;
}