#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 100'005;

struct node {
    node *l, *r, *p;
    int cnt, v;
    node* mx;
    bool flip;
    node(int v = 0) : l(nullptr), r(nullptr), p(nullptr), cnt(1), v(v), mx(this), flip(false) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { flip && (swap(l, r), flip = false, l && (l->flip ^= true), r && (r->flip ^= true)); };
    void update() {
        cnt = 1; mx = this;
        if (l) {
            l->push();
            cnt += l->cnt;
            if (l->mx->v > mx->v) mx = max(l->mx, mx);
        }
        if (r) {
            r->push();
            cnt += r->cnt;
            if (r->mx->v > mx->v) mx = max(r->mx, mx);
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
    friend node* max(node* u, node* v) {
        return u->v > v->v ? u : v;
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
// u와 v 사이의 경로의 v 최댓값을 가진 노드(간선)를 찾는다
node* query_max(node* u, node* v) {
    node* lca = find_lca(u, v);
    node* ret = lca;
    access(u); splay(lca);
    if (lca->r) ret = max(ret, lca->r->mx);
    access(v); splay(lca);
    if (lca->r) ret = max(ret, lca->r->mx);
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int T; cin >> T;
    ll ans = 0;
    while (T--) {
        int n, m; cin >> n >> m;
        ll cur = 0, ans = 0;
        vector<pair<int, int>> adj[MAXN];
        vector<node*> pool(n);
        unordered_map<node*, pair<int, int>> edge_to_uv(n);
        for (int i = 0; i < n; ++i) pool[i] = new node(-1);
        for (int u = 1; u <= n - 1; ++u) {
            int v, c; cin >> v >> c;
            cur += c;
            adj[v].emplace_back(u, c);
            adj[u].emplace_back(v, c);
        }

        auto dfs = [&](int u, int p, auto&& dfs) -> void {
            for (auto [v, c] : adj[u]) {
                if (v == p) continue;
                node* edge = new node(c);
                pool.push_back(edge);
                
                make_root(pool[u]);
                link(pool[u], edge);
                
                make_root(pool[v]);
                link(pool[v], edge);

                edge_to_uv[edge] = {u, v};
                dfs(v, u, dfs);
            }
        };
        dfs(0, -1, dfs);

        while (m--) {
            int u, v, c; cin >> u >> v >> c;
            node* max_edge = query_max(pool[u], pool[v]);
            if (max_edge->v <= c) {
                ans ^= cur;
                continue;
            }
            auto [a, b] = edge_to_uv[max_edge];
            make_root(pool[a]);
            cut(max_edge);
            make_root(max_edge);
            cut(pool[b]);
            cur += c - max_edge->v;
            ans ^= cur;

            node* new_edge = new node(c);
            pool.push_back(new_edge);
            make_root(pool[u]);
            link(pool[u], new_edge);
            make_root(pool[v]);
            link(pool[v], new_edge);
            edge_to_uv[new_edge] = {u, v};
        }

        cout << ans << '\n';

        for (int i = n; i < pool.size(); ++i) delete pool[i];
    }

    return 0;
}