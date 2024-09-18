#include <bits/stdc++.h>

using namespace std;
typedef long long ll;

constexpr int sz = 100'001, sq = 317, mx = 1 << 20 + 1;

int a[sz], cnt[mx], k;
ll ans[sz], res;

struct query {
    int l, r, idx;
    bool operator<(const query &q) const {
        return l / sq == q.l / sq ? r < q.r : l / sq < q.l / sq;
    }
} q[sz];

inline void add(const int& i) {
    const int& v = a[i];
    res += cnt[v ^ k];
    ++cnt[v];
}

inline void sub(const int& i) {
    const int& v = a[i];
    --cnt[v];
    res -= cnt[v ^ k];
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, m; cin >> n >> k;
    for (int i = 1; i <= n; ++i) cin >> a[i], a[i] ^= a[i - 1];
    cin >> m;
    for (int i = 0; i < m; ++i) cin >> q[i].l >> q[i].r, q[i].idx = i, --q[i].l;
    sort(q, q + m);

    int x = 0, y = -1;
    for (int i = 0; i < m; ++i) {
        auto& [l, r, idx] = q[i];
        if (!i) x = l, y = l - 1;
        while (y < r) add(++y);
        while (y > r) sub(y--);
        while (x > l) add(--x);
        while (x < l) sub(x++);

        ans[idx] = res;
    }

    for (int i = 0; i < m; ++i) cout << ans[i] << '\n';

    return 0;
}