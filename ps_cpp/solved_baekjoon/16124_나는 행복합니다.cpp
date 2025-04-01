#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 1e6 + 3, MOD = 998'244'353, SIZE = 1 << (33 - __builtin_clz(MAXN));

int pw[MAXN];
// auto pw([]() {
//     array<int, MAXN> ret;
//     ret[0] = 1;
//     for (int i = 1; i < MAXN; ++i) {
//         ret[i] = (ll)ret[i - 1] * 10 % MOD;
//     }
//     return ret;
// }());

string S;

struct Node {
    // v[i]: 111...111 % MOD 형태의 계수만 저장하고 값 출력시 i를 곱하여 사용
    // cnt: 서브 노드의 개수
    int v[10], cnt;
    int eval() {
        int ret = 0;
        for (int i = 0; i < 10; ++i) {
            ret = (ret + (ll)v[i] * i) % MOD;
        }
        return ret;
    }
    Node& operator=(const int& base) {
        cnt = 1;
        v[base] = 1;
        return *this;
    }
    Node operator+(const Node &o) const {
        static Node ret;
        ret.cnt = cnt + o.cnt;
        for (int i = 0; i < 10; ++i) {
            ret.v[i] = ((ll)v[i] * pw[o.cnt] + o.v[i]) % MOD;
        }
        return ret;
    }
} T[SIZE];

struct Lazy {
    unsigned char to[10];
    Lazy() { clear(); }
    void clear() { iota(to, to + 10, 0); }
    bool is_identity() const {
        for (int i = 0; i < 10; ++i) {
            if (to[i] != i) return false;
        }
        return true;
    }
    Node apply(const Node &n) {
        static Node ret;
        ret.cnt = n.cnt;
        memset(ret.v, 0, sizeof(ret.v));
        for (int i = 0; i < 10; ++i) {
            ret.v[to[i]] = (ret.v[to[i]] + n.v[i]) % MOD;
        }
        return ret;
    }
    Lazy operator+(const Lazy &o) const {
        static Lazy ret;
        for (int i = 0; i < 10; ++i) {
            ret.to[i] = o.to[to[i]];
        }
        return ret;
    }
} L[SIZE];

Node& init(int l, int r, int i = 1) {
    if (l == r) {
        return T[i] = S[l] - '0';
    }
    int m = l + r >> 1;
    return T[i] = init(l, m, i << 1) + init(m + 1, r, i << 1 | 1);
}

void push(int l, int r, int i) {
    if (L[i].is_identity()) return;
    T[i] = L[i].apply(T[i]);
    if (l ^ r) {
        L[i << 1] = L[i << 1] + L[i];
        L[i << 1 | 1] = L[i << 1 | 1] + L[i];
    }
    L[i].clear();
}

void update(int l, int r, int ql, int qr, int from, int to, int i = 1) {
    push(l, r, i);
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        L[i].to[from] = to;
        push(l, r, i);
        return;
    }
    int m = l + r >> 1;
    update(l, m, ql, qr, from, to, i << 1); update(m + 1, r, ql, qr, from, to, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

Node query(int l, int r, int ql, int qr, int i = 1) {
    push(l, r, i);
    if (qr < l || r < ql) return Node();
    if (ql <= l && r <= qr) return T[i];
    int m = l + r >> 1;
    return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    pw[0] = 1;
    for (int i = 1; i < MAXN; ++i) {
        pw[i] = (ll)pw[i - 1] * 10 % MOD;
    }

    cin >> S;
    int N = S.size(), Q; cin >> Q;

    init(0, N - 1);
    while (Q--) {
        int op, i, j; cin >> op >> i >> j; --i; --j;
        if (op == 1) {
            int from, to; cin >> from >> to;
            update(0, N - 1, i, j, from, to);
        } else {
            cout << query(0, N - 1, i, j).eval() << '\n';
        }
    }

    return 0;
}