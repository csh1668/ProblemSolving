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

    LiChaoTree(ll s, ll e) {
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

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int Q; cin >> Q;
    LiChaoTree L(-1e12, 1e12);

    while (Q--) {
        int op; cin >> op;
        if (op == 1) {
            ll a, b; cin >> a >> b;
            L.insert({a, b});
        } else {
            ll x; cin >> x;
            cout << L.query(x) << '\n';
        }
    }

    return 0;
}