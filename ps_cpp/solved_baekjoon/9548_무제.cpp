#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct node
{
    node *l, *r, *p;
    int cnt; char v; bool dummy;
    node(char v, node* p) : v(v), p(p){
        l = r = nullptr; cnt = 1;
    }
    node(char v) : node(v, nullptr) {}
    node() : node(0) {}
} *tree, *nodes[1000010];

char str[1000010];

inline void update(node* x){
    x->cnt = 1;
    if (x->l) x->cnt += x->l->cnt;
    if (x->r) x->cnt += x->r->cnt;
}

inline void rotate(node* x){
    node *p = x->p, *b = nullptr;
    if (!p) return;
    if (x == p->l) p->l = b = x->r, x->r = p;
    else p->r = b = x->l, x->l = p;
    x->p = p->p, p->p = x;
    if (b) b->p = p;
    (x->p ? (p == x->p->l ? x->p->l : x->p->r) : tree) = x;
    update(p); update(x);
}

inline void splay(node* x){
    while(x->p){
        node *p = x->p, *g = p->p;
        if (g){
            if ((x == p->l) == (p == g->l)) rotate(p);
            else rotate(x);
        }
        rotate(x);
    }
}

inline void findKth(int k){
    node* x = tree;
    while (true){
        while (x->l && x->l->cnt > k) x = x->l;
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
    }
    splay(x);
}

inline void interval(int s, int e){
    findKth(s - 1);
    node* x = tree;
    tree = x->r;
    tree->p = nullptr;
    findKth(e - s + 1);
    x->r = tree;
    tree->p = x;
    tree = x;
}

inline void insert(int idx, const char& v){
    findKth(idx);
    node* p = tree->r;
    while (p->l) p = p->l;
    p->l = new node(v, p);
    splay(p->l);
}

void printTree(node* cur){
    if (cur == nullptr) return;
    printTree(cur->l);
    if (!cur->dummy) cout << cur->v;
    printTree(cur->r);
}

inline void initTree(int n){
    memset(nodes, 0, sizeof(nodes));
    nodes[0] = tree = new node(numeric_limits<char>::min()); tree
    ->dummy = true;
    auto cur = tree;
    for (int i = 1; i <= n; ++i) nodes[i] = cur->r = new node(str[i - 1], cur), cur = cur->r;
    nodes[n + 1] = cur->r = new node(numeric_limits<char>::max(), cur); cur->r->dummy = true;
    for (int i = n + 1; i >= 0; --i) update(nodes[i]);
    if (n == 1) return;
    splay(nodes[n >> 1]);
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int t; cin >> t;
    for (int test = 0; test < t; ++test){
        memset(str, 0, sizeof(str));
        cin >> str;
        int n = strlen(str);
        initTree(n);
        while (true){
            string cmd; cin >> cmd;
            if (cmd[0] == 'E') break;
            if (cmd[0] == 'I'){
                string r;
                cin >> r; int x; cin >> x;
                for (int i = 0; i < r.size(); ++i){
                    insert(x + i, r[i]);
                }
            }
            if (cmd[0] == 'P'){
                int x, y; cin >> x >> y; x++, y++;
                for (int i = x; i <= y; ++i){
                    findKth(i);
                    cout << tree->v;
                } cout << '\n';
            }
        }
    }
}