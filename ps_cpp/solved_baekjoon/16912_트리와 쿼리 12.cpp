#include <bits/stdc++.h>

using namespace std;

// 무지성 링크컷 트리

struct node {
    node *l, *r, *p;
    int cnt; bool flip;
    node() : l(0), r(0), p(0), cnt(1), flip(0) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), l && (l->flip ^= 1), r && (r->flip ^= 1), flip = 0); }
    void update() { cnt = 1; if (l) l->push(), cnt += l->cnt; if (r) r->push(), cnt += r->cnt; }
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
    for (; x->p; splay(x)) { splay(x->p); x->p->r = x; x->p->update(); }
}

void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c; c->update();
}

void cut(node* x) {
    access(x); if (!x->l) return;
    x->l = x->l->p = 0; x->update();
}

node* find_root(node* x) { access(x); while (x->l) x = x->l, x->push(); splay(x); return x; }
node* find_parent(node* x) { access(x); if (!x->l) return 0; x = x->l, x->push(); while (x->r) x = x->r, x->push(); splay(x); return x;}

void make_root(node* x) { access(x), splay(x), x->flip ^= 1; }

void link_node(node* u, node* v) { make_root(u); link(u, v); }
void cut_node(node* u, node* v) {
    assert(find_root(u) == find_root(v));
    if (find_parent(u) == v) swap(u, v);
    // u가 v의 부모
    cut(v);
}
bool connected(node* u, node* v) { return find_root(u) == find_root(v); }

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    vector<node*> vertex(N + 1);
    for (int i = 1; i <= N; ++i) vertex[i] = new node();
    while (M--) {
        int op, a, b; cin >> op >> a >> b;
        if (op == 1) {
            assert(!connected(vertex[a], vertex[b]));
            link_node(vertex[a], vertex[b]);
        } else if (op == 2) {
            assert(connected(vertex[a], vertex[b]));
            cut_node(vertex[a], vertex[b]);
        } else {
            cout << (connected(vertex[a], vertex[b]) ? "1\n" : "0\n");
        }
    }

    return 0;
}