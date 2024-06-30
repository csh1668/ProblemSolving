#include <bits/stdc++.h>

using namespace std;

typedef long long int ll;

struct node
{
    node *l, *r, *p;
    int cnt;
    ll sum, lazy, v;
    // ll sum, v, mn, mx;
    // bool flip;

    node(ll v, node* p) : l(nullptr), r(nullptr), p(p), cnt(1), sum(v), v(v) {}
    node(ll v) : node(v, nullptr) {}
    node() : node(0) {}

    ~node() {
        if (l) delete l;
        if (r) delete r;
    }
} *tree, *nodes[1000010];

ll arr[1000010];

void update(node* x){
    x->cnt = 1;
    x->sum = x->v;
    if (x->l) {
        x->cnt += x->l->cnt;
        x->sum += x->l->sum;
    }
    if (x->r) {
        x->cnt += x->r->cnt;
        x->sum += x->r->sum;
    }
}

void propagate(node* x){
    if (x->lazy == 0) return;
    x->v += x->lazy;
    if (x->l){
        x->l->lazy += x->lazy;
        x->l->sum += x->l->cnt * x->lazy;
    }
    if (x->r){
        x->r->lazy += x->lazy;
        x->r->sum += x->r->cnt * x->lazy;
    }
    x->lazy = 0;
}

void rotate(node* x){
    node* p = x->p;
    node* b = nullptr;
    if (!p) return; // x가 루트라면 종료
    if (x == p->l) { // x가 왼쪽 자식
        p->l = b = x->r;
        x->r = p;
    } else { // x가 오른쪽 자식
        p->r = b = x->l;
        x->l = p;
    } // x가 p의 부모가 됨
    x->p = p->p;
    p->p = x;
    if (b) b->p = p;
    (x->p ? (p==x->p->l ? x->p->l : x->p->r) : tree) = x;
    update(p); update(x);
}

void splay(node* x){
    while (x->p){ // x가 루트가 될 때까지
        node* p = x->p;
        node* g = p->p; // x의 조상의 조상
        if (g){
            // g의 왼쪽 왼쪽이 or 오른쪽 오른쪽이 x일 경우
            if ((x==p->l) == (p==g->l)) rotate(p);
            else rotate(x);
        }
        rotate(x);
    }
}

bool find(ll k){
    node* p = tree;
    if (!p) return false;
    while (p){
        if (k == p->cnt) break;
        if (k < p->cnt){
            if (!p->l) break;
            p = p->l;
        } else {
            if (!p->r) break;
            p = p->r;
        }
    }
    splay(p);
    return k == p->cnt;
}

void findKth(int k){
    node* x = tree;
    propagate(x);
    while (true){
        while (x->l && x->l->cnt > k){
            x = x->l; propagate(x);
        }
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
        propagate(x);
    }
    splay(x);
}

void insert(int k, uint v){
    findKth(k);
    node* p = tree->r;
    while (p->l) p = p->l; // p는 k번째 요소의 전위후속자
    p->l = new node(v);
    p->l->p = p; splay(p->l);
}

void remove(int k){
    findKth(k);
    node* p = tree;
    if (p->l && p->r){
        tree = p->l; tree->p = nullptr;
        node* x = tree;
        while (x->r) x = x->r;
        x->r = p->r; p->r->p = x;
        p->l = p->r = nullptr;
        delete p; 
        return;
    }
    if (p->l){
        tree = p->l; tree->p = nullptr;
        p->l = nullptr;
        delete p; 
        return;
    }
    if (p->r){
        tree = p->r; tree->p = nullptr;
        p->r = nullptr;
        delete p; 
        return;
    }
    // delete p; 
    tree = nullptr;
}

void initTree(int n){
    memset(nodes, 0, sizeof(nodes));

    tree = new node(numeric_limits<ll>::min()); // left dummy
    auto cur = tree;
    for (int i = 1; i <= n; i++){
        nodes[i] = cur->r = new node(arr[i], cur);
        cur = cur->r;
    }
    cur->r = new node(numeric_limits<ll>::max(), cur); // right dummy
    for (int i = n; i >= 1; i--) update(nodes[i]);
    if (n == 1) return;
    splay(nodes[n >> 1]);
}

void interval(int s, int e){
    findKth(s - 1);
    node* x = tree;
    tree = x->r;
    tree->p = nullptr; // 일시적으로 연결을 끊고
    findKth(e - s + 1);
    x->r = tree;
    tree->p = x; // 다시 연결
    tree = x;
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, m, k; cin >> n >> m >> k; m += k;
    for (int i = 1; i <= n; i++) cin >> arr[i];
    initTree(n);
    for (int i = 0; i < m; i++){
        int mode; cin >> mode;
        if (mode == 1){
            ll l, r, x; cin >> l >> r >> x;
            interval(l, r);
            auto cur = tree->r->l;
            cur->sum += cur->cnt * x;
            cur->lazy += x;
            update(tree);
        } else {
            ll l, r; cin >> l >> r;
            interval(l, r);
            cout << tree->r->l->sum << '\n';
        }
    }
}