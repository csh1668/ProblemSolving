#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct DynamicTree { // Link/Cut Tree
    struct Node {
        Node *l, *r, *p;
        int cnt, idx; // idx for debug
        bool flip;
        Node(int idx = -1) : l(0), r(0), p(0), cnt(1), idx(idx), flip(0) {}
        bool is_root() { return !p || (p->l != this && p->r != this); }
        bool is_left() { return p && p->l == this; }
        void push() { flip && (swap(l, r), flip = 0, l && (l->flip ^= 1), r && (r->flip ^= 1)); }
        void update() { cnt = 1; l && (cnt += l->cnt); r && (cnt += r->cnt); }
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

    void splay(Node *x)  {
        for (; !x->is_root(); x->rotate()) {
            if (!x->p->is_root()) x->p->p->push();
            x->p->push(); x->push();
            if (x->p->is_root()) continue;
            if (x->is_left() == x->p->is_left()) x->p->rotate();
            else x->rotate();
        }
        x->push();
    }
    void access(Node *x) {
        splay(x); x->r = 0; x->update();
        for (; x->p; splay(x)) {
            splay(x->p); x->p->r = x; x->p->update();
        }
    }
    // c의 부모를 p로 만든다
    void link(Node *c, Node *p) {
        access(c); access(p);
        assert(c->l == 0);
        c->l = p; p->p = c; c->update();
    }
    // x와 x의 부모를 끊는다
    void cut(Node *x) {
        access(x);
        if (!x->l) return;
        x->l->p = 0; x->l = 0; x->update();
    }
    Node *find_root(Node *x) {
        access(x);
        while (x->l) x = x->l;
        splay(x);
        return x;
    }
    Node *lca(Node *x, Node *y) {
        access(x); access(y); splay(x);
        return x->p ? x->p : x;
    }
    void make_root(Node *x) {
        access(x); splay(x);
        x->flip ^= 1;
    }
    bool connected(Node *x, Node *y) {
        return find_root(x) == find_root(y);
    }

    // interfaces
    int N;
    vector<Node*> vertex;
    DynamicTree(int N) : N(N) {
        vertex.resize(N);
        for (int i = 0; i < N; ++i) {
            vertex[i] = new Node(i);
        }
    }
    ~DynamicTree() {
        for (int i = 0; i < N; ++i) {
            delete vertex[i];
        }
    }
    bool connected(int u, int v) {
        return connected(vertex[u], vertex[v]);
    }
    void link(int u, int v) {
        assert(!connected(u, v));
        make_root(vertex[u]);
        link(vertex[u], vertex[v]);
        assert(connected(u, v));
    }
    void cut(int u, int v) {
        assert(connected(u, v));
        make_root(vertex[u]);
        cut(vertex[v]);
        assert(!connected(u, v));
    }
};

struct LazyPropSegTree {
    vector<ll> T, L; int N;
    LazyPropSegTree(int N) : N(N) {
        const int sz = 1 << (33 - __builtin_clz(N));
        T.resize(sz, 0); L.resize(sz, 0);
    }
    void push(int l, int r, int i) {
        if (L[i] == 0) return;
        T[i] += L[i] * (r - l + 1);
        if (l ^ r) {
            L[i << 1] += L[i];
            L[i << 1 | 1] += L[i];
        }
        L[i] = 0;
    }
    void update(int l, int r, int ql, int qr, ll v, int i = 1) {
        push(l, r, i);
        if (l > qr || r < ql) return;
        if (ql <= l && r <= qr) {
            L[i] += v;
            push(l, r, i);
            return;
        }
        int m = (l + r) >> 1;
        update(l, m, ql, qr, v, i << 1); update(m + 1, r, ql, qr, v, i << 1 | 1);
        T[i] = T[i << 1] + T[i << 1 | 1];
    }
    ll query(int l, int r, int ql, int qr, int i = 1) {
        push(l, r, i);
        if (l > qr || r < ql) return 0;
        if (ql <= l && r <= qr) return T[i];
        int m = (l + r) >> 1;
        return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
    }
    void update(int ql, int qr, ll v) {
        update(1, N, ql, qr, v);
    }
    ll query(int ql, int qr) {
        return query(1, N, ql, qr);
    }
};

struct Edge {
    int u, v;
    Edge(int u, int v) : u(min(u, v)), v(max(u, v)) {}
};

struct Query {
    int r, idx;
    Query(int r, int idx) : r(r), idx(idx) {}
};

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    DynamicTree dt(N + 1);
    
    vector<Edge> E; E.emplace_back(0, 0);
    for (int i = 1; i <= M; ++i) {
        int u, v; cin >> u >> v;
        E.emplace_back(u, v);
    }

    vector<int> rmx(M + 1, 0);
    int r = 1;
    for (int l = 1; l <= M; ++l) {
        while (r <= M && !dt.connected(E[r].u, E[r].v)) {
            dt.link(E[r].u, E[r].v);
            r++;
        }
        rmx[l] = r - 1;
        dt.cut(E[l].u, E[l].v);
    }

    vector<vector<Query>> sweep(M + 1);
    int Q; cin >> Q;
    vector<ll> ans(Q);

    for (int i = 0; i < Q; ++i) {
        int l, r; cin >> l >> r;
        sweep[l].emplace_back(r, i);
    }

    LazyPropSegTree ST(M + 1);
    for (int l = M; l >= 1; --l) {
        ST.update(l, rmx[l], 1);

        for (auto &[r, idx] : sweep[l]) {
            ans[idx] = ST.query(l, r);
        }
    }

    for (int i = 0; i < Q; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}