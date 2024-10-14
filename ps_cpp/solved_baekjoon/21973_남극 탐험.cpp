#include <bits/stdc++.h>

using namespace std;

struct node {
    node *l, *r, *p;
    int cnt, v, sum;
    bool flip;
    node(int v = 0) : l(nullptr), r(nullptr), p(nullptr), cnt(1), v(v), sum(v), flip(false) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() {
        if (flip) {
            swap(l, r);
            if (l) l->flip ^= 1;
            if (r) r->flip ^= 1;
            flip = false;
        }
    }
    void update() {
        cnt = 1; sum = v;
        if (l) {
            l->push();
            cnt += l->cnt;
            sum += l->sum;
        }
        if (r) {
            r->push();
            cnt += r->cnt;
            sum += r->sum;
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
    for(; !x->is_root(); x->rotate()) {
        if (!x->p->is_root()) x->p->p->push();
        x->p->push(); x->push();
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
        splay(x->p); x->p->r = x; x->p->update();
    }
}
// c의 부모를 p로 만든다
void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c;
    c->update();
}
// x와 x의 부모를 끊는다
void cut(node* x) {
    access(x); x->l->p = nullptr; x->l = nullptr;
    x->update();
}

node* find_root(node* x) {
    access(x); // x의 왼쪽 서브트리에 x의 조상들이 쭉 있다
    while (x->l) x = x->l;
    splay(x);
    return x;
}

node* find_parent(node* x) {
    access(x);
    if (!x->l) return nullptr;
    x = x->l; while (x->r) x = x->r; // x의 predeccessor
    splay(x);
    return x;
}

node* find_lca(node* u, node* v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
}

void make_root(node* x) {
    access(x); splay(x);
    x->flip ^= true;
}

int query(node* u, node* v) {
    node* lca = find_lca(u, v);
    int ret = lca->v;
    access(u); splay(lca);
    if (lca->r) ret += lca->r->sum;

    access(v); splay(lca);
    if (lca->r) ret += lca->r->sum;

    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N;
    vector<node*> nodes(N + 1);
    for (int i = 1; i <= N; ++i) {
        int v; cin >> v;
        nodes[i] = new node(v);
    }

    cin >> Q;
    while (Q--) {
        string op; int a, b; cin >> op >> a >> b;
        if (op[0] == 'b') { // bridge
            auto &u = nodes[a], &v = nodes[b];
            make_root(u);
            if (find_root(v) == u) {
                cout << "no\n";
            } else {
                cout << "yes\n";
                link(u, v);
            }
        } else if (op[0] == 'p') { // penguins
            auto &u = nodes[a];
            splay(u);
            u->v = b;
            u->update();
        } else if (op[0] == 'e') { // excursion
            auto &u = nodes[a], &v = nodes[b];
            make_root(u);
            if (find_root(v) == u) {
                cout << query(u, v) << '\n';
            } else {
                cout << "impossible\n";
            }
        }
        cout.flush();
    }

    return 0;
}