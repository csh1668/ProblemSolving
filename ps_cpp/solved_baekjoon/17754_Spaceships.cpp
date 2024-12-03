#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p;
    int cnt, v;
    node(int v) : l(0), r(0), p(0), cnt(1), v(v) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void update() { cnt = 1 + (l ? l->cnt : 0) + (r ? r->cnt : 0); }
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
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    }
}
void access(node* x) { splay(x); x->r = 0; x->update(); for (; x->p; splay(x)) splay(x->p), x->p->r = x, x->p->update(); }
void link(node* c, node* p) { access(c); access(p); c->l = p; p->p = c; c->update(); }
void cut(node* x) { access(x); if (!x->l) return; x->l = x->l->p = 0; x->update(); }
node* find_root(node* x) { access(x); while (x->l) x = x->l; splay(x); return x; }
node* find_lca(node* u, node* v) { access(u); access(v); splay(u); return u->p ? u->p : u; }
bool connected(node* u, node* v) { return find_root(u) == find_root(v); }
int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    vector<node*> nodes(N + 1);
    for (int i = 1; i <= N; ++i) nodes[i] = new node(i);

    while (Q--) {
        int op, u, v; cin >> op >> u;
        if (op == 1) {
            cin >> v;
            link(nodes[u], nodes[v]);
        } else if (op == 2) {
            cut(nodes[u]);
        } else {
            cin >> v;
            if (!connected(nodes[u], nodes[v])) cout << "-1\n";
            else cout << find_lca(nodes[u], nodes[v])->v << '\n';
        }
    }
    
    return 0;
}