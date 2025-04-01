#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 1'000'002, SIZE = 1 << (33 - __builtin_clz(MAXN));

ll T[SIZE], L[SIZE];

void push(int l, int r, int i) {
    assert(l <= r && i < SIZE);
    if (L[i]) {
        T[i] += L[i] * (r - l + 1);
        if (l ^ r) {
            L[i << 1] += L[i];
            L[i << 1 | 1] += L[i];
        }
        L[i] = 0;
    }
}

void _update(int l, int r, int ql, int qr, ll v, int i = 1) {
    push(l, r, i);
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        L[i] += v;
        push(l, r, i);
        return;
    }
    int m = (l + r) >> 1;
    _update(l, m, ql, qr, v, i << 1); _update(m + 1, r, ql, qr, v, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

ll _query(int l, int r, int ql, int qr, int i = 1) {
    push(l, r, i);
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) return T[i];
    int m = (l + r) >> 1;
    return _query(l, m, ql, qr, i << 1) + _query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q1, Q2; cin >> N >> Q1 >> Q2;

    auto update = [&](int l, int r, ll v) {
        _update(1, N, l, r, v);
    };
    auto query = [&](int l, int r) {
        return _query(1, N, l, r);
    };

    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        update(i, i, x);
    }

    for (int i = 0; i < Q1 + Q2; i++) {
        int q, a, b; cin >> q >> a >> b;
        if (q == 1) {
            cout << query(a, b) << '\n';
        } else {
            int c; cin >> c;
            update(a, b, c);
        }
    }

    return 0;
}