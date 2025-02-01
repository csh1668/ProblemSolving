#pragma GCC optimize("O3")
#include <bits/stdc++.h>

using namespace std;
typedef long long ll;
constexpr int MAXN = 500'002, SEG = 1 << (33 - __builtin_clz(MAXN));
constexpr ll INF = 1e18;

struct Node {
    ll sum, mx, smx, mn, smn, cnt_mx, cnt_mn, B;
    Node(ll v = 0) : sum(v), mx(v), smx(-INF), mn(v), smn(INF), cnt_mx(1), cnt_mn(1), B(0) {}
    Node operator+(const Node &o) const {
        Node ret;
        ret.sum = sum + o.sum;
        ret.B = B + o.B;

        if (mx == o.mx) {
            ret.mx = mx;
            ret.cnt_mx = cnt_mx + o.cnt_mx;
            ret.smx = max(smx, o.smx);
        } else if (mx > o.mx) {
            ret.mx = mx;
            ret.cnt_mx = cnt_mx;
            ret.smx = max(smx, o.mx);
        } else {
            ret.mx = o.mx;
            ret.cnt_mx = o.cnt_mx;
            ret.smx = max(mx, o.smx);
        }

        if (mn == o.mn) {
            ret.mn = mn;
            ret.cnt_mn = cnt_mn + o.cnt_mn;
            ret.smn = min(smn, o.smn);
        } else if (mn < o.mn) {
            ret.mn = mn;
            ret.cnt_mn = cnt_mn;
            ret.smn = min(smn, o.mn);
        } else {
            ret.mn = o.mn;
            ret.cnt_mn = o.cnt_mn;
            ret.smn = min(mn, o.smn);
        }

        return ret;
    }
} tree[SEG];

struct Lazy {
    ll add_lazy, max_lazy, min_lazy, add_cnt, max_cnt, min_cnt;
    Lazy() : add_lazy(0), max_lazy(0), min_lazy(0), add_cnt(0), max_cnt(0), min_cnt(0) {}
} lazy[SEG];

int arr[MAXN];

Node& init(int s, int e, int i = 1) {
    if (s == e) return tree[i] = Node(arr[s]);
    int m = (s + e) >> 1;
    return tree[i] = init(s, m, i << 1) + init(m + 1, e, i << 1 | 1);
}

void push(int s, int e, int i) {
    auto &cur = tree[i], &l = tree[i << 1], &r = tree[i << 1 | 1];
    auto &lz = lazy[i], &lz_l = lazy[i << 1], &lz_r = lazy[i << 1 | 1];

    if (cur.mx == cur.mn) {
        lz.add_lazy += lz.max_lazy + lz.min_lazy;
        lz.add_cnt += lz.max_cnt + lz.min_cnt;
        lz.max_lazy = lz.min_lazy = lz.max_cnt = lz.min_cnt = 0;
    }

    if (lz.add_cnt) {
        cur.sum += lz.add_lazy * (e - s + 1);
        cur.B += lz.add_cnt * (e - s + 1);
        cur.mx += lz.add_lazy;
        cur.smx += lz.add_lazy;
        cur.mn += lz.add_lazy;
        cur.smn += lz.add_lazy;
        if (s ^ e) {
            lz_l.add_lazy += lz.add_lazy;
            lz_l.add_cnt += lz.add_cnt;
            lz_r.add_lazy += lz.add_lazy;
            lz_r.add_cnt += lz.add_cnt;
        }

        lz.add_lazy = lz.add_cnt = 0;
    }
    if (lz.max_lazy) {
        cur.sum += lz.max_lazy * cur.cnt_mn;
        cur.B += lz.max_cnt * cur.cnt_mn;
        if (cur.smx == cur.mn) cur.smx += lz.max_lazy;
        cur.mn += lz.max_lazy;
        // 자식 노드의 mn값과 현재 노드의 mn값이 같다
        // => 자식 노드의 lazy값도 업데이트해야 한다
        if (s ^ e && cur.mn == l.mn + lz.max_lazy + lz_l.add_lazy + lz_l.max_lazy) {
            lz_l.max_lazy += lz.max_lazy;
            lz_l.max_cnt += lz.max_cnt;
        }
        if (s ^ e && cur.mn == r.mn + lz.max_lazy + lz_r.add_lazy + lz_r.max_lazy) {
            lz_r.max_lazy += lz.max_lazy;
            lz_r.max_cnt += lz.max_cnt;
        }

        lz.max_lazy = lz.max_cnt = 0;
    }
    if (lz.min_lazy) {
        cur.sum += lz.min_lazy * cur.cnt_mx;
        cur.B += lz.min_cnt * cur.cnt_mx;
        if (cur.smn == cur.mx) cur.smn += lz.min_lazy;
        cur.mx += lz.min_lazy;
        if (s ^ e && cur.mx == l.mx + lz.min_lazy + lz_l.add_lazy + lz_l.min_lazy) {
            lz_l.min_lazy += lz.min_lazy;
            lz_l.min_cnt += lz.min_cnt;
        }
        if (s ^ e && cur.mx == r.mx + lz.min_lazy + lz_r.add_lazy + lz_r.min_lazy) {
            lz_r.min_lazy += lz.min_lazy;
            lz_r.min_cnt += lz.min_cnt;
        }
        
        lz.min_lazy = lz.min_cnt = 0;
    }
}

void update_add(int s, int e, int l, int r, ll d, int i = 1) {
    push(s, e, i);
    if (r < s || e < l) return;
    if (l <= s && e <= r) {
        lazy[i].add_lazy += d;
        lazy[i].add_cnt++;
        push(s, e, i);
        return;
    }
    int m = (s + e) >> 1;
    update_add(s, m, l, r, d, i << 1); update_add(m + 1, e, l, r, d, i << 1 | 1);
    tree[i] = tree[i << 1] + tree[i << 1 | 1];
}

void update_min(int s, int e, int l, int r, ll v, int i = 1) {
    push(s, e, i);
    if (r < s || e < l || tree[i].mx <= v) return;
    if (l <= s && e <= r && tree[i].smx < v) { // smx < v < mx
        lazy[i].min_lazy += (v - tree[i].mx);
        lazy[i].min_cnt++;
        push(s, e, i);
        return;
    }
    if (s == e) return;
    int m = (s + e) >> 1;
    update_min(s, m, l, r, v, i << 1); update_min(m + 1, e, l, r, v, i << 1 | 1);
    tree[i] = tree[i << 1] + tree[i << 1 | 1];
}

void update_max(int s, int e, int l, int r, ll v, int i = 1) {
    push(s, e, i);
    if (r < s || e < l || tree[i].mn >= v) return;
    if (l <= s && e <= r && tree[i].smn > v) { // mn < v < smn
        lazy[i].max_lazy += (v - tree[i].mn);
        lazy[i].max_cnt++;
        push(s, e, i);
        return;
    }
    if (s == e) return;
    int m = (s + e) >> 1;
    update_max(s, m, l, r, v, i << 1); update_max(m + 1, e, l, r, v, i << 1 | 1);
    tree[i] = tree[i << 1] + tree[i << 1 | 1];
}

ll query_sum(int s, int e, int l, int r, int i = 1) {
    push(s, e, i);
    if (r < s || e < l) return 0;
    if (l <= s && e <= r) return tree[i].sum;
    int m = (s + e) >> 1;
    return query_sum(s, m, l, r, i << 1) + query_sum(m + 1, e, l, r, i << 1 | 1);
}

ll query_min(int s, int e, int l, int r, int i = 1) {
    push(s, e, i);
    if (r < s || e < l) return INF;
    if (l <= s && e <= r) return tree[i].mn;
    int m = (s + e) >> 1;
    return min(query_min(s, m, l, r, i << 1), query_min(m + 1, e, l, r, i << 1 | 1));
}

ll query_max(int s, int e, int l, int r, int i = 1) {
    push(s, e, i);
    if (r < s || e < l) return -INF;
    if (l <= s && e <= r) return tree[i].mx;
    int m = (s + e) >> 1;
    return max(query_max(s, m, l, r, i << 1), query_max(m + 1, e, l, r, i << 1 | 1));
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    
    int N, M; cin >> N;
    for (int i = 0; i < N; ++i) cin >> arr[i];
    init(0, N - 1);

    cin >> M;
    while (M--) {
        int op, l, r; cin >> op >> l >> r; --l; --r;
        if (op == 1) {
            int x; cin >> x;
            update_min(0, N - 1, l, r, x);
        } else if (op == 2) {
            int x; cin >> x;
            update_max(0, N - 1, l, r, x);
        } else if (op == 3) {
            int x; cin >> x;
            update_add(0, N - 1, l, r, x);
        } else if (op == 4) {
            cout << query_min(0, N - 1, l, r) << '\n';
        } else if (op == 5) {
            cout << query_max(0, N - 1, l, r) << '\n';
        } else {
            cout << query_sum(0, N - 1, l, r) << '\n';
        }
    }

    return 0;
}