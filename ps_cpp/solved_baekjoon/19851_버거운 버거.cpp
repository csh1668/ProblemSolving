#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 1'000'005;

struct node {
    node *l, *r, *p;
    int cnt, lv, rv, wlv, wrv;
    // '(' : false, ')' : true
    bool v, swch;
    node(bool v = false, node* p = 0) : l(0), r(0), p(p), cnt(1), v(v) {
        if (!v) lv = 1, wrv = 1;
        else rv = 1, wlv = 1;
    }
    int eval() { return (cnt - lv - rv) >> 1; }
    void combineLeft(node* a) {
        int p = min(a->lv, rv), wp = min(a->wrv, wlv);
        lv += a->lv - p;
        rv += a->rv - p;
        wlv += a->wlv - wp;
        wrv += a->wrv - wp;
    }
    void combineRight(node* b) {
        int p = min(lv, b->rv), wp = min(wrv, b->wlv);
        lv += b->lv - p;
        rv += b->rv - p;
        wlv += b->wlv - wp;
        wrv += b->wrv - wp;
    }
} *root, *nodes[MAX];

char s[MAX];

void push(node* x) {
    if (x->swch) {
        x->v ^= 1;
        if (x->l) x->l->swch ^= 1;
        if (x->r) x->r->swch ^= 1;
        swap(x->lv, x->wrv);
        swap(x->rv, x->wlv);
        x->swch = 0;
    }
}

void pull(node* x) {
    x->cnt = 1;
    auto &a = x->l, &b = x->r;
    if (!x->v) x->lv = x->wlv = 1, x->rv = x->wrv = 0;
    else x->lv = x->wlv = 0, x->rv = x->wrv = 1;

    if (a && b) {
        push(a), push(b);
        x->cnt += a->cnt + b->cnt;
        x->combineLeft(a);
        x->combineRight(b);
    } else if (a) {
        push(a);
        x->cnt += a->cnt;
        x->combineLeft(a);
    } else if (b) {
        push(b);
        x->cnt += b->cnt;
        x->combineRight(b);
    }
}

void rotate(node* x){
    node *p = x->p, *b = nullptr;
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
    (x->p ? (p == x->p->l ? x->p->l : x->p->r) : root) = x;
    pull(p); pull(x);
}

void splay(node* x){
    while (x->p){
        node *p = x->p, *g = p->p;
        if (g){
            if ((x==p->l) == (p==g->l)) rotate(p);
            else rotate(x);
        } rotate(x);
    }
}

void findKth(int k){
    node* x = root; push(x);
    while (true){
        while (x->l && x->l->cnt > k)
            x = x->l, push(x);
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
        push(x);
    } splay(x);
}

node* interval(int s, int e){
    findKth(s - 1);
    auto x = root;
    root = x->r;
    root->p = nullptr;
    findKth(e - s + 1);
    x->r = root;
    root->p = x;
    root = x;
    push(root->r->l);
    return root->r->l;
}

void swch(int l, int r) {
    auto x = interval(l, r);
    x->swch ^= 1;
}

void prefix(node* x) {
    if (!x) return;
    push(x);
    prefix(x->l);
    cout << (x->v ? ')' : '(');
    prefix(x->r);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N;
    cin >> (s + 1);
    
    root = nodes[0] = new node();
    auto cur = root;
    for (int i = 1; i <= N; ++i) cur->r = nodes[i] = new node(s[i] == ')', cur), cur = cur->r;
    nodes[N + 1] = cur->r = new node(0, cur);
    for (int i = N; i >= 1; --i) pull(nodes[i]);
    splay(nodes[N >> 1]);

    cin >> Q;
    while (Q--) {
        int op, l, r; cin >> op >> l >> r;
        if (op == 1) {
            swch(l, r);
        } else {
            auto cur = interval(l, r);
            int paired = cur->eval();
            cout << (cur->cnt - (paired << 1)) + cur->cnt << '\n';
        }

        // prefix(interval(1, N)); cout << '\n';
    }

    return 0;
}