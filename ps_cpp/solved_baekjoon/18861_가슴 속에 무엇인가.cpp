#include <bits/stdc++.h>

using namespace std;
constexpr int INF = 1'000'000'010;

struct node {
    node *l, *r, *p;
    int cnt, v;
    node* mn;
    bool flip;
    node(int v = INF) : l(nullptr), r(nullptr), p(nullptr), cnt(1), v(v), mn(this), flip(false) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), flip = false, l && (l->flip ^= true), r && (r->flip ^= true)); }
    void update() {
        cnt = 1; mn = this;
        if (l) {
            l->push();
            cnt += l->cnt;
            if (l->mn->v < mn->v) mn = min(l->mn, mn);
        }
        if (r) {
            r->push();
            cnt += r->cnt;
            if (r->mn->v < mn->v) mn = min(r->mn, mn);
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
    friend node* min(node* u, node* v) {
        return u->v < v->v ? u : v;
    }
};
// x를 루트로 만든다
void splay(node* x) {
    for (; !x->is_root(); x->rotate()) {
        if (!x->p->is_root()) x->p->p->push();
        x->p->push(); x->push();
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    }
    x->push();
}
// x에서 루트의 경로를 chain으로 묶고, x를 루트로 만든다
void access(node* x) {
    splay(x); x->r = nullptr; x->update();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x; x->p->update();
    }
}
// c의 부모를 p로 만든다
void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c; c->update();
}
// x와 x의 부모를 끊는다
void cut(node* x) {
    access(x); 
    if (!x->l) return;
    x->l->p = nullptr; x->l = nullptr; x->update();
}
// x가 속한 chain의 root를 찾는다
node* find_root(node* x) {
    access(x);
    while (x->l) x = x->l;
    splay(x);
    return x;
}
// u와 v의 lca를 찾는다
node* find_lca(node* u, node* v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
}
// x를 루트로 만든다
void make_root(node* x) {
    access(x); splay(x);
    x->flip ^= true;
}
// u와 v 사의 경로의 최솟값을 가진 노드(간선)을 찾는다
node* query_min(node* u, node* v) {
    node* lca = find_lca(u, v);
    node* ret = lca;
    access(u); splay(lca);
    if (lca->r) ret = min(ret, lca->r->mn);
    access(v); splay(lca);
    if (lca->r) ret = min(ret, lca->r->mn);
    return ret;
}

bool connected(node* u, node* v) {
    make_root(u);
    return find_root(v) == u;
}

void cut_edge(node* u, node* e, node* v) {
    make_root(u);
    cut(e);
    make_root(e);
    cut(v);
}

void link_edge(node* u, node* e, node* v) {
    make_root(u);
    link(u, e);
    make_root(v);
    link(v, e);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    vector<node*> pool(N + 1);
    set<pair<int, node*>> edges;
    unordered_map<node*, pair<int, int>> edge_to_uv;

    for (int i = 1; i <= N; ++i) pool[i] = new node();

    while (Q--) {
        int op; cin >> op;
        if (op == 1) {
            int u, v, w; cin >> u >> v >> w;
            node* min_edge;
            if (connected(pool[u], pool[v])) {
                if ((min_edge = query_min(pool[u], pool[v]))->v >= w) continue;
                auto [x, y] = edge_to_uv[min_edge];
                cut_edge(pool[x], min_edge, pool[y]);
                edges.erase({min_edge->v, min_edge});
            }

            node* edge = new node(w);
            // pool.push_back(edge);
            edges.insert({w, edge});
            
            link_edge(pool[u], edge, pool[v]);

            edge_to_uv[edge] = {u, v};
        } else if (op == 2) {
            int d; cin >> d;
            while (!edges.empty() && edges.begin()->first < d) {
                auto [w, edge] = *edges.begin();
                auto [u, v] = edge_to_uv[edge];
                cut_edge(pool[u], edge, pool[v]);
                edges.erase(edges.begin());
            }
        } else {
            int u, v; cin >> u >> v;
            if (connected(pool[u], pool[v])) {
                node* min_edge = query_min(pool[u], pool[v]);
                assert(min_edge->v < INF);
                cout << min_edge->v << '\n';
            } else {
                cout << "0\n";
            }
        }
    }

    return 0;
}