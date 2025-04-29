#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct Node {
    Node *l, *r, *p;
    int cnt, debug;
    ll val, lval, rval, add_lazy;

    int pre_inc, pre_dec, suf_inc, suf_dec;
    ll inc_sum, dec_sum;

    bool flip;
    Node(ll v = 0, Node *p = 0) {
        l = r = 0;
        this->p = p;
        cnt = 1;
        val = lval = rval = v;
        pre_inc = pre_dec = suf_inc = suf_dec = inc_sum = dec_sum = 1;
        add_lazy = 0;
        flip = 0;
    }
    ~Node() {
        if (l) delete l;
        if (r) delete r;
    }
    bool all_inc() {
        assert(flip == 0);
        return pre_inc == cnt;
    }
    bool all_dec() {
        assert(flip == 0);
        return pre_dec == cnt;
    }
} *root = 0;

void push(Node *x) {
    if (x->flip) {
        swap(x->l, x->r);
        swap(x->lval, x->rval);
        swap(x->pre_inc, x->suf_dec);
        swap(x->suf_inc, x->pre_dec);
        swap(x->inc_sum, x->dec_sum);
        if (x->l) x->l->flip ^= 1;
        if (x->r) x->r->flip ^= 1;
        x->flip = 0;
    }
    if (x->add_lazy) {
        x->val += x->add_lazy;
        x->lval += x->add_lazy;
        x->rval += x->add_lazy;
        if (x->l) x->l->add_lazy += x->add_lazy;
        if (x->r) x->r->add_lazy += x->add_lazy;
        x->add_lazy = 0;
    }
}

void pull(Node *x) {
    x->cnt = x->pre_inc = x->pre_dec = x->suf_inc = x->suf_dec = x->inc_sum = x->dec_sum = 1;
    x->lval = x->rval = x->val;
    Node *left = x->l, *right = x->r;
    if (left && right) {
        push(left); push(right);
        x->cnt += left->cnt + right->cnt;
        x->lval = left->lval; x->rval = right->rval;

        x->inc_sum += left->inc_sum + right->inc_sum;
        if (left->rval < x->val && x->val < right->lval) {
            x->inc_sum += left->suf_inc * right->pre_inc;
        }
        if (left->rval < x->val) x->inc_sum += left->suf_inc;
        if (x->val < right->lval) x->inc_sum += right->pre_inc;

        x->dec_sum += left->dec_sum + right->dec_sum;
        if (left->rval > x->val && x->val > right->lval) {
            x->dec_sum += left->suf_dec * right->pre_dec;
        }
        if (left->rval > x->val) x->dec_sum += left->suf_dec;
        if (x->val > right->lval) x->dec_sum += right->pre_dec;

        x->pre_inc = left->pre_inc;
        if (left->all_inc() && left->rval < x->val) {
            x->pre_inc++;
            if (x->val < right->lval) {
                x->pre_inc += right->pre_inc;
            }
        }
        x->pre_dec = left->pre_dec;
        if (left->all_dec() && left->rval > x->val) {
            x->pre_dec++;
            if (x->val > right->lval) {
                x->pre_dec += right->pre_dec;
            }
        }

        x->suf_inc = right->suf_inc;
        if (right->all_inc() && x->val < right->lval) {
            x->suf_inc++;
            if (left->rval < x->val) {
                x->suf_inc += left->suf_inc;
            }
        }
        x->suf_dec = right->suf_dec;
        if (right->all_dec() && x->val > right->lval) {
            x->suf_dec++;
            if (left->rval > x->val) {
                x->suf_dec += left->suf_dec;
            }
        }
    } else if (left) {
        push(left);
        x->cnt += left->cnt;
        x->lval = left->lval;
        x->inc_sum += left->inc_sum;
        if (left->rval < x->val) x->inc_sum += left->suf_inc;
        x->dec_sum += left->dec_sum;
        if (left->rval > x->val) x->dec_sum += left->suf_dec;

        x->pre_inc = left->pre_inc;
        if (left->all_inc() && left->rval < x->val) {
            x->pre_inc++;
        }
        if (left->rval < x->val) {
            x->suf_inc += left->suf_inc;
        }

        x->pre_dec = left->pre_dec;
        if (left->all_dec() && left->rval > x->val) {
            x->pre_dec++;
        }
        if (left->rval > x->val) {
            x->suf_dec += left->suf_dec;
        }
    } else if (right) {
        push(right);
        x->cnt += right->cnt;
        x->rval = right->rval;
        x->inc_sum += right->inc_sum;
        if (x->val < right->lval) x->inc_sum += right->pre_inc;
        x->dec_sum += right->dec_sum;
        if (x->val > right->lval) x->dec_sum += right->pre_dec;

        x->suf_inc = right->suf_inc;
        if (right->all_inc() && x->val < right->lval) {
            x->suf_inc++;
        }
        if (x->val < right->lval) {
            x->pre_inc += right->pre_inc;
        }

        x->suf_dec = right->suf_dec;
        if (right->all_dec() && x->val > right->lval) {
            x->suf_dec++;
        }
        if (x->val > right->lval) {
            x->pre_dec += right->pre_dec;
        }
    }
}

void rotate(Node *x) {
    Node *p = x->p, *b = 0;
    if (!p) return;
    push(p); push(x);
    if (x == p->l) {
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

void splay(Node *x) {
    while (x->p) {
        Node *p = x->p, *g = p->p;
        if (g) {
            if ((x == p->l) == (p == g->l)) rotate(p);
            else rotate(x);
        }
        rotate(x);
    }
}

void findKth(int k) {
    Node *x = root;
    push(x);
    while (1) {
        while (x->l && x->l->cnt > k) x = x->l, push(x);
        if (x->l) k -= x->l->cnt;
        if (!k--) break;
        x = x->r;
        push(x);
    }
    splay(x);
}

Node *interval(int s, int e) {
    findKth(s - 1);
    Node *x = root;
    root = x->r;
    root->p = 0;
    findKth(e - s + 1);
    x->r = root; root->p = x;
    root = x;
    if (root->r && root->r->l) {
        push(root->r->l);
        return root->r->l;
    }
    return 0;
}

#ifndef ONLINE_JUDGE
void print(Node *x, int depth = 0) {
    if (!x) return;
    push(x);
    print(x->l, depth + 1);
    cout << string(depth * 2, ' ') << "Node: ";
    cout << format("val: {}\tcnt: {}\tpre_inc: {}\tsuf_inc: {}\tinc_sum: {}\tpre_dec: {}\tsuf_dec: {}\tdec_sum: {}\tdebug: {}\t",
        x->val, x->cnt, x->pre_inc, x->suf_inc, x->inc_sum, x->pre_dec, x->suf_dec, x->dec_sum, x->debug);
    cout << endl;
    print(x->r, depth + 1);
}
#endif

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    freopen("test.txt", "r", stdin);
    ifstream a("answer.txt");
    #endif

    int T; cin >> T;
    while (T--) {
        int N, Q; cin >> N >> Q;
        root = new Node(0); root->debug = 0;
        Node *last = root;
        for (int i = 1; i <= N; ++i) {
            int x; cin >> x;
            Node *new_node = new Node(x, last); new_node->debug = i;
            last->r = new_node;
            last = new_node;
        }
        last = new Node(0, last); last->debug = N + 1;
        last->p->r = last;
        while (last->p) {
            pull(last);
            last = last->p;
        }

        char buf[4];
        for (int i = 0; i < Q; ++i) {
            cin >> buf;
            if (buf[0] == 'I') {
                int i, j, k; cin >> i >> j >> k;
                Node *x = interval(i, j);
                x->add_lazy += k;
                push(x);
            } else {
                int i, j; cin >> i >> j;
                Node *x = interval(i, j);
                x->flip ^= 1;
                push(x);
            }
            
            // #ifndef ONLINE_JUDGE
            // cout << "====================" << endl;
            // print(interval(1, N));
            // cout << "====================" << endl;
            // cout << endl;
            // #endif


            #ifdef ONLINE_JUDGE
            cout << interval(1, N)->inc_sum << '\n';
            #else
            ll ans = interval(1, N)->inc_sum;
            ll corr; a >> corr;
            if (ans != corr) {
                cout << "Wrong answer: " << ans << " != " << corr << " at " << i + 1 << endl;
                return 0;
            }
            #endif
        }

        delete root;
    }

    return 0;
}