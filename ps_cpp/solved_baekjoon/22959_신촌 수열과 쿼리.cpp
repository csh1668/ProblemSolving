#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
constexpr int MAXN = 200'005, SIZE = 1 << (33 - __builtin_clz(MAXN)), INF = 1e9 + 5;

struct Node {
    ll sum; int mn;
    Node() : sum(0), mn(INF) {}
    Node(ll s, int m) : sum(s), mn(m) {}
    Node operator+(const Node& o) const {
        return {sum + o.sum, min(mn, o.mn)};
    }
} T[SIZE];

void update(int l, int r, int pos, int v, int i = 1) {
    if (l == r) {
        T[i] = {v, v};
        return;
    }
    int m = (l + r) >> 1;
    if (pos <= m) update(l, m, pos, v, i << 1);
    else update(m + 1, r, pos, v, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

Node query(int l, int r, int ql, int qr, int i = 1) {
    if (ql > r || qr < l) return Node();
    if (ql <= l && r <= qr) return T[i];
    int m = (l + r) >> 1;
    return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;
    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        update(1, N, i, x);
    }

    cin >> M;
    while (M--) {
        int op, i, j; cin >> op >> i >> j;
        if (op == 1) {
            update(1, N, i, j);
        } else {
            int lo = 1, hi = i;
            while (lo < hi) {
                int m = (lo + hi) >> 1;
                if (query(1, N, m, i).mn < j) lo = m + 1;
                else hi = m;
            }
            int left = lo;
            lo = i, hi = N;
            while (lo < hi) {
                int m = (lo + hi + 1) >> 1;
                if (query(1, N, i, m).mn >= j) lo = m;
                else hi = m - 1;
            }
            int right = lo;
            cout << query(1, N, left, right).sum << '\n';
        }
    }

    return 0;
}