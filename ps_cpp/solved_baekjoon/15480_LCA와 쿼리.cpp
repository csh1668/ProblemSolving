#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p;
    int cnt, v; bool flip;
    node(int v = 0) : l(nullptr), r(nullptr), p(nullptr), cnt(1), v(v), flip(false) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), flip = false, l && (l->flip ^= true), r && (r->flip ^= true)); }
    void update() { cnt = 1; l && (l->push(), cnt += l->cnt); r && (r->push(), cnt += r->cnt); }
    void rotate() { 
        if (is_left()) {
            if (r) r->p = p; p->l = r; r = p;
        } else {
            if (l) l->p = p; p->r = l; l = p;
        }
        if (!p->is_root()) (p->is_left() ? p->p->l : p->p->r) = this;
        auto t = p; p = t->p; t->p = this; t->update(); update();
    }
};

void splay(node* x) {
    for (; !x->is_root(); x->rotate()) {
        if (!x->p->is_root()) x->p->p->push(); x->p->push(); x->push();
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    } x->push();
}

void access(node* x) {
    splay(x); x->r = nullptr; x->update();
    for (; x->p; splay(x)) splay(x->p), x->p->r = x, x->p->update();
}

void link(node* c, node* p) { access(c), access(p); c->l = p; p->p = c; c->update(); }

node* find_lca(node* u, node* v) { access(u); access(v); splay(u); return u->p ? u->p : u; }

void make_root(node* x) { access(x); splay(x); x->flip ^= true; }

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;
    vector<vector<int>> adj(N + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        adj[u].push_back(v); adj[v].push_back(u);
    }

    vector<node*> pool(N + 1);
    for (int i = 1; i <= N; ++i) pool[i] = new node(i);
    auto dfs = [&](int u, int p, auto&& self) -> void {
        for (int v : adj[u]) if (v != p) {
            make_root(pool[u]); link(pool[u], pool[v]);
            self(v, u, self);
        }
    };
    dfs(1, 0, dfs);

    cin >> M;
    for (int i = 0; i < M; ++i) {
        int r, u, v; cin >> r >> u >> v;
        make_root(pool[r]);
        cout << (find_lca(pool[u], pool[v])->v) << '\n';
    }

    return 0;
}