#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 500'002, SGSZ = 1 << (33 - __builtin_clz(MAX));

struct PST {
    struct Node {
        int l, r, v;
        Node() : l(-1), r(-1), v(0) {}
        Node(int v) : l(-1), r(-1), v(v) {}
        Node(int l, int r, int v) : l(l), r(r), v(v) {}
    };
    vector<Node> nodes;
    vector<int> roots;
    int N;
    PST(int n) : N(n) {
        roots.push_back(build(0, N));
    }
    
    int build(int s, int e) {
        if (s == e) nodes.emplace_back(Node(0));
        else {
            int m = s + e >> 1;
            int l = build(s, m), r = build(m + 1, e);
            nodes.emplace_back(Node(l, r, 0));
        }
        return nodes.size() - 1;
    }

    int _update_add(int node, int i, int v, int s, int e) {
        Node cur = nodes[node];
        if (s == e) nodes.push_back(Node(cur.v + v));
        else {
            int m = s + e >> 1;
            if (i <= m) {
                int l = _update_add(cur.l, i, v, s, m);
                nodes.push_back(Node(l, cur.r, cur.v + v));
            } else {
                int r = _update_add(cur.r, i, v, m + 1, e);
                nodes.push_back(Node(cur.l, r, cur.v + v));
            }
        }
        return nodes.size() - 1;
    }

    void update_add(int i, int v) {
        roots.push_back(_update_add(roots.back(), i, v, 0, N));
    }

    int _query_sum(int node, int l, int r, int s, int e) {
        Node cur = nodes[node];
        if (r < s || e < l) return 0;
        if (l <= s && e <= r) return cur.v;
        int m = s + e >> 1;
        return _query_sum(cur.l, l, r, s, m) + _query_sum(cur.r, l, r, m + 1, e);
    }

    int query_sum(int l, int r, int x) {
        if (l > r) return 0;
        if (l <= 0) return _query_sum(roots[r], 0, x, 0, N);
        return _query_sum(roots[r], 0, x, 0, N) - _query_sum(roots[l - 1], 0, x, 0, N);
    }

    int _query_kth(int lnode, int rnode, int l, int r, int k) {
        if (l == r) return l;
        Node lcur = nodes[lnode], rcur = nodes[rnode];
        int m = l + r >> 1;
        int left = nodes[rcur.l].v - nodes[lcur.l].v;
        if (k <= left) return _query_kth(lcur.l, rcur.l, l, m, k);
        return _query_kth(lcur.r, rcur.r, m + 1, r, k - left);
    }

    int query_kth(int l, int r, int k) {
        return _query_kth(roots[l - 1], roots[r], 0, N, k);
    }

    int _query_xor(int lnode, int rnode, int l, int r, int x) {
        if (l == r) return l;
        Node lcur = nodes[lnode], rcur = nodes[rnode];
        int m = l + r >> 1;
        int left = nodes[rcur.l].v - nodes[lcur.l].v;
        int right = nodes[rcur.r].v - nodes[lcur.r].v;
        if (x & ((r - l + 1) >> 1) && left || !right) return _query_xor(lcur.l, rcur.l, l, m, x);
        return _query_xor(lcur.r, rcur.r, m + 1, r, x);
    }

    int query_xor(int l, int r, int x) {
        return _query_xor(roots[l - 1], roots[r], 0, N, x);
    }
} pst((SGSZ >> 1) - 1);

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int M; cin >> M;
    for (int i = 0; i < M; ++i) {
        int op; cin >> op;
        if (op == 1) {
            int x; cin >> x;
            pst.update_add(x, 1);
        } else if (op == 2) {
            int l, r, x; cin >> l >> r >> x;
            cout << pst.query_xor(l, r, x) << '\n';
        } else if (op == 3) {
            int k; cin >> k;
            while (k--) { pst.roots.pop_back(); }
        } else if (op == 4) {
            int l, r, x; cin >> l >> r >> x;
            cout << pst.query_sum(l, r, x) << '\n';
        } else if (op == 5) {
            int l, r, k; cin >> l >> r >> k;
            cout << pst.query_kth(l, r, k) << '\n';
        }
    }

    return 0;
}