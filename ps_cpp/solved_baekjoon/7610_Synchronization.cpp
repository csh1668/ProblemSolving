#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p;
    int cnt, v; bool flip;
    node() : l(0), r(0), p(0), cnt(1), v(1) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { if (flip) { swap(l, r); if (l) l->flip ^= 1; if (r) r->flip ^= 1; flip = 0; } }
    void update() {
        cnt = 1;
        if (l) l->push(), cnt += l->cnt;
        if (r) r->push(), cnt += r->cnt;
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
    access(x); if (!x->l) return;
    x->l->p = 0; x->l = 0; x->update();
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

bool connected(node* u, node* v) {
    return find_root(u) == find_root(v);
}

void cut(node* u, node* v) {
    if (find_parent(u) == v) cut(u);
    else if (find_parent(v) == u) cut(v);
    else assert(0);
}

void make_root(node* x) {
    access(x); x->flip ^= 1; x->push();
}

struct edge { int u, v, sum; };

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M, Q; cin >> N >> M >> Q;
    vector<node*> vertex(N + 1);
    vector<edge> edges(N);
    for (int i = 1; i <= N; ++i) vertex[i] = new node();
    for (int i = 1; i <= N - 1; ++i) {
        int u, v; cin >> u >> v;
        edges[i] = {u, v, 0};
    }

    while (M--) {
        int i; cin >> i;
        auto& e = edges[i];
        auto &u = vertex[e.u], &v = vertex[e.v];

        if (connected(u, v)) {
            auto root = find_root(u);
            e.sum = root->v;
            cut(u, v);
            auto u_root = find_root(u), v_root = find_root(v);
            u_root->v = v_root->v = e.sum;
        } else {
            auto u_root = find_root(u), v_root = find_root(v);
            int sum = u_root->v + v_root->v - e.sum;
            make_root(u); link(u, v);
            auto root = find_root(u);
            root->v = sum;
        }
    }

    while (Q--) {
        int i; cin >> i;
        auto& v = vertex[i];
        auto root = find_root(v);
        cout << root->v << '\n';
    }

    return 0;
}