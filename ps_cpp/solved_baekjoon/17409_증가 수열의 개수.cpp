#include <bits/stdc++.h>

using namespace std;
constexpr int MOD = 1e9 + 7, MAXN = 1e5 + 5;

struct Fenwick {
    int tree[MAXN];
    Fenwick() { memset(tree, 0, sizeof(tree)); }
    void add(int i, int v) { for (; i < MAXN; i += i & -i) tree[i] = (tree[i] + v) % MOD; }
    int sum(int i) { int ret = 0; for (; i > 0; i -= i & -i) ret = (ret + tree[i]) % MOD; return ret; }
    int query(int l, int r) {
        if (l > r) return 0;
        return (sum(r) - sum(l - 1) + MOD) % MOD;
    }
};
// S: 각 원소의 개수를 저장
// fw[k]: a_x1 < a_x2 < ... < a_xk를 만족하는 (x1, x2, ..., xk) 쌍의 개수를 저장

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, K; cin >> N >> K; K--;
    if (K == 0) {
        cout << N << '\n';
        return 0;
    }
    Fenwick S, fw[K];

    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        S.add(x, 1);
        fw[0].add(x, S.query(1, x - 1));
        for (int k = 1; k < K; ++k) {
            fw[k].add(x, fw[k - 1].query(1, x - 1));
        }
    }

    cout << fw[K - 1].query(1, N) << '\n';

    return 0;
}