#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

constexpr int sz = 100001, sq = 320, fwsz = 131072;
int n, q, arr[sz], fenwick[fwsz];
ll res[sz], ans;

inline ll findSum(int idx){
    ll ret = 0;
    while (idx) ret += fenwick[idx], idx -= idx & -idx;
    return ret;
}

inline ll findSum(int s, int e) { return findSum(e) - findSum(s - 1); }

inline void update(int idx, bool add){
    if (add) while (idx < fwsz) fenwick[idx]++, idx += idx & -idx;
    else while (idx < fwsz) fenwick[idx]--, idx += idx & -idx;
}

struct query
{
    int l, r, idx;
    bool operator<(const query &o) const {
        if (l / sq == o.l / sq) return r < o.r;
        return l / sq < o.l / sq;
    }
} queries[sz];

inline void add(const int& k, const bool left){
    const int& v = arr[k];
    update(v, true);
    if (left) ans += findSum(1, v - 1);
    else ans += findSum(v + 1, sz - 1);
}

inline void sub(const int& k, const bool left){
    const int& v = arr[k];
    update(v, false);
    if (left) ans -= findSum(1, v - 1);
    else ans -= findSum(v + 1, sz - 1);
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> n >> q;
    for (int i = 0; i < n; ++i) cin >> arr[i];
    for (int i = 0; i < q; ++i) cin >> queries[i].l >> queries[i].r, --queries[i].l, --queries[i].r, queries[i].idx = i;
    sort(queries, queries + q);

    int x, y;
    for (int i = 0; i < q; ++i){
        const int &l = queries[i].l, &r = queries[i].r, &idx = queries[i].idx;

        if (!i) x = l, y = l - 1;
        while (y < r) add(++y, false);
        while (y > r) sub(y--, false);
        while (x > l) add(--x, true);
        while (x < l) sub(x++, true);

        res[idx] = ans;
    }

    for (int i = 0; i < q; ++i) cout << res[i] << '\n';
}