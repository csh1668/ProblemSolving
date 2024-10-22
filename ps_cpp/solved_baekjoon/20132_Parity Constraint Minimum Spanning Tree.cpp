#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct node {
    node *l, *r, *p, *odd_mx, *even_mx;
    int cnt, v; bool flip;
    node(int v = -1) : l(0), r(0), p(0), cnt(1), v(v), flip(0) {
        if (v == -1) odd_mx = even_mx = 0;
        else if (v & 1) odd_mx = this, even_mx = 0;
        else odd_mx = 0, even_mx = this;
    }
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), l && (l->flip ^= 1), r && (r->flip ^= 1), flip = 0); }
    void update() {
        cnt = 1;
        if (v == -1) odd_mx = even_mx = 0;
        else if (v & 1) odd_mx = this, even_mx = 0;
        else odd_mx = 0, even_mx = this;

        if (l) {
            l->push();
            cnt += l->cnt;
            if (l->odd_mx && (odd_mx == 0 || l->odd_mx->v > odd_mx->v)) {
                odd_mx = l->odd_mx;
            }
            if (l->even_mx && (even_mx == 0 || l->even_mx->v > even_mx->v)) {
                even_mx = l->even_mx;
            }
        }
        if (r) {
            r->push();
            cnt += r->cnt;
            if (r->odd_mx && (odd_mx == 0 || r->odd_mx->v > odd_mx->v)) {
                odd_mx = r->odd_mx;
            }
            if (r->even_mx && (even_mx == 0 || r->even_mx->v > even_mx->v)) {
                even_mx = r->even_mx;
            }
        }
    }
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
        t->update(); update();
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
    splay(x); x->r = 0; x->update();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x; x->p->update();
    }
}

void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c; c->update();
}

void cut(node* x) {
    access(x);
    if (!x->l) return;
    x->l = x->l->p = 0; x->update();
}

node* find_root(node* x) {
    access(x); while (x->l) x = x->l; splay(x);
    return x;
}

node* find_lca(node* u, node* v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
}

void make_root(node* x) {
    access(x); splay(x); x->flip ^= 1;
}
// {홀수 최대, 짝수 최대}
pair<node*, node*> query_max(node* u, node* v) {
    node* lca = find_lca(u, v);
    pair<node*, node*> ret;
    if (lca->v == -1) ret = {0, 0};
    else if (lca->v & 1) ret = {lca, 0};
    else ret = {0, lca};

    auto& [odd_mx, even_mx] = ret;
    access(u); splay(lca);
    if (lca->r) {
        auto r_odd_mx = lca->r->odd_mx, r_even_mx = lca->r->even_mx;
        if (r_odd_mx && (!odd_mx || r_odd_mx->v > odd_mx->v)) odd_mx = r_odd_mx;
        if (r_even_mx && (!even_mx || r_even_mx->v > even_mx->v)) even_mx = r_even_mx;
    }
    access(v); splay(lca);
    if (lca->r) {
        auto r_odd_mx = lca->r->odd_mx, r_even_mx = lca->r->even_mx;
        if (r_odd_mx && (!odd_mx || r_odd_mx->v > odd_mx->v)) odd_mx = r_odd_mx;
        if (r_even_mx && (!even_mx || r_even_mx->v > even_mx->v)) even_mx = r_even_mx;
    }
    return ret;
}

bool connected(node* u, node* v) { return find_root(u) == find_root(v); }

void link_edge(node* u, node* e, node* v) {
    make_root(u);
    link(u, e);
    make_root(v);
    link(v, e);
}

using Edge = tuple<node*, int, int>;

struct UnionFind {
    vector<int> par, sz;
    UnionFind(int n) : par(n), sz(n, 1) {
        iota(par.begin(), par.end(), 0);
    }
    int find(int x) { return x == par[x] ? x : par[x] = find(par[x]); }
    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return 0;
        if (sz[x] < sz[y]) swap(x, y);
        par[y] = x; sz[x] += sz[y];
        return 1;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    vector<node*> vertex(N + 1);
    vector<Edge> edges(M), not_used;
    for (int i = 1; i <= N; i++) vertex[i] = new node();
    for (int i = 0; i < M; ++i) {
        int u, v, w; cin >> u >> v >> w;
        edges[i] = {new node(w), u, v};
    }
    sort(edges.begin(), edges.end(), [&](const Edge& a, const Edge& b) { return get<0>(a)->v < get<0>(b)->v; });

    ll mst = 0; not_used.reserve(M);
    UnionFind uf = UnionFind(N + 1);
    for (const auto& edge : edges) {
        const auto& [e, u, v] = edge;
        if (uf.merge(u, v)) {
            link_edge(vertex[u], e, vertex[v]);
            mst += e->v;
        } else {
            not_used.push_back(edge);
        }
    }

    if (uf.sz[uf.find(1)] != N) {
        cout << "-1 -1\n";
        return 0;
    } else if (not_used.empty()) {
        if (mst & 1) cout << mst << " -1\n";
        else cout << "-1 " << mst << '\n';
        return 0;
    }

    ll min_diff = LLONG_MAX;
    for (const auto& [e, u, v] : not_used) {
        auto [odd_mx, even_mx] = query_max(vertex[u], vertex[v]);
        if (e->v & 1) {
            if (even_mx) min_diff = min(min_diff, (ll)-even_mx->v + e->v);
        } else {
            if (odd_mx) min_diff = min(min_diff, (ll)-odd_mx->v + e->v);
        }
    }

    ll mst2 = min_diff == LLONG_MAX ? -1 : mst + min_diff;

    ll odd = (mst & 1) ? mst : mst2, even = (mst & 1) ? mst2 : mst;
    cout << odd << ' ' << even << '\n';

    return 0;
}