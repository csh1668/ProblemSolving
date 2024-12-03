#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int INF = 1e9;

struct node {
    node *l, *r, *p;
    int cnt, v, set_lazy, ncnt;
    int lmx, rmx, mx, sum;
    bool flip, dummy;
    node(int v = 0, node* p = 0) : l(0), r(0), p(p), cnt(1), v(v), set_lazy(INF), ncnt(1), lmx(v), rmx(v), mx(v), sum(v), flip(0), dummy(0) {}
    ~node() {}
} *tree;

void push(node* x) {
    if (x->flip) {
        swap(x->l, x->r);
        swap(x->lmx, x->rmx);
        if (x->l) x->l->flip ^= 1;
        if (x->r) x->r->flip ^= 1;
        x->flip = 0;
    }
    if (x->set_lazy != INF) {
        x->v = x->set_lazy;
        x->sum = x->set_lazy * x->ncnt;
        x->lmx = x->rmx = x->mx = max(x->v, x->sum);
        if (x->l) x->l->set_lazy = x->set_lazy;
        if (x->r) x->r->set_lazy = x->set_lazy;
        x->set_lazy = INF;
    }
}

void pull(node* x) {
    x->cnt = 1; x->ncnt = !(x->dummy);
    x->sum = x->v; x->lmx = x->rmx = x->mx = x->v;
    auto l = x->l, r = x->r;

    if (l) push(l), x->cnt += l->cnt, x->sum += l->sum, x->ncnt += l->ncnt;
    if (r) push(r), x->cnt += r->cnt, x->sum += r->sum, x->ncnt += r->ncnt;

    if (l && r) {
        x->lmx = max({l->lmx, l->sum + x->v, l->sum + x->v + r->lmx});
        x->rmx = max({r->rmx, r->sum + x->v, r->sum + x->v + l->rmx});
        x->mx = max({l->mx, r->mx, l->rmx + x->v + r->lmx, l->rmx + x->v, r->lmx + x->v, x->mx});
    }
    else if (l) {
        x->lmx = max({l->lmx, l->sum + x->v});
        x->rmx = max({l->rmx + x->v, x->v});
        x->mx = max({l->mx, x->mx, l->rmx + x->v});
    }
    else if (r) {
        x->lmx = max({r->lmx + x->v, x->v});
        x->rmx = max({r->rmx, r->sum + x->v});
        x->mx = max({r->mx, x->mx, r->lmx + x->v});
    }

    assert(x->mx == max({x->lmx, x->rmx, x->sum, x->mx}));
}

void rotate(node* x){
    node* p = x->p, *b = nullptr;
    if (!p) return;
    push(p); push(x);
    if (x == p->l){
        p->l = b = x->r;
        x->r = p;
    } else {
        p->r = b = x->l;
        x->l = p;
    }
    x->p = p->p; p->p = x;
    if (b) b->p = p;
    (x->p ? (p == x->p->l ? x->p->l : x->p->r) : tree) = x;
    pull(p); pull(x);
}

void splay(node* x){
    while (x->p){
        node *p = x->p, *g = p->p;
        if (g){
            if ((x==p->l) == (p==g->l)) rotate(p);
            else rotate(x);
        }
        rotate(x);
    }
}

void findKth(int k){
    node* x = tree;
    push(x);
    while (true){
        while (x->l && x->l->cnt > k) x = x->l, push(x);
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
        push(x);
    }
    splay(x);
}

void insertKth(int k, int v) { 
    findKth(k);
    node* x = tree->r; push(x);
    while (x->l) x = x->l, push(x);
    x->l = new node(v, x);
    pull(x);
    splay(x->l);
}

void eraseKth(int k) {
    findKth(k);
    node* p = tree; push(p);
    tree = p->l; tree->p = 0;
    node* x = tree; push(x);
    while (x->r) x = x->r, push(x);
    x->r = p->r; p->r->p = x;
    pull(x->r); pull(x); pull(tree); delete p;
    splay(x);
}

node* interval(int s, int e) {
    findKth(s - 1);
    auto x = tree;
    tree = x->r;
    tree->p = 0;
    findKth(e - s + 1);
    x->r = tree; tree->p = x;
    tree = x;
    if (tree->r && tree->r->l) {
        push(x->r->l);
        return x->r->l;
    } else {
        return 0;
    }
}

void prefix(node* x) {
    if (!x) return;
    push(x);
    prefix(x->l);
    cout << x->v << ' ';
    prefix(x->r);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);


    int N, M; cin >> N >> M;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) cin >> A[i];

    vector<node*> nodes(N + 2);
    nodes[0] = tree = new node(0); tree->dummy = 1, tree->ncnt = 0;
    auto cur = tree;
    
    for (int i = 1; i <= N; ++i) nodes[i] = cur->r = new node(A[i - 1], cur), cur = cur->r;
    nodes[N + 1] = cur->r = new node(0, cur); cur->r->dummy = 1, cur->r->ncnt = 0;
    for (int i = N; i >= 0; --i) pull(nodes[i]);
    splay(nodes[N / 2]);


    while (M--) {
        string cmd; cin >> cmd;
        if (cmd[0] == 'I') { // INSERT
            int posi, tot; cin >> posi >> tot;
            for (int i = 0; i < tot; ++i) {
                int x; cin >> x;
                insertKth(posi + i, x);
            }
        } else if (cmd[0] == 'D') { // DELETE
            int posi, tot; cin >> posi >> tot;
            for (int i = 0; i < tot; ++i) {
                eraseKth(posi);
            }
        } else if (cmd[0] == 'M' && cmd[2] == 'K') { // MAKE-SAME
            int posi, tot; cin >> posi >> tot;
            int c; cin >> c;
            if (!tot) continue;
            auto x = interval(posi, posi + tot - 1);
            if (!x) continue;
            push(x);
            x->set_lazy = c;
            push(x); pull(x);
        } else if (cmd[0] == 'R') { // REVERSE
            int posi, tot; cin >> posi >> tot;
            if (!tot) continue;
            auto x = interval(posi, posi + tot - 1);
            if (!x) continue;
            push(x);
            x->flip ^= 1;
            push(x); pull(x);
        } else if (cmd[0] == 'G') { // GET-SUM
            int posi, tot; cin >> posi >> tot;
            auto x = interval(posi, posi + tot - 1);
            cout << (x ? x->sum : 0) << '\n';
        } else if (cmd[0] == 'M' && cmd[2] == 'X') { // MAX-SUM
            auto x = interval(1, tree->cnt - 2);
            if (!x) cout << 0 << '\n';
            else cout << x->mx << '\n';
            // cout << x->mx << '\n';
            assert(x->mx == max({x->lmx, x->rmx, x->sum, x->mx}));
        }
    }

    // prefix(tree); cout << '\n';

    return 0;
}