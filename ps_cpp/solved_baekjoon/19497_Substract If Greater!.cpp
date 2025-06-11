#include <bits/stdc++.h>

using namespace std;

struct Node {
    Node *l, *r;
    int prior, cnt;

    int val, lazy;
    Node(int v) : l(0), r(0), prior(rand()), cnt(1), val(v), lazy(0) {}
    void push() {
        if (lazy == 0) return;
        val += lazy;
        if (l) {
            l->lazy += lazy;
        }
        if (r) {
            r->lazy += lazy;
        }
        lazy = 0;
    }
    void pull() {
        cnt = 1;
        if (l) cnt += l->cnt;
        if (r) cnt += r->cnt;
    }
};
typedef Node* pNode;

// left: x보다 작거나 같은 원소들
// right: x보다 큰 원소들
pair<pNode, pNode> split(pNode t, int x) {
    if (!t) return {0, 0};
    t->push();
    if (t->val <= x) {
        auto [l, r] = split(t->r, x);
        t->r = l;
        t->pull();
        return {t, r};
    } else {
        auto [l, r] = split(t->l, x);
        t->l = r;
        t->pull();
        return {l, t};
    }
}

// left의 모든 원소는 right의 모든 원소보다 작거나 같아야 한다
pNode merge(pNode left, pNode right) {
    if (!left || !right) return left ? left : right;
    if (left->prior > right->prior) {
        left->push();
        left->r = merge(left->r, right);
        left->pull();
        return left;
    } else {
        right->push();
        right->l = merge(left, right->l);
        right->pull();
        return right;
    }
}

// x를 BST에 삽입
pNode insert(pNode t, pNode x) {
    if (!t) return x;
    t->push();
    auto [l, r] = split(t, x->val);
    t = merge(l, merge(x, r));
    t->pull();
    return t;
}

pNode find_kth(pNode t, int k) { // 1-based
    if (!t) return 0;
    t->push();
    int lcnt = t->l ? t->l->cnt : 0;
    if (k <= lcnt) {
        return find_kth(t->l, k);
    } else if (k == lcnt + 1) {
        return t;
    } else {
        return find_kth(t->r, k - lcnt - 1);
    }
}

void dump(pNode t, vector<pNode>& v) {
    if (!t) return;
    t->push();
    v.push_back(t);
    dump(t->l, v);
    dump(t->r, v);
}

void debug(pNode t, int depths = 0) {
    if (!t) return;
    t->push();
    debug(t->l, depths + 1);
    cout << string(depths, ' ') << "val: " << t->val << ", prior: " << t->prior
         << ", cnt: " << t->cnt << ", lazy: " << t->lazy << endl;
    debug(t->r, depths + 1);
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    srand(time(0));

    int N, Q; cin >> N >> Q;
    pNode root = 0;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        pNode node = new Node(x);
        root = insert(root, node);
    }

    while (Q--) {
        int op, x; cin >> op >> x;
        if (op == 1) {
            pNode i = find_kth(root, x);
            assert(i);
            cout << i->val << '\n';
        } else {
            auto [A, BC] = split(root, x);
            if (BC) {
                BC->lazy -= x;
                BC->push();

                auto [B, C] = split(BC, x);
                if (B) {
                    vector<pNode> v; v.reserve(B->cnt);
                    dump(B, v);
                    for (auto& e : v) {
                        e->l = 0; e->r = 0; e->pull();
                        A = insert(A, e);
                    }
                }

                root = merge(A, C);
            }
        }
    }

    return 0;
}