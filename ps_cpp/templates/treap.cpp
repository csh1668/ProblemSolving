#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct Data {
    int k;
    Data(int k = 0) : k(k) {}
    bool operator<(const Data &other) const {
        return k < other.k;
    }
    bool operator<=(const Data &other) const {
        return k <= other.k;
    }
    bool operator==(const Data &other) const {
        return k == other.k;
    }
    Data operator+(const Data &other) const {
        return Data(k + other.k);
    }
    Data operator+=(const Data &other) {
        k += other.k;
        return *this;
    }
};

struct Node {
    Node *l, *r;
    int prior, cnt;

    Data key, sum;
    Node(Data k) : l(0), r(0), prior(rand()), cnt(1), key(k), sum(k) {}
    void pull() {
        cnt = 1; sum = key;
        if (l) cnt += l->cnt, sum += l->sum;
        if (r) cnt += r->cnt, sum += r->sum;
    }
};
typedef Node* pNode;

pair<pNode, pNode> split(pNode t, Data k) {
    if (!t) return {0, 0};
    if (t->key < k) {
        auto [l, r] = split(t->r, k);
        t->r = l; t->pull();
        return {t, r};
    } else {
        auto [l, r] = split(t->l, k);
        t->l = r; t->pull();
        return {l, t};
    }
}

pNode merge(pNode left, pNode right) {
    if (!left || !right) return left ? left : right;
    if (left->prior < right->prior) {
        left->r = merge(left->r, right);
        left->pull();
        return left;
    } else {
        right->l = merge(left, right->l);
        right->pull();
        return right;
    }
}

void insert(pNode &t, Data key) {
    auto [l, r] = split(t, key);
    t = merge(merge(l, new Node(key)), r);
}

void erase_one(pNode &t, Data key) {
    if (!t) return;
    if (t->key == key) {
        pNode to_delete = t;
        t = merge(t->l, t->r);
        delete to_delete;
    } else if (t->key < key) {
        erase_one(t->r, key);
    } else {
        erase_one(t->l, key);
    }
    t->pull();
    return;
}

Data find(pNode t, Data key) {
    if (!t) return Data();
    if (t->key == key) return t->key;
    if (t->key < key) return find(t->r, key);
    return find(t->l, key);
}

int find_idx(pNode t, Data key) { // 0-based
    if (!t) return -1;
    if (t->key == key) {
        int lcnt = t->l ? t->l->cnt : 0;
        return lcnt;
    }
    if (t->key < key) {
        int r_index = find_idx(t->r, key);
        return r_index == -1 ? -1 : (t->l ? t->l->cnt : 0) + 1 + r_index;
    }
    return find_idx(t->l, key);
}

int lower_bound(pNode t, Data key) {
    if (!t) return 0;
    if (t->key < key) {
        int r_index = lower_bound(t->r, key);
        return (t->l ? t->l->cnt : 0) + 1 + r_index;
    } else {
        return lower_bound(t->l, key);
    }
}

int upper_bound(pNode t, Data key) {
    if (!t) return 0;
    if (t->key <= key) {
        int r_index = upper_bound(t->r, key);
        return (t->l ? t->l->cnt : 0) + 1 + r_index;
    } else {
        return upper_bound(t->l, key);
    }
}

Data find_kth(pNode t, int k) { // 0-based
    if (!t || k < 0 || k >= t->cnt) return Data();
    int lcnt = t->l ? t->l->cnt : 0;
    if (k < lcnt) {
        return find_kth(t->l, k);
    } else if (k == lcnt) {
        return t->key;
    } else {
        return find_kth(t->r, k - lcnt - 1);
    }
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    srand(time(0));

    pNode treap = nullptr;
    insert(treap, Data(5));
    insert(treap, Data(3));
    insert(treap, Data(8));
    insert(treap, Data(1));
    insert(treap, Data(4));

    return 0;
}