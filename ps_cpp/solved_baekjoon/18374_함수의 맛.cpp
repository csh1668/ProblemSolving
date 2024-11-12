#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct node {
    node *l, *r, *p;
    int cnt, func, v; ll sum;
#ifndef ONLINE_JUDGE
    int idx;
#endif
    node(int v, int func) : l(0), r(0), p(0), cnt(1), func(func), v(v), sum(v) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void update() { cnt = 1; sum = v; l && (cnt += l->cnt, sum += l->sum); r && (cnt += r->cnt, sum += r->sum); }
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
        if (x->p->is_root()) continue;
        if (x->is_left() == x->p->is_left()) x->p->rotate();
        else x->rotate();
    }
}
void access(node* x) {
    splay(x); x->r = 0; x->update();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x; x->p->update();
    }
}
void link(node* c, node* p) { access(c); access(p); c->l = p; p->p = c; c->update(); }
void cut(node* x) { access(x); if (!x->l) return; x->l->p = 0; x->l = 0; x->update(); }
node* find_root(node* x) { access(x); while(x->l) x = x->l; splay(x); return x; }
node* find_lca(node* u, node* v) { access(u); access(v); splay(u); return u->p ? u->p : u; }
bool connected(node* u, node* v) { return find_root(u) == find_root(v); }

#ifndef ONLINE_JUDGE
// For debugging
void inspect(node* x) {
    cout << "idx: " << x->idx << '\t';
    cout << "a_i: " << x->v << '\t';
    cout << "sum: " << x->sum << '\t';
    cout << "fnc: " << x->func << '\t';
    cout << "par: " << (x->p ? x->p->idx : -1) << '\t';
    cout << "lft: " << (x->l ? x->l->idx : -1) << '\t';
    cout << "rgt: " << (x->r ? x->r->idx : -1) << '\t';
    cout << "root: " << find_root(x)->idx << '\n';
}
#endif

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    vector<node*> pool(N + 1);
    for (int i = 1; i <= N; ++i) {
        int func; cin >> func;
        pool[i] = new node(0, func);
#ifndef ONLINE_JUDGE
        pool[i]->idx = i;
#endif
    }
    for (int i = 1; i <= N; ++i) {
        int v; cin >> v;
        pool[i]->v = pool[i]->sum = v;
    }

    vector<int> start(N + 1, 0);
    auto dfs = [&](int u, int st, auto&& dfs) -> void {
        int v = pool[u]->func;
        start[u] = st;
        if (start[v]) {
            if (start[v] != st) {
                assert(!connected(pool[u], pool[v]));
                link(pool[u], pool[v]);
            }
        } else {
            dfs(v, st, dfs);
            assert(!connected(pool[u], pool[v]));
            link(pool[u], pool[v]);
        }
    };
    for (int i = 1; i <= N; ++i) if (!start[i]) dfs(i, i, dfs);

#ifndef ONLINE_JUDGE
    for (int i = 1; i <= N; ++i) {
        access(pool[i]);
        cout << "access "; inspect(pool[i]);
    }
#endif

    while (Q--) {
        int op; cin >> op;
        if (op == 1) {
            int i, j; cin >> i >> j;
            if (pool[i]->func == j) continue;
            /*
            - cur가 chain의 루트고 lca(cur, bottom)가 cur일 때: 값 변경
            - cur가 chain의 루트고 lca(cur, bottom)가 cur가 아닐 때?: link(cur, target), 값 변경
            - cur가 chain의 루트가 아니고 lca(cur, bottom)가 cur일 때: cut(cur), link(top, bottom), 값 변경
            - cur가 chain의 루트가 아니고 lca(cur, bottom)가 cur가 아닐 때: cut(cur), link(cur, target), 값 변경
            */
            auto cur = pool[i], target = pool[j], top = find_root(cur), bottom = pool[top->func], lca = find_lca(cur, bottom);

            if (cur == top) {
                top = find_root(target);
                if (cur == top) {
                    // do nothing
                } else {
                    link(cur, target);
                }
            } else {
                cut(cur);
                if (lca == cur) {
                    link(top, bottom);
                }
                top = find_root(target);
                if (cur == top) {
                    // do nothing
                } else {
                    link(cur, target);
                }
            }

            cur->func = j;

        } else if (op == 2) {
            int i, x; cin >> i >> x;
            access(pool[i]);
            pool[i]->v = x; pool[i]->update();
        } else {
            int x; cin >> x;
            ll ans = 0;
            // functional grpah에는 반드시 cycle이 존재한다.
            // bottom <-> cur의 LCA를 구한다. bottom <-> cur 경로의 합을 구하고, LCA <-> top의 합을 구하면 된다. LCA를 중복으로 더하지 않도록 주의한다.
            auto cur = pool[x], top = find_root(cur), bottom = pool[top->func], lca = find_lca(cur, bottom);
            access(cur); splay(lca);
            if (lca->r) ans += lca->r->sum;
            access(bottom); splay(lca);
            if (lca->r) ans += lca->r->sum;
            access(lca); ans += lca->sum;

#ifdef ONLINE_JUDGE
            cout << ans << '\n';
#else
            cout << "ans: " << ans << endl;
#endif
        }
    }

    return 0;
}