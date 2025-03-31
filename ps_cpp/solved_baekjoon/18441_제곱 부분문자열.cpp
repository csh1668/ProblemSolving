#pragma GCC optimize("O3")

#define USE_AVX2

#define private public
#include <bitset>
#undef private

#include <bits/stdc++.h>

#ifdef USE_AVX2
#pragma GCC target("avx2")
#include <immintrin.h>
#endif

using namespace std;
typedef unsigned long long int ll;

// reference : https://gist.github.com/cgiosy/a441de545c9e96b1d7b02cc7a00561f9?fbclid=IwAR0N3Woe8GwzAsxMapbEE9b7rrE_XArl50BRdQ9ZOTCxk-2X5BRrm-HBVpo
template <size_t _Nw> void _M_do_sub(_Base_bitset<_Nw>& x, const _Base_bitset<_Nw>& y) {
    for (size_t i = 0, carry = 0; i < _Nw; ++i) {
        carry = __builtin_ia32_sbb_u64(carry, x._M_w[i], y._M_w[i], (unsigned long long int*)(x._M_w + i));
    }
}
template <size_t _Nb> bitset<_Nb>& operator-=(bitset<_Nb>& x, const bitset<_Nb>& y) {
    _M_do_sub(x, y);
    return x;
}
template <size_t _Nb> bitset<_Nb> operator-(const bitset<_Nb>& x, const bitset<_Nb>& y) {
    bitset<_Nb> ret = x;
    return ret -= y;
}
// ==========

bitset<3001> X, S[26];
bitset<3001> *D;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    freopen("input.txt", "r", stdin);
    auto time = clock();
    #endif

    int T; cin >> T;
    for (int t = 1; t <= T; ++t) {
        string s; cin >> s;
        int mx_len = 0, n, m; bitset<3001> *mx_D = nullptr;
        string::iterator mx_A_iter, mx_B_iter;

        auto calc = [&](int A_size) {
            int N = A_size, M = s.size() - A_size;
            if (mx_len >= min(N, M)) return;
            auto A_iter = s.begin(), B_iter = s.begin() + A_size;
            if (N > M) swap(N, M), swap(A_iter, B_iter);

            D = new bitset<3001>[N];
            for (int i = 0; i < 26; ++i) S[i].reset();
            for (int i = 0; i < M; ++i) S[*(B_iter + i) - 'a'].set(i);
            for (int i = 0; i < N; ++i) {
                D[i].reset();
                X = S[*(A_iter + i) - 'a'];
                if (i) X |= D[i - 1], D[i] = D[i - 1] << 1;
                D[i].set(0);
                D[i] = X & (X ^ (X - D[i]));
            }

            if (mx_len >= D[N - 1].count()) {
                delete[] D;
                return;
            }

            delete[] mx_D;
            mx_D = D;
            mx_len = D[N - 1].count();
            n = N, m = M;
            mx_A_iter = A_iter, mx_B_iter = B_iter;
        };
        for (int offset = s.size() / 2; offset < s.size(); ++offset) {
            calc(offset);
            calc(s.size() - offset);
        }

        cout << "Case #" << t << ": " << mx_len * 2 << '\n';

        if (mx_len) {
            string mx_lcs;
            int i = n - 1, j = m - 1;
            mx_lcs.reserve(mx_len);
            while (i >= 0 && j >= 0) {
                if (*(mx_A_iter + i) == *(mx_B_iter + j)) {
                    mx_lcs.push_back(*(mx_A_iter + i));
                    --i, --j;
                } else if (i && mx_D[i - 1][j]) {
                    --i;
                } else {
                    --j;
                }
            }
    
    
            reverse(mx_lcs.begin(), mx_lcs.end());
            
            cout << mx_lcs << mx_lcs << '\n';

            delete[] mx_D;
        }

    }

    #ifndef ONLINE_JUDGE
    cerr << "Time: " << (double)(clock() - time) / CLOCKS_PER_SEC << "s\n";
    #endif

    return 0;
}