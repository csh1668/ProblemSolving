#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct Node {
    Node *l, *r;
    int prior, cnt;
    bool rev;

    ll val, sum;
    Node(ll v) : l(0), r(0), prior(rand()), cnt(1), rev(0), val(v), sum(v) {}
    void push() {
        if (rev) {
            swap(l, r);
            if (l) l->rev ^= 1;
            if (r) r->rev ^= 1;
            rev = 0;
        }
    }
    void pull() {
        cnt = 1; sum = val;
        if (l) cnt += l->cnt, sum += l->sum;
        if (r) cnt += r->cnt, sum += r->sum;
    }
};
typedef Node* pNode;

pair<pNode, pNode> split(pNode t, int k) {
    if (!t) return {0, 0};
    t->push();
    int lcnt = t->l ? t->l->cnt : 0;
    if (k <= lcnt) {
        auto [l, r] = split(t->l, k);
        t->l = r;
        t->pull();
        return {l, t};
    } else {
        auto [l, r] = split(t->r, k - lcnt - 1);
        t->r = l;
        t->pull();
        return {t, r};
    }
}

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

tuple<pNode, pNode, pNode> split(pNode t, int l, int r) {
    auto [A, BC] = split(t, l - 1);
    auto [B, C] = split(BC, r - l + 1);
    return {A, B, C};
}

pNode flip(pNode t, int l, int r) {
    auto [A, B, C] = split(t, l, r);
    if (B) B->rev ^= 1, B->push();
    return merge(A, merge(B, C));
}

void flip(pNode t) {
    if (!t) return;
    t->rev ^= 1;
    t->push();
}

void left_shift(pNode &t, int k) {
    if (!t || k <= 0) return;
    t->push();

    int mid = 
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    srand(time(0));

    return 0;
}