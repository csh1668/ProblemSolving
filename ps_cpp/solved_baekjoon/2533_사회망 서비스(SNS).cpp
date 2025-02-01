#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 1'000'001;

vector<int> G[MAXN];
int dp[MAXN][2]; // dp[i][0]: i번 노드가 얼리어답터가 아닐 때, dp[i][1]: i번 노드가 얼리어답터일 때 i를 루트로 하는 서브트리의 최소 얼리어답터 수

// void dfs(int u, int p = -1) {
//     dp[u][0] = 0;
//     dp[u][1] = 1;
//     for (auto &v : G[u]) {
//         if (v == p) continue;
//         dfs(v, u);
//         dp[u][0] += dp[v][1];
//         dp[u][1] += min(dp[v][0], dp[v][1]);
//     }
// }

void solve() {
    vector<tuple<int, int, int>> st; st.reserve(MAXN << 1);
    st.emplace_back(1, -1, 0);
    st.emplace_back(1, -1, 1);

    while (!st.empty()) {
        auto [u, p, state] = st.back(); st.pop_back();
        if (state) {
            dp[u][0] = 0;
            dp[u][1] = 1;
            for (auto &v : G[u]) {
                if (v == p) continue;
                st.emplace_back(v, u, 0);
                st.emplace_back(v, u, 1);
            }
        } else { // post-visit
            for (auto &v : G[u]) {
                if (v == p) continue;
                dp[u][0] += dp[v][1];
                dp[u][1] += min(dp[v][0], dp[v][1]);
            }
        }
    }
}


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 0 ; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    // dfs(1);
    solve();
    cout << min(dp[1][0], dp[1][1]) << '\n';

    return 0;
}