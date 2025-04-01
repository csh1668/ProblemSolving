#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 300'003, SIZE = 1 << (33 - __builtin_clz(MAXN));

int P[MAXN];
vector<int> tree[SIZE];

vector<int>& init(int l, int r, int i = 1) {
    if (l == r) {
        tree[i].push_back(P[l]);
        return tree[i];
    }
    int m = l + r >> 1;
    auto &left = init(l, m, i << 1), &right = init(m + 1, r, i << 1 | 1);
    tree[i].resize(left.size() + right.size());
    merge(left.begin(), left.end(), right.begin(), right.end(), tree[i].begin());
    return tree[i];
}

int query(int l, int r, int ql, int qr, int k, int i = 1) {
    if (qr < l || r < ql) return 0;
    if (ql <= l && r <= qr) {
        auto &v = tree[i];
        auto it = lower_bound(v.begin(), v.end(), k);

        // for (auto& e: v) {
        //     cout << e << ' ';
        // }
        // cout << "it: " << it - v.begin() << "\n";

        return it - v.begin();
    }
    int m = l + r >> 1;
    return query(l, m, ql, qr, k, i << 1) + query(m + 1, r, ql, qr, k, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;
    for (int i = 2; i <= N; ++i) {
        cin >> P[i];
    }
    init(1, N);

    cin >> M;
    while (M--) {
        int l, r; cin >> l >> r;
        cout << query(1, N, l, r, l) << '\n';
    }

    return 0;
}