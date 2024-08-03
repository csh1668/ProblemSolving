#include <bits/stdc++.h>

using namespace std;

struct node
{
    node *l, *r, *p;
    int cnt, v;
    node(int v, node* p) : v(v), p(p){
        l = r = nullptr;
    }
    node(int v) : node(v, nullptr) {}
    node() : node(0) {}
} *tree, *nodes[100010];

void update(node* x){
    x->cnt = 1;
    if (x->l) x->cnt += x->l->cnt;
    if (x->r) x->cnt += x->r->cnt; 
}

void rotate(node * x){
    node *p = x->p, *b = nullptr;
    if (!p) return;
    if (x == p->l) p->l = b = x->r, x->r = p;
    else p->r = b = x->l, x->l = p;
    x->p = p->p, p->p = x;
    if (b) b->p = p;
    (x->p ? (p == x->p->l ? x->p->l : x->p->r) : tree) = x;
    update(p); update(x);
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
    while (true){
        while (x->l && x->l->cnt > k) x = x->l;
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
    }
    splay(x);
}

void deleteKth(int k){
    findKth(k);
    node *p = tree;
    if (p->l && p->r){
        tree = p->l; tree->p = nullptr;
        node *x = tree;
        while (x->r) x = x->r;
        x->r = p->r; p->r->p = x;
        update(x->r); update(x); update(tree);
    }
    else if (p->l) tree = p->l, tree->p = nullptr;
    else if (p->r) tree = p->r, tree->p = nullptr;
}

void initJosephus(int n){
    nodes[0] = tree = new node(numeric_limits<int>::min());
    auto cur = tree;
    for (int i = 1; i <= n; i++)
        nodes[i] = cur->r = new node(i, cur), cur = cur->r;
    nodes[n + 1] = cur->r = new node(numeric_limits<int>::max(), cur);
    for (int i = n + 1; i >= 0; i--)
        update(nodes[i]);
    if (n == 1) return;
    splay(nodes[n >> 1]);
}

// for splay tree study
void prefix(node* cur, int level=0, string str="ROOT:"){
    if (cur == nullptr) return;
    for (int i = 0; i < level; i++) cout << "  ";
    cout << str << " (" << cur->v << " " << cur->cnt << ")\n";
    prefix(cur->l, level + 1, "L--");
    prefix(cur->r, level + 1, "R--"); 
}

void print(node* cur){
    if (cur == nullptr) return;
    print(cur->l);
    cout << " (" << cur->v << " " << cur->cnt << ")";
    print(cur->r);
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, k; cin >> n >> k;
    initJosephus(n);
    
    cout << "<";
    int pos = 0;
    for (int i = 0; i < n; i++){
        pos = (pos + k - 1) % (n - i);
        findKth(pos + 1);
        // print(tree); cout << endl;
        cout << tree->v;
        if (i < n - 1) cout << ", ";
        deleteKth(pos + 1);
    }
    cout << ">\n";
}