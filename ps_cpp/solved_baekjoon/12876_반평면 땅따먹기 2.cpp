#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct LiChaoTree {
    struct Line {
        ll a, b;
        Line(ll a = 0, ll b = LLONG_MIN) : a(a), b(b) {}
        ll operator()(ll x) { return a * x + b; }
    };
    struct Node {
        int l, r; // 자식 노드의 인덱스
        ll s, e; // 해당 노드가 커버하는 x 좌표 범위
        Line f; // 직선 y = ax + b
        Node(ll s, ll e) : s(s), e(e), l(-1), r(-1) {
            f = Line();
        }
    };
    vector<Node> T;

    LiChaoTree(ll s = -2e9, ll e = 2e9) {
        T.emplace_back(s, e);
    }

    void insert(Line v, int i = 0) {
        ll s = T[i].s, e = T[i].e, m = (s + e) >> 1;

        Line lo = T[i].f, hi = v;
        if (lo(s) > hi(s)) swap(lo, hi);

        if (lo(e) <= hi(e)) {
            T[i].f = hi;
            return;
        }

        if (lo(m) <= hi(m)) {
            T[i].f = hi;
            if (T[i].r == -1) {
                T[i].r = T.size();
                T.emplace_back(m + 1, e);
            }
            insert(lo, T[i].r);
        } else {
            T[i].f = lo;
            if (T[i].l == -1) {
                T[i].l = T.size();
                T.emplace_back(s, m);
            }
            insert(hi, T[i].l);
        }
    }

    ll query(ll x, int i = 0) {
        if (i == -1) return LLONG_MIN;
        ll s = T[i].s, e = T[i].e, m = (s + e) >> 1;
        if (x <= m) return max(T[i].f(x), query(x, T[i].l));
        return max(T[i].f(x), query(x, T[i].r));
    }
};

struct OfflineLiChaoTree {
    vector<LiChaoTree> LCTs;
    vector<tuple<int, int, LiChaoTree::Line>> UDTs;
    vector<tuple<int, ll>> Qs;
    vector<ll> ans;
    int order = 0;

    OfflineLiChaoTree(int q) : LCTs(1 << (33 - __builtin_clz(q))) {}

    void _insert(int l, int r, int ql, int qr, LiChaoTree::Line v, int i = 1) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            LCTs[i].insert(v);
            return;
        }
        int m = ((ll)l + r) >> 1;
        _insert(l, m, ql, qr, v, i << 1);
        _insert(m + 1, r, ql, qr, v, i << 1 | 1);
    }

    ll _query(int l, int r, int idx, ll x, int i = 1) {
        if (idx < l || r < idx) return LLONG_MIN;
        ll ret = LCTs[i].query(x);
        if (l == r) return ret;
        int m = ((ll)l + r) >> 1;
        return max({ret, _query(l, m, idx, x, i << 1), _query(m + 1, r, idx, x, i << 1 | 1)});
    }

    int insert(LiChaoTree::Line v) {
        UDTs.emplace_back(++order, -1, v);
        return UDTs.size() - 1;
    }

    void remove(int i) { // 1-based
        get<1>(UDTs[i]) = ++order;
    }

    void query(ll x) {
        Qs.emplace_back(++order, x);
    }

    void process() {
        for (auto &[l, r, v] : UDTs) {
            _insert(1, order, l, r == -1 ? order : r, v);
        }

        ans.reserve(Qs.size());
        for (auto &[i, x] : Qs) {
            ans.push_back(_query(1, order, i, x));
        }
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    OfflineLiChaoTree T(N + 5);

    unordered_map<int, int> mp;
    for (int i = 1; i <= N; ++i) {
        int op; cin >> op;
        if (op == 1) {
            ll a, b; cin >> a >> b;
            // T.insert({a, b});
            mp[i] = T.insert({a, b});
        } else if (op == 2) {
            int i; cin >> i;
            T.remove(mp[i]);
        } else {
            ll x; cin >> x;
            T.query(x);
        }
    }

    T.process();
    
    for (ll a : T.ans) {
        if (a == LLONG_MIN) cout << "EMPTY\n";
        else cout << a << '\n';
    }

    return 0;
}