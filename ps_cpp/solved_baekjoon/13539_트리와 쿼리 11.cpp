#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p;
    int cnt;
    int v;
    node(int v = 0) : l(nullptr), r(nullptr), p(nullptr), cnt(1), v(v) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() {
        // push
    }
    void update() {
        cnt = 1;
        if (l) {
            l->push();
            cnt += l->cnt;
        }
        if (r) {
            r->push();
            cnt += r->cnt;
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
// x를 루트로 만든다
void splay(node* x) {
    for (; !x->is_root(); x->rotate()) {
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    }
    x->push();
}
// x에서 루트까지 올라가는 경로를 Chain으로 묶고, x를 루트로 만든다.
void access(node* x) {
    splay(x); x->r = nullptr; x->update();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x;
    }
}
// c의 부모를 p로 만든다
void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c;
}
// x와 x의 부모를 끊는다
void cut(node* x) {
    access(x); x->l->p = nullptr; x->l = nullptr;
}

node* find_root(node* x) {
    access(x);
    while (x->l) x = x->l;
    splay(x);
    return x;
}

node* find_parent(node* x) {
    access(x);
    if (!x->l) return nullptr;
    x = x->l; while (x->r) x = x->r;
    splay(x);
    return x;
}

int depth(node* x) {
    access(x);
    return x->cnt - 1;
}

node* find_lca(node* u, node* v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
}

vector<int> adj[100005];
node* nodes[100005];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    for (int i = 1; i <= N; ++i) nodes[i] = new node(i);

    while (Q--) {
        int op; cin >> op;
        if (op == 1) {
            int u, v; cin >> u >> v;
            link(nodes[u], nodes[v]);
        } else if (op == 2) {
            int u; cin >> u;
            cut(nodes[u]);
        } else if (op == 3) {
            int u, v; cin >> u >> v;
            cout << find_lca(nodes[u], nodes[v])->v << '\n';
        }
    }

    return 0;
}