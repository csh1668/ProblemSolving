#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 200'003, LOG = 21;

int dp[LOG][MAXN];

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int m, Q; cin >> m;
    for (int i = 1; i <= m; ++i) {
        cin >> dp[0][i];
    }
    for (int j = 1; j < LOG; ++j) {
        for (int i = 1; i <= m; ++i) {
            dp[j][i] = dp[j - 1][dp[j - 1][i]];
        }
    }

    cin >> Q;
    while (Q--) {
        int n, x; cin >> n >> x;
        while (n) {
            int j = __lg(n);
            x = dp[j][x];
            n -= (1 << j);
        }
        cout << x << '\n';
    }

    return 0;
}