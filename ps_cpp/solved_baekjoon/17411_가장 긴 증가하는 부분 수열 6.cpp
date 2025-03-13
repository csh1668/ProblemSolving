#include <bits/stdc++.h>

using namespace std;

constexpr int MAXN = 1'000'005, MOD = 1'000'000'007, SIZE = 1 << (33 - __builtin_clz(MAXN));

struct SegTree {
    struct Node {
        int len, cnt;
        Node(int len = 0, int cnt = 0) : len(len), cnt(cnt) {}
        Node operator+(const Node &o) const {
            if (len > o.len) return *this;
            if (len < o.len) return o;
            return Node(len, (cnt + o.cnt) % MOD);
        }
    } T[SIZE];
    SegTree() { fill(T, T + SIZE, Node()); }
    void add(int l, int r, int pos, Node v, int i = 1) {
        if (pos < l || r < pos) return;
        if (l == r) { T[i] = T[i] + v; return; }
        int m = (l + r) >> 1;
        add(l, m, pos, v, i << 1), add(m + 1, r, pos, v, i << 1 | 1);
        T[i] = T[i << 1] + T[i << 1 | 1];
    }
    Node query(int l, int r, int ql, int qr, int i = 1) {
        if (r < ql || qr < l) return Node();
        if (ql <= l && r <= qr) return T[i];
        int m = (l + r) >> 1;
        return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
    }
} S;

int A[MAXN], B[MAXN];

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 1; i <= N; ++i) cin >> A[i], B[i] = A[i];

    // 좌표 압축
    auto B_begin = B + 1, B_end = B + N + 1;
    sort(B_begin, B_end); B_end = unique(B_begin, B_end);
    for (int i = 1; i <= N; ++i) A[i] = lower_bound(B_begin, B_end, A[i]) - B_begin + 1;

    for (int i = 1; i <= N; ++i) {
        auto P = S.query(1, N, 1, A[i] - 1);
        S.add(1, N, A[i], SegTree::Node(P.len + 1, P.cnt == 0 ? 1 : P.cnt));
    }

    auto ans = S.query(1, N, 1, N);
    cout << ans.len << ' ' << ans.cnt << '\n';
}
