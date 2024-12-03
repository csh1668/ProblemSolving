#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
// typedef tuple<ll, ll, int> line; // x, y, idx

struct line {
    ll a, b;
    int idx;
    line(ll a, ll b, int idx) : a(a), b(b), idx(idx) {}
};

deque<line> lines;

double inter(int a, int b) {
    return (double)(lines[b].b - lines[a].b) / (double)(lines[a].a - lines[b].a);
}

ll eval(int i, ll x) {
    return lines[i].a * x + lines[i].b;
}

void add(line l) {
    lines.push_back(l);
    int n;
    while ((n = lines.size()) >= 3 && inter(n - 2, n - 1) < inter(n - 3, n - 2)) {
        lines[n - 2] = lines[n - 1];
        lines.pop_back();
    }
}

int query(int x) {
    while (lines.size() >= 2 && eval(0, x) <= eval(1, x)) lines.pop_front();
    return 0;
    // int s = 0, e = lines.size() - 1;
    // while (s < e) {
    //     int m = (s + e) / 2;
    //     if (inter(m, m + 1) < x) s = m + 1;
    //     else e = m;
    // }
    // return e;
}

ll dp[2][100005], a[100005];
int pre[205][100005];
vector<int> ans;


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, K; cin >> N >> K;
    for (int i = 1; i <= N; i++) cin >> a[i], a[i] += a[i - 1];

    for (int k = 1; k <= K; ++k) {
        auto &cur = dp[k % 2], &prev = dp[(k + 1) % 2];
        lines.clear();
        add({a[1], prev[0] - a[1] * a[1], 1});

        for (int i = 2; i <= N; ++i) {
            int j = query(a[i]);
            auto& [x, y, idx] = lines[j];
            cur[i] = a[i] * a[idx] + prev[idx] - a[idx] * a[idx];
            pre[k][i] = idx;
            if (a[i] - a[i - 1] == 0) continue;
            add({a[i], prev[i] - a[i] * a[i], i});
        }
    }

    int p = N;
    ll res = dp[K % 2][p];
    for (int k = K; k >= 1; --k) {
        p = pre[k][p];
        ans.push_back(p);
    }
    reverse(ans.begin(), ans.end());
    for (int i = 0; i < K; ++i) {
        if (!ans[i]) ans[i] = 1;
        if (i && ans[i] <= ans[i - 1]) ans[i] = ans[i - 1] + 1;
    }

    cout << res << '\n';
    for (auto& x : ans) cout << x << ' ';

    return 0;
}