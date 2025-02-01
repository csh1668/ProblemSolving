#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 1'000'005;

struct PST {
    struct Node {
        int v, l, r;
        Node(int v = 0, int l = -1, int r = -1) : v(v), l(l), r(r) {}
    };
    vector<Node> nodes; vector<int> roots;
    int N;
    PST(int N) : N(N) {
        roots.reserve(N);
        nodes.reserve(N * 50);
        roots.push_back(_build(0, N - 1));
    }

    int _build(int s, int e) {
        if (s == e) nodes.emplace_back(0);
        else {
            int m = s + e >> 1;
            int l = _build(s, m), r = _build(m + 1, e);
            // nodes.emplace_back(nodes[l].v + nodes[r].v, l, r);
            nodes.emplace_back(0, l, r);
        }
        return nodes.size() - 1;
    }

    int _update_add(int n, int s, int e, int idx, int v) {
        #define left nodes[n].l
        #define right nodes[n].r
        #define value nodes[n].v

        if (s == e) nodes.emplace_back(value + v);
        else {
            int m = s + e >> 1;
            if (idx <= m) {
                int l = _update_add(left, s, m, idx, v);
                nodes.emplace_back(v + value, l, right);
            } else {
                int r = _update_add(right, m + 1, e, idx, v);
                nodes.emplace_back(v + value, left, r);
            }
        }

        return nodes.size() - 1;
    }

    int update_add(int idx, int v) {
        int p = nodes.size();
        roots.push_back(_update_add(roots.back(), 0, N - 1, idx, v));
        return roots.back();
    }

    int _query(int n, int s, int e, int l, int r) {
        #define left nodes[n].l
        #define right nodes[n].r
        #define value nodes[n].v

        if (r < s || e < l) return 0;
        if (l <= s && e <= r) return value;
        int m = s + e >> 1;
        return _query(left, s, m, l, r) + _query(right, m + 1, e, l, r);
    }

    int query(int root, int l, int r) {
        return _query(root, 0, N - 1, l, r);
    }
};

int A[MAXN], tmp[MAXN], last[MAXN], version[MAXN];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N;
    PST pst(N + 5);

    for (int i = 0; i < N; ++i) cin >> A[i], tmp[i] = A[i];

    // 좌표 압축
    sort(tmp, tmp + N);
    int M = unique(tmp, tmp + N) - tmp;

    for (int i = 0; i < N; ++i) {
        A[i] = lower_bound(tmp, tmp + M, A[i]) - tmp;
    }

    fill(last, last + M, -1);
    for (int i = 0; i < N; ++i) {
        if (last[A[i]] != -1) pst.update_add(last[A[i]], -1);
        last[A[i]] = i;
        version[i] = pst.update_add(i, 1);
    }

    // for (int v = 0; v < N; ++v) {
    //     cout << "version " << v << ": ";
    //     for (int i = 0; i < N; ++i) {
    //         cout << pst.query(version[v], i, i) << ' ';
    //     }
    //     cout << '\n';
    // }

    cin >> Q;
    int q = 0;
    while (Q--) {
        int x, r; cin >> x >> r; --x, --r;
        int l = x + q;

        int ans = pst.query(version[r], l, r);
        cout << (q = ans) << '\n';

        assert(pst.nodes.size() < (1'000'005 * 50));
        // cout << "version: " << r << "(" << version[r] << "), query: " << l << " ~ " << r << " = " << ans << '\n';
        // q = ans;
    }

    return 0;
}