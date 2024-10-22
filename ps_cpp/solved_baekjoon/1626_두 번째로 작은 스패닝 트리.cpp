#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p, *mx, *smx;
    int cnt, v; bool flip;
    node(int v = -1) : l(0), r(0), p(0), mx(this), smx(0), cnt(1), v(v), flip(0) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), l && (l->flip ^= 1), r && (r->flip ^= 1), flip = 0); }
    void update() {
        cnt = 1; mx = this; smx = 0;
        if (l) {
            l->push();
            cnt += l->cnt;
            if (l->mx->v > mx->v) smx = mx, mx = l->mx;
            if ((!smx && l->mx->v != mx->v) || (smx && l->mx->v > smx->v && l->mx->v != mx->v)) smx = l->mx;
            if ((!smx && l->smx && l->smx->v != mx->v) || (smx && l->smx && l->smx->v > smx->v)) smx = l->smx;
        }
        if (r) {
            r->push();
            cnt += r->cnt;
            if (r->mx->v > mx->v) smx = mx, mx = r->mx;
            if ((!smx && r->mx->v != mx->v) || (smx && r->mx->v > smx->v && r->mx->v != mx->v)) smx = r->mx;
            if ((!smx && r->smx && r->smx->v != mx->v) || (smx && r->smx && r->smx->v > smx->v)) smx = r->smx;
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

pair<node*, node*> query_max(node* u, node* v) {
    node* lca = find_lca(u, v);
    pair<node*, node*> ret = {lca, 0};
    access(u); splay(lca);
    if (lca->r) {
        auto& [mx, smx] = ret;
        auto lmx = lca->r->mx, lsmx = lca->r->smx;
        if (lmx->v > mx->v) smx = mx, mx = lmx;
        if ((!smx && lmx->v != mx->v) || (smx && lmx->v > smx->v && lmx->v != mx->v)) smx = lmx;
        if ((!smx && lsmx && lsmx->v != mx->v) || (smx && lsmx && lsmx->v > smx->v)) smx = lsmx;
    }
    access(v); splay(lca);
    if (lca->r) {
        auto& [mx, smx] = ret;
        auto rmx = lca->r->mx, rsmx = lca->r->smx;
        if (rmx->v > mx->v) smx = mx, mx = rmx;
        if ((!smx && rmx->v != mx->v) || (smx && rmx->v > smx->v && rmx->v != mx->v)) smx = rmx;
        if ((!smx && rsmx && rsmx->v != mx->v) || (smx && rsmx && rsmx->v > smx->v)) smx = rsmx;
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

struct UnionFind {
    vector<int> p, sz;
    UnionFind(int n) {
        p.resize(n + 1); sz.resize(n + 1);
        iota(p.begin(), p.end(), 0); fill(sz.begin(), sz.end(), 1);
    }
    int find(int x) { return x == p[x] ? x : p[x] = find(p[x]); }
    void merge(int x, int y) {
        x = find(x); y = find(y);
        if (x == y) return;
        if (sz[x] < sz[y]) swap(x, y);
        p[y] = x; sz[x] += sz[y];
    }
};

using Edge = tuple<node*, int, int>;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int V, E; cin >> V >> E;
    vector<node*> vertex(V + 1);
    vector<Edge> edge(E);
    UnionFind uf(V);
    for (int i = 1; i <= V; ++i) vertex[i] = new node();
    for (int i = 0; i < E; ++i) {
        int u, v, w; cin >> u >> v >> w;
        auto e = new node(w);
        edge[i] = {e, u, v};
    }
    sort(edge.begin(), edge.end(), [](const Edge& a, const Edge& b) {
        return get<0>(a)->v < get<0>(b)->v;
    });

    int added = 0, mst = 0;
    for (int i = 0; i < E; ++i) { // 크루스칼
        const auto& [e, u, v] = edge[i];
        if (uf.find(u) == uf.find(v)) continue;
        uf.merge(u, v);
        link_edge(vertex[u], e, vertex[v]);
        mst += e->v;
        added++;

        if (uf.sz[uf.find(u)] == V) {
            ++i; break;
        }
    }
    // MST나 두번째로 작은 스패닝 트리가 없는 경우
    if (added != V - 1 || uf.sz[uf.find(1)] < V) {
        cout << -1;
        return 0;
    }

    int min_diff = 1e9;
    for (int i = 0; i < E; ++i) {
        const auto& [e, u, v] = edge[i];
        assert(connected(vertex[u], vertex[v]));

        auto [mx, smx] = query_max(vertex[u], vertex[v]);
        assert(mx->v != -1);
        assert(mx->v != smx->v);
        if (mx->v >= e->v) {
            if (!smx || smx->v >= e->v || smx->v == -1) continue;
            min_diff = min(min_diff, e->v - smx->v);
        } else {
            min_diff = min(min_diff, e->v - mx->v);
        }
    }

    if (min_diff == 1e9) cout << -1;
    else cout << mst + min_diff;

    return 0;
}