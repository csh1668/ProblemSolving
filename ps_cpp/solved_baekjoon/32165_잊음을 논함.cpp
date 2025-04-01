#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;

template<typename T> using hashset = gp_hash_table<T, null_type, hash<T>, equal_to<T>, direct_mask_range_hashing<int>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;

struct node {
    node *l, *r, *p;
    int cnt, v; bool flip;
    node(int v = 0) : l(0), r(0), p(0), cnt(1), v(v) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { if (flip) { swap(l, r); if (l) l->flip ^= 1; if (r) r->flip ^= 1; flip = 0; } }
    void pull() { cnt = 1; if (l) l->push(), cnt += l->cnt; if (r) r->push(), cnt += r->cnt; }
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
        t->pull(); pull();
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
    splay(x); x->r = 0; x->pull();
    for (; x->p; splay(x)) {
        splay(x->p); x->p->r = x; x->p->pull();
    }
}

void link(node* c, node* p) {
    access(c); access(p);
    c->l = p; p->p = c; c->pull();
}

void cut(node* x) {
    access(x); if (!x->l) return;
    x->l->p = 0; x->l = 0; x->pull();
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

void cut(node* u, node* v) {
    if (find_parent(u) == v) cut(u);
    else if (find_parent(v) == u) cut(v);
    else assert(0);
}

struct vertex {
    node *a, *b;
    int time;
    vertex(node *a = 0, node *b = 0, int time = 0) : a(a), b(b), time(time) {}
    // bool operator<(const vertex& rhs) const { return time < rhs.time; }
    bool operator==(const vertex& rhs) const { return time == rhs.time; }
    int operator()() const { return time; }
};

const int RAND = chrono::high_resolution_clock::now().time_since_epoch().count();

inline size_t hashing(size_t x) {
    x += 0x9e3779b97f4a7c15 + RAND;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

namespace std {
    template <> struct hash<vertex> {
        inline std::size_t operator()(const vertex& p) const {
            return hashing(p.time);
        }
    };
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;

    // set<vertex> S;
    hashset<vertex> S; S.set_loads({0.25, 0.5}); S.resize(Q);
    vector<node*> last(N + 1);
    for (int i = 1; i <= N; ++i) {
        last[i] = new node(i);
    }

    auto test = [&]() {
        for (int i = 1; i <= N; ++i) {
            cout << "a_" << i << ": ";
            access(last[i]);
            cout << find_root(last[i])->v << '\n';
        }
    };

    for (int q = 1; q <= Q; ++q) {
        char op; cin >> op;
        if (op == 's') {
            int i, j; cin >> i >> j;
            node *u = new node(i), *v = new node(j);
            link(u, last[j]); link(v, last[i]);
            last[i] = u; last[j] = v;
            S.insert(vertex(u, v, q));
        } else if (op == 'f' || op == 'r') {
            int i; cin >> i;
            auto& cur = *S.find(vertex(0, 0, i));
            assert(cur.a && cur.b);

            node *u = cur.a, *v = cur.b;
            node *pu = find_parent(u), *pv = find_parent(v);
            // cut(u, pu), cut(v, pv);
            cut(u), cut(v);

            link(u, pv); link(v, pu);
        } else { // op == 'q'
            int i; cin >> i;
            auto& cur = last[i];
            access(cur);
            cout << find_root(cur)->v << '\n';
            // cout << find_root(cur)->v << endl;
        }

        // cout << "Q" << q << ":\n";
        // test();
    }

    return 0;
}