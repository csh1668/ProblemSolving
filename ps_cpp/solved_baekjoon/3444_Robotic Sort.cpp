#include <bits/stdc++.h>

using namespace std;

typedef long long int ll;

struct node
{
    node *l, *r, *p;
    ll cnt, v;
    bool flip;

    node(ll v, node* p) : l(nullptr), r(nullptr), p(p), cnt(1), v(v) {}
    node(ll v) : node(v, nullptr) {}
    node() : node(0) {}

    ~node() {
        // if (l) delete l;
        // if (r) delete r;
    }
} *tree, *nodes[100010];

int arr[100010], tmp[100010];
unordered_map<int, int> compress;

void update(node* x){
    x->cnt = 1;
    if (x->l) {
        x->cnt += x->l->cnt;
    }
    if (x->r) {
        x->cnt += x->r->cnt;
    }
}

void propagate(node* x){
    if (!x->flip) return;
    swap(x->l, x->r);
    if (x->l) x->l->flip = !x->l->flip;
    if (x->r) x->r->flip = !x->r->flip;
    x->flip = false;
}

void rotate(node* x){
    node* p = x->p;
    node* b = nullptr;
    if (!p) return; // x가 루트라면 종료
    propagate(p); propagate(x);
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

void initTree(int n){
    tree = new node(numeric_limits<ll>::min()); // left dummy
    auto cur = tree;
    for (int i = 1; i <= n; i++){
        nodes[arr[i]] = cur->r = new node(arr[i], cur);
        cur = cur->r;
    }
    cur->r = new node(numeric_limits<ll>::max(), cur); // right dummy
    for (int i = n + 1; i >= 0; i--){
        if (nodes[i])
            update(nodes[i]);
    }
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

void flip(int s, int e){
    interval(s, e);
    auto cur = tree->r->l;
    cur->flip = !cur->flip;
}

// for splay tree study
void prefix(node* cur, int level=0, string str="ROOT:"){
    if (cur == nullptr) return;
    for (int i = 0; i < level; i++) cout << "  ";
    cout << str << " (" << cur->v << " " << cur->cnt << ")\n";
    prefix(cur->l, level + 1, "L--");
    prefix(cur->r, level + 1, "R--"); 
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    // freopen("testcase.txt", "r", stdin);
    // freopen("out1.txt", "w", stdout);

    while (true){
        // 입력 처리
        int n; cin >> n; if (n == 0) break;
        // tmp.clear(); tmp.reserve(n);
        compress.clear();
        for (int i = 1; i <= n; ++i){
            cin >> arr[i]; tmp[i] = arr[i];
        }
        // 좌표 압축
        sort(tmp, tmp + 1 + n);
        for (int i = 0; i <= n; ++i){
            if (compress.find(tmp[i]) == compress.end()) compress[tmp[i]] = i;
        }
        for (int i = 1; i <= n; ++i)
            arr[i] = compress[arr[i]]++;
        // for (int i = 1; i <= n; ++i)
        //     cout << arr[i] << " ";        
        // 트리 초기화
        initTree(n);
        // prefix(tree); cout << endl;

        // 쿼리 연산
        for (int i = 1; i <= n; ++i){
            splay(nodes[i]);
            int j = tree->l->cnt; cout << j << " ";
            if (j > 100000 || i > j) continue;
            flip(i, j);
            update(tree);
        }
        cout << endl;
    }
}