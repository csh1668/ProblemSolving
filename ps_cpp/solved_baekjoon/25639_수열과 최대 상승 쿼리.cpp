#include <bits/stdc++.h>

using namespace std;

constexpr int sz = 100001, segSz = 1 << 18, INF = 1e9;

struct node
{
    int mn, mx, rs;
    node(int v){
        mn = mx = v;
        rs = 0;
    }
    node() {
        mn = INF, mx = -INF;
        rs = 0;
    }
    node operator+(const node& o) const {
        node ret;
        ret.mn = min(mn, o.mn);
        ret.mx = max(mx, o.mx);
        ret.rs = max({rs, o.rs, o.mx - mn});
        return ret;
    }
} seg[segSz];

int arr[sz], loc[sz];

node& build(int s, int e, int i = 1){
    if (s == e){
        loc[s] = i;
        return seg[i] = node(arr[s]);
    }
    const int m = s + e >> 1;
    return seg[i] = build(s, m, i << 1) + build(m + 1, e, i << 1 | 1);
}

void update(int idx, int x){
    int i = loc[idx];
    seg[i] = node(x);
    while (i >>= 1)
        seg[i] = seg[i << 1] + seg[i << 1 | 1];
}

node find(int s, int e, int l, int r, int i = 1){
    if (r < s || e < l) return node();
    if (l <= s && e <= r) return seg[i];
    const int m = s + e >> 1;
    return find(s, m, l, r, i << 1) + find(m + 1, e, l, r, i << 1 | 1);
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, m; cin >> n;
    for (int i = 0; i < n; ++i) cin >> arr[i];
    build(0, n - 1); cin >> m;
    for (int i = 0; i < m; ++i){
        int op, a, b; cin >> op >> a >> b;
        if (op == 1){
            update(a - 1, b);
        } else {
            auto res = find(0, n - 1, a - 1, b - 1);
            cout << res.rs << '\n';
        }
    }
}