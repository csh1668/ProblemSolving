#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

typedef long long int ll;

struct node {
    node *l, *r, *p;
    int cnt, v; ll sum; bool flip;
    node(int v = 0) : l(0), r(0), p(0), cnt(1), v(v), sum(v) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { if (flip) { swap(l, r); l && (l->flip ^= 1); r && (r->flip ^= 1); flip = 0; } }
    void pull() { cnt = 1, sum = v; if (l) cnt += l->cnt, sum += l->sum; if (r) cnt += r->cnt, sum += r->sum; }
    void rotate() {
        if (is_left()) {
            if (r) r->p = p;
            p->l = r; r = p;
        } else {
            if (l) l->p = p;
            p->r = l; l = p;
        }
        if (!p->is_root()) (p->is_left() ? p->p->l : p->p->r) = this;
        auto t = p; p = t->p; t->p = this;
        t->pull(); pull();
    }
};

void splay(node* x) {
    for (; !x->is_root(); x->rotate()) {
        if (!x->p->is_root()) x->p->p->push();
        x->p->push(); x->push();
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    } x->push();
}

void access(node* x) {
    splay(x); x->r = 0; x->pull();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x; x->p->pull();
    }
}

void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c; c->pull();
}

void cut(node* x) {
    access(x); if (!x->l) return;
    x->l->p = 0; x->l = 0; x->pull();
}

node* find_root(node* x) {
    access(x); while (x->l) x = x->l;
    splay(x); return x;
}

node* find_parent(node* x) {
    access(x); x = x->l; if (!x) return 0;
    while (x->r) x = x->r;
    splay(x); return x;
}

node* find_lca(node* u, node* v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
}

void make_root(node* x) {
    access(x); splay(x);
    x->flip ^= true;
}

void cut(node* u, node* v) {
    make_root(u);
    cut(v);
}

bool connected(node* u, node* v) {
    make_root(u);
    return find_root(v) == u;
}

ll query(node* u, node* v) {
    auto lca = find_lca(u, v);
    ll ret = lca->v;
    access(u); splay(lca);
    if (lca->r) ret += lca->r->sum;
    access(v); splay(lca);
    if (lca->r) ret += lca->r->sum;
    return ret;
}

struct UnionFind {
    vector<int> par, sz, val;
    UnionFind(int n) : par(n), sz(n, 1), val(n) {
        iota(par.begin(), par.end(), 0);
    }
    int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }
    bool merge(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return false;
        if (sz[x] < sz[y]) swap(x, y);
        par[y] = x; sz[x] += sz[y]; val[x] += val[y];
        return true;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    freopen("test.txt", "r", stdin);
    freopen("test_o.txt", "w", stdout);
    #endif

    int N, M; cin >> N >> M;
    vector<short> H(N + 1);
    UnionFind uf(N + 1);
    vector<node*> V(N + 1);
    ll hard = 0;

    for (int i = 1; i <= N; ++i) cin >> H[i], V[i] = new node(H[i]), uf.val[i] = H[i], hard += H[i];
    
    for (int i = 0; i < M; ++i) {
        int a, b; cin >> a >> b;
        uf.merge(a, b);
    }

    int Q; cin >> Q;
    auto one = V[1];

    set<pair<int, int>> S;

    while (Q--) {
        int p, a, b; cin >> p >> a >> b; if (a > b) swap(a, b);
        auto u = V[a], v = V[b];
        if (p == 1) {
            uf.merge(a, b);
        } else if (p == 2) {
            if (uf.find(a) != uf.find(b) || connected(u, v)) {
                cout << "-1\n";
            } else {
                make_root(u); link(u, v);

                S.insert({a, b});
            }
        } else if (p == 3) {
            if (S.count({a, b})) {
                cut(u, v);
                S.erase({a, b});
            } else cout << "-1\n";
        } else if (p == 4) {
            cout << hard - uf.val[uf.find(1)] << '\n';
        } else if (p == 5) {
            int ans1 = 0, ans2 = 0;
            if (uf.find(1) == uf.find(a)) ans1 += H[a];
            if (uf.find(1) == uf.find(b)) ans2 += H[b];
            if (connected(one, u)) ans1 <<= 1;
            if (connected(one, v)) ans2 <<= 1;
            cout << ans1 + ans2 << '\n';
        } else if (p == 6) {
            if (!connected(u, v)) {
                cout << "-1\n";
            } else if (uf.find(1) != uf.find(a)) {
                cout << 0 << '\n';
            } else {
                int ans = query(u, v);
                if (connected(one, u)) ans <<= 1;
                cout << ans << '\n';
            }
        } else assert(0);
    }

    return 0;
}