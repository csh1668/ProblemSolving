#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 200'002, SIZE = 1 << (33 - __builtin_clz(MAXN)), INF = 1e9 + 1e5;

struct Antenna {
    int H, A, B;
} ant[MAXN];
struct Query {
    int L, R;
} q[MAXN];
vector<int> on[MAXN], off[MAXN], q_end[MAXN];
auto ans([]() { array<int, MAXN> ans; ans.fill(-1); return ans; }());

struct Node {
    int H, ans;
    Node() : H(INF), ans(-1) {}
    Node operator+(const Node &o) const {
        Node ret;
        ret.H = min(H, o.H);
        ret.ans = max(ans, o.ans);
        return ret;
    }
} T[SIZE];
auto L([]() { array<int, SIZE> L; L.fill(-INF); return L; }());

void push(int l, int r, int i) {
    if (L[i] == -INF) return;
    T[i].ans = max(T[i].ans, L[i] - T[i].H);
    if (l ^ r) {
        L[i << 1] = max(L[i << 1], L[i]);
        L[i << 1 | 1] = max(L[i << 1 | 1], L[i]);
    }
    L[i] = -INF;
}

// point update
void update_H(int l, int r, int pos, int v, int i = 1) {
    push(l, r, i);
    if (r < pos || pos < l) return;
    if (l == r) {
        T[i].H = v;
        return;
    }
    int m = l + r >> 1;
    update_H(l, m, pos, v, i << 1); update_H(m + 1, r, pos, v, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

// range update
void update_ans(int l, int r, int ql, int qr, int v, int i = 1) {
    push(l, r, i);
    if (r < ql || qr < l) return;
    if (ql <= l && r <= qr) {
        L[i] = max(L[i], v);
        push(l, r, i);
        return;
    }
    int m = l + r >> 1;
    update_ans(l, m, ql, qr, v, i << 1); update_ans(m + 1, r, ql, qr, v, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

Node query(int l, int r, int ql, int qr, int i = 1) {
    push(l, r, i);
    if (r < ql || qr < l) return Node();
    if (ql <= l && r <= qr) return T[i];
    int m = l + r >> 1;
    return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    for (int i = 1; i <= N; ++i) {
        auto &c = ant[i];
        cin >> c.H >> c.A >> c.B;

        if (i + c.A <= N) {
            on[i + c.A].push_back(i);
            off[min(i + c.B, N)].push_back(i);
        }
    }

    int M; cin >> M;
    for (int i = 1; i <= M; ++i) {
        auto &c = q[i];
        cin >> c.L >> c.R;
        q_end[c.R].push_back(i);
    }

    auto sweep = [&](bool rev) {
        for (int i = 2; i <= N; ++i) {
            for (auto &j : on[i])
                update_H(1, N, j, (rev ? -1 : 1) * ant[j].H);
            
            update_ans(1, N, max(i - ant[i].B, 0), max(i - ant[i].A, 0), (rev ? -1 : 1) * ant[i].H);

            for (auto &j : q_end[i]) {
                auto &c = q[j];
                ans[j] = max(ans[j], query(1, N, c.L, c.R).ans);
            }

            for (auto &j : off[i])
                update_H(1, N, j, INF);
        }
    };

    sweep(false);
    for (int i = 1; i < SIZE; ++i) {
        T[i] = Node();
        L[i] = -INF;
    }
    sweep(true);

    for (int i = 1; i <= M; ++i) cout << ans[i] << '\n';

    return 0;
}