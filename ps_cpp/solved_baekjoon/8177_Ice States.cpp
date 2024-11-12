#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int SZ = 200'010, SGSZ = 1 << (33 - __builtin_clz(SZ));
constexpr ll MIN = -1e18;

struct node {
    ll lsum, rsum, mxsum, sum;
    node(ll v) : lsum(v), rsum(v), mxsum(v), sum(v) {}
    node() : lsum(MIN), rsum(MIN), mxsum(MIN), sum(0) {}
    node operator+(const node& o) const {
        node ret;
        ret.lsum = max(lsum, sum + o.lsum);
        ret.rsum = max(o.rsum, o.sum + rsum);
        ret.mxsum = max({mxsum, o.mxsum, rsum + o.lsum});
        ret.sum = sum + o.sum;
        return ret;
    }
} tree[SGSZ];

int A[SZ], loc[SZ];

node& init(int s, int e, int i = 1) {
    if (s == e) {
        loc[s] = i;
        return tree[i] = node(A[s]);
    }
    int m = (s + e) >> 1;
    return tree[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1);
}

void update_add(int idx, int v) {
    int i = loc[idx];
    tree[i] = node(tree[i].sum + v);
    while (i >>= 1) tree[i] = tree[i << 1] + tree[i << 1 | 1];
}

node query(int s, int e, int l, int r, int i = 1) {
    if (r < s || e < l) return node();
    if (l <= s && e <= r) return tree[i];
    int m = (s + e) >> 1;
    return query(s, m, l, r, i << 1) + query(m + 1, e, l, r, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, m, k, d; cin >> n >> m >> k >> d;
    fill(A, A + SZ, -k);
    init(1, n);

    while (m--) {
        int r, x; cin >> r >> x;
        update_add(r, x);
        cout << (tree[1].mxsum <= (ll)k * d ? "TAK" : "NIE") << '\n';
    }

    return 0;
}