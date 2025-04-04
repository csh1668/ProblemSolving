#pragma GCC optimize("O3")
#pragma GCC optimize("inline")
#pragma GCC target("avx,avx2,fma")
#pragma GCC optimize("unroll-loops")

#define private public
#include <bitset>
#undef private

#include <bits/stdc++.h>
#include <immintrin.h>

using namespace std;
typedef long long int ll;
constexpr int MAX = 100'005, INTERVAL_SIZE = 850;

template <size_t _Nw> inline int _find_kth_set_bit(const _Base_bitset<_Nw>& b, int k) {
    int cnt;
    for (int i = 0; i < _Nw; ++i) {
        if ((cnt = __builtin_popcountll(b._M_w[i])) >= k) {
            for (int j = 0; j < 64; ++j) {
                if (b._M_w[i] & (1LL << j)) {
                    if (--k == 0) return i * 64 + j;
                }
            }
        }
        k -= cnt;
    }
    return -1;
}

template <size_t _Nb> inline int find_kth_set_bit(const bitset<_Nb>& b, int k) {
    return _find_kth_set_bit(b, k);
}

struct SQDC {
    int naive[MAX];
    // bitset<MAX> st[SQ], res;
    // st: 버킷, res: 쿼리 결과 구할 때 쓸 임시 변수
    // tree: 버킷에 대한 세그먼트 트리
    bitset<MAX> st[MAX / INTERVAL_SIZE], res, tree[1 << (33 - __builtin_clz(MAX / INTERVAL_SIZE + 1))], empty;
    int N;

    void init(int N) {
        this->N = N;
        for (int i = 0, j; i < N; i = j) {
            j = min(i + INTERVAL_SIZE, N);
            init_bucket(st[i / INTERVAL_SIZE], i, j);
        }
        init_tree(0, N / INTERVAL_SIZE);
    }

    void init_tree(int l, int r, int i = 1) {
        if (l == r) {
            tree[i] = st[l];
            return;
        }
        int m = (l + r) >> 1;
        init_tree(l, m, i << 1); init_tree(m + 1, r, i << 1 | 1);
        tree[i] = tree[i << 1]; tree[i] |= tree[i << 1 | 1];
    }

    bitset<MAX> query_tree(int l, int r, int ql, int qr, int i = 1) {
        if (r < ql || qr < l) return empty;
        if (ql <= l && r <= qr) return tree[i];
        int m = (l + r) >> 1;
        bitset<MAX> res = query_tree(l, m, ql, qr, i << 1);
        res |= query_tree(m + 1, r, ql, qr, i << 1 | 1);
        return res;
    }

    // [l, r)
    void init_bucket(bitset<MAX>& bkt, int l, int r) {
        bkt.reset();
        for (int i = l; i < r; ++i) {
            bkt.set(naive[i]);
        }
    }

    // [l, r]
    int query(int l, int r, int k) {
        res.reset();
        for (int i = l; i <= r;) {
            if (i % INTERVAL_SIZE == 0 && i + INTERVAL_SIZE - 1 <= r) {
                // res |= st[i / SQ];
                // i += SQ;
                int j = 0;
                while ((i + j + INTERVAL_SIZE) + INTERVAL_SIZE - 1 <= r) j += INTERVAL_SIZE;
                res |= query_tree(0, N / INTERVAL_SIZE, i / INTERVAL_SIZE, (i + j) / INTERVAL_SIZE);
                i += j + INTERVAL_SIZE;
            } else {
                res.set(naive[i]);
                ++i;
            }
        }
        return find_kth_set_bit(res, k);
    }
} sqdc;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N;
    vector<int> tmp(N);
    for (int i = 0; i < N; ++i) cin >> sqdc.naive[i], tmp[i] = sqdc.naive[i];

    // 좌표 압축
    sort(tmp.begin(), tmp.end());
    tmp.erase(unique(tmp.begin(), tmp.end()), tmp.end());
    for (int i = 0; i < N; ++i) {
        sqdc.naive[i] = lower_bound(tmp.begin(), tmp.end(), sqdc.naive[i]) - tmp.begin();
    }

    sqdc.init(N);

    cin >> M;

    int prev = 0;
    for (int i = 0; i < M; ++i) {
        int a, b, c, d, k; cin >> a >> b >> c >> d >> k;
        // int l, r, k; cin >> l >> r >> k;

        int l = (1LL * a * max(prev, 0) + b) % N + 1,
            r = (1LL * c * max(prev, 0) + d) % N + 1;
        if (l > r) swap(l, r);

        int res = sqdc.query(l - 1, r - 1, k);
        if (res != -1) res = tmp[res];
        cout << res << '\n', prev = res;
    }
    

    return 0;
}