#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAX = 100'005, SIZE = 1 << (33 - __builtin_clz(MAX));

struct GCD {
    ll operator()(ll a, ll b) { return gcd(a, b); }
};

template <typename T>
struct LazySegTree {
    ll tree[SIZE], lazy[SIZE];
    int start, end;
    T func;

    LazySegTree(int s, int e) {
        start = s;
        end = e;
        fill(tree, tree + SIZE, 0);
        fill(lazy, lazy + SIZE, 0);
    }

    void push(int s, int e, int i) {
        if (lazy[i] == 0) return;
        tree[i] += (e - s + 1) * lazy[i];
        if (s ^ e) {
            lazy[i << 1] += lazy[i];
            lazy[i << 1 | 1] += lazy[i];
        }
        lazy[i] = 0;
    }

    void _update(int s, int e, int l, int r, ll v, int i = 1) {
        push(s, e, i);
        if (r < s || e < l) return;
        if (l <= s && e <= r) {
            lazy[i] += v;
            push(s, e, i);
            return;
        }
        int m = (s + e) >> 1;
        _update(s, m, l, r, v, i << 1); _update(m + 1, e, l, r, v, i << 1 | 1);
        tree[i] = func(tree[i << 1], tree[i << 1 | 1]);
    }

    void update(int l, int r, ll v) { _update(start, end, l, r, v); }

    ll _query(int s, int e, int l, int r, int i = 1) {
        push(s, e, i);
        if (r < s || e < l) return 0;
        if (l <= s && e <= r) return tree[i];
        int m = (s + e) >> 1;
        return func(_query(s, m, l, r, i << 1), _query(m + 1, e, l, r, i << 1 | 1));
    }

    ll query(int l, int r) { return _query(start, end, l, r); }
};

ll arr[MAX];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N;
    // seg1: 수열의 값을 저장
    // seg2: 수열의 값들의 gcd를 저장 (사실 LazyProp이 아님)
    // LazySegTree seg1(1, N, [](ll a, ll b) { return a + b; }), seg2(0, N + 1, [](ll a, ll b) { return gcd(a, b); });
    LazySegTree<plus<ll>> seg1(1, N);
    LazySegTree<GCD> seg2(0, N + 1);

    for (int i = 1; i <= N; ++i) {
        cin >> arr[i];
        seg1.update(i, i, arr[i]);
        seg2.update(i, i, arr[i] - arr[i - 1]);
    }
    seg2.update(N + 1, N + 1, arr[N] - arr[N - 1]);

    cin >> Q;
    while (Q--) {
        int t, a, b; cin >> t >> a >> b;
        if (t) {
            seg1.update(a, b, t);
            seg2.update(a, a, t);
            seg2.update(b + 1, b + 1, -t);
        } else {
            cout << gcd(seg1.query(a, a), seg2.query(a + 1, b)) << endl;
        }
    }

    return 0;
}