#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
constexpr int MAXN = 100'005;

template<typename T, typename U> using hashmap = gp_hash_table<T, U, std::hash<T>>;

const int RAND = chrono::high_resolution_clock::now().time_since_epoch().count();

inline size_t hash_combine(size_t a, size_t b) {
    a *= 0x9e3779b97f4a7c15;
    return a ^ (b + (a << 6) + (a >> 2));
}

inline size_t hashing(size_t x) {
    x += 0x9e3779b97f4a7c15 + RAND;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

namespace std {
    template<> struct hash<pair<int, int>> {
        inline size_t operator()(const pair<int, int> &p) const {
            return hash_combine(hashing(p.first), hashing(p.second));
        }
    };
}

struct UnionFind {
    struct History { int u, v, s; };
    int par[MAXN], sz[MAXN];
    vector<History> his;

    UnionFind(int N) { init(N); }
    void init(int N) { iota(par, par + N, 0); fill(sz, sz + N, 1); his.clear(); }
    int find(int x) { return x == par[x] ? x : find(par[x]); }
    bool merge(int u, int v) {
        u = find(u), v = find(v);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        his.push_back({u, v, sz[u]});
        par[v] = u, sz[u] += sz[v];
        return true;
    }
    void rollback(int cnt = 1) {
        if (cnt == -1) cnt = his.size();
        assert(cnt <= his.size());
        while (cnt--) {
            auto [u, v, s] = his.back(); his.pop_back();
            par[v] = v, sz[u] = s;
        }
    }
} uf(MAXN);

vector<pair<int, int>> T[MAXN << 2];

void update(int l, int r, int ql, int qr, pair<int, int> v, int i = 1) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
        T[i].push_back(v);
        return;
    }
    int m = (l + r) >> 1;
    update(l, m, ql, qr, v, i << 1); update(m + 1, r, ql, qr, v, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;

    hashmap<pair<int, int>, int> mp;
    vector<tuple<int, int, int, int>> L(M), E;
    vector<pair<int, int>> Q;
    for (int i = 0; i < M; ++i) {
        int op, u, v;
        cin >> op >> u >> v;
        if (u > v) swap(u, v);

        if (op == 1) {
            mp[{u, v}] = i;
            L[i] = {u, v, Q.size(), -2};
        } else if (op == 2) {
            auto &[uu, vv, s, e] = L[mp[{u, v}]];
            e = Q.size() - 1;
            E.emplace_back(uu, vv, s, e);
        } else {
            Q.emplace_back(u, v);
        }
    }

    for (auto &[u, v, s, e] : L) if (e == -2) E.emplace_back(u, v, s, Q.size() - 1);

    for (auto &[u, v, s, e] : E) {
        update(0, Q.size() - 1, s, e, {u, v});
    }

    auto uf = UnionFind(N + 1);

    auto solve = [&](int l, int r, auto solve, int i = 1) {
        int cnt = 0;
        for (auto &[u, v] : T[i]) cnt += uf.merge(u, v);
        if (l == r) {
            auto &[u, v] = Q[l];
            cout << (uf.find(u) == uf.find(v)) << '\n';
            uf.rollback(cnt);
            return;
        } else {
            int m = (l + r) >> 1;
            solve(l, m, solve, i << 1); solve(m + 1, r, solve, i << 1 | 1);
            uf.rollback(cnt);
        }
    };

    solve(0, Q.size() - 1, solve);

    return 0;
}