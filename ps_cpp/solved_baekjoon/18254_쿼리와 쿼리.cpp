#pragma GCC optimize("03")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'005, BUCKET_SIZE = 200;
// constexpr int MAXN = 12, BUCKET_SIZE = 2;

constexpr int BUCKET_COUNT = MAXN / BUCKET_SIZE + 1, SEG_SIZE = 1 << (33 - __builtin_clz(MAXN));

int N;

struct Fenwick {
    int tree[MAXN];
    void update(int i, int v) { for (; i < MAXN; i += i & -i) tree[i] ^= v; }
    int sum(int i) { int ret = 0; for (; i > 0; i -= i & -i) ret ^= tree[i]; return ret; }
    int query(int l, int r) { return sum(r) ^ sum(l - 1); }
};

struct LazyXOR {
    Fenwick O, E;
    // [p, N]
    inline void update(int p, int v) {
        if (p & 1) O.update(p, v);
        else E.update(p, v); 
    }
    // [1, q]
    inline int query(int q) {
        if (q & 1) return O.sum(q);
        else return E.sum(q);
    }
    void update(int l, int r, int v) {
        update(l, v);
        update(r + 1, v);
    }
    int query(int l, int r) {
        return query(r) ^ query(l - 1);
    }
} ft;

struct Update {
    int l, r;
} updates[MAXN];

unsigned char cover[BUCKET_COUNT][MAXN];
int bucket[BUCKET_COUNT];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int M, Q; cin >> N >> M >> Q;
    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        ft.update(i, i, x);
    }

    for (int i = 0; i < M;) {
        int j = min(i + BUCKET_SIZE - 1, M - 1);
        int b = i / BUCKET_SIZE;
        for (int k = i; k <= j; ++k) {
            int l, r, x; cin >> l >> r >> x;
            updates[k] = {l, r};
            ft.update(l, r, x);
            cover[b][l] ^= 1;
            cover[b][r + 1] ^= 1;
        }

        for (int k = 2; k <= N; ++k) cover[b][k] ^= cover[b][k - 1]; // 업데이트 연산이 홀수번 덮는 위치가 1이된다
        for (int k = 2; k <= N; ++k) cover[b][k] ^= cover[b][k - 1]; // prefix-sum으로 [L-R] 범위를 덮는 수의 개수를 O(1)에 구할 수 있도록 전처리

        i = j + 1;
    }

    while (Q--) {
        int op, l, r; cin >> op >> l >> r;
        if (op == 1) {
            int v; cin >> v;
            --l, --r;
            for (int i = l; i <= r;) {
                if (i % BUCKET_SIZE == 0 && i + BUCKET_SIZE - 1 <= r) {
                    int b = i / BUCKET_SIZE;
                    bucket[b] ^= v;
                    i += BUCKET_SIZE;
                } else {
                    ft.update(updates[i].l, updates[i].r, v);
                    ++i;
                }
            }
        } else {
            int ans = ft.query(l, r);
            for (int i = 0; i < BUCKET_COUNT; ++i) {
                if (cover[i][l - 1] ^ cover[i][r]) ans ^= bucket[i];
            }
            cout << ans << '\n';
        }
    }

    return 0;
}