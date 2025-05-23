#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'002, SZ = 1 << (33 - __builtin_clz(MAXN));

int T[SZ], L[SZ];

void push(int l, int r, int i) {
    if (L[i] == 0) return;
    T[i] = L[i];
    if (l ^ r) {
        L[i << 1] = L[i];
        L[i << 1 | 1] = L[i];
    }
    L[i] = 0;
}
void update(int l, int r, int ql, int qr, int v, int i = 1) {
    push(l, r, i);
    if (ql > r || qr < l) return;
    if (ql <= l && r <= qr) return L[i] = v, push(l, r, i);
    int m = (l + r) >> 1;
    update(l, m, ql, qr, v, i << 1); update(m + 1, r, ql, qr, v, i << 1 | 1);
    T[i] = T[i << 1] | T[i << 1 | 1];
}
int query(int l, int r, int ql, int qr, int i = 1) {
    push(l, r, i);
    if (ql > r || qr < l) return 0;
    if (ql <= l && r <= qr) return T[i];
    int m = (l + r) >> 1;
    return query(l, m, ql, qr, i << 1) | query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    fill(T, T + SZ, 1 << 0);

    int N, T, Q; cin >> N >> T >> Q;
    while (Q--) {
        char op; int x, y; cin >> op >> x >> y;
        if (x > y) swap(x, y); // 얼탱 ㅋㅋ

        if (op == 'C') {
            int z; cin >> z;
            update(1, N, x, y, 1 << (z - 1));
        } else {
            int ans = query(1, N, x, y);
            cout << __builtin_popcount(ans) << '\n';
        }
    }

    return 0;
}