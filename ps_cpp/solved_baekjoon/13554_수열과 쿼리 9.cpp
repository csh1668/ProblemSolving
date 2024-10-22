#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int SZ = 100'005, SQ = 317;
ll res[SZ];
int A[SZ], B[SZ];

struct Query {
    int l, r, k, idx;
    bool operator<(const Query& o) const {
        return l / SQ == o.l / SQ ? r < o.r : l / SQ < o.l / SQ;
    }
} q[SZ];

struct Fenwick {
    ll tree[SZ];
    void update(int i, int v) { for (; i < SZ; i += i & -i) tree[i] += v; }
    ll prefix(int i) { int ret = 0; for (; i > 0; i -= i & -i) ret += tree[i]; return ret; }
    ll query(int l, int r) {
        if (l > r) return 0;
        return prefix(r) - prefix(l - 1);
    }
} fw1, fw2;

inline void add(int i, int k) {
    int &a = A[i], &b = B[i];
    fw1.update(a, 1); fw2.update(b, 1);
}

inline void sub(int i, int k) {
    int &a = A[i], &b = B[i];
    fw1.update(a, -1); fw2.update(b, -1);
}

inline ll get(int l, int r, int k) {
    ll ret = 0; int sqrt_k = sqrt(k);
    for (int i = 1; i <= sqrt_k; ++i) {
        ret += fw1.query(i, i) * fw2.query(1, k / i) + fw2.query(i, i) * fw1.query(sqrt_k + 1, k / i);
    }
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 1; i <= N; ++i) cin >> A[i];
    for (int i = 1; i <= N; ++i) cin >> B[i];
    int M; cin >> M;
    for (int i = 1; i <= M; ++i) cin >> q[i].l >> q[i].r >> q[i].k, q[i].idx = i;
    sort(q + 1, q + M + 1);

    int x, y;
    for (int i = 1; i <= M; ++i) {
        auto& [l, r, k, idx] = q[i];
        if (i == 1) x = l, y = l - 1;
        while (y < r) add(++y, k);
        while (y > r) sub(y--, k);
        while (x > l) add(--x, k);
        while (x < l) sub(x++, k);

        res[idx] = get(l, r, k);
    }

    for (int i = 1; i <= M; ++i) cout << res[i] << '\n';

    return 0;
}