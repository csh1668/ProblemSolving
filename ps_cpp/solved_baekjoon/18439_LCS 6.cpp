#define USE_AVX2

#include <bits/stdc++.h>

#ifdef USE_AVX2
#pragma GCC target("avx2")
#include <immintrin.h>
#endif

using namespace std;
typedef unsigned long long int ll;
constexpr int MAXN = 1'010;

template <size_t SIZE> struct my_bitset {
    static constexpr size_t LEN = (SIZE >> 6) + 1;
    alignas(32) ll B[LEN];
    my_bitset() { clear(); }
    my_bitset(const my_bitset& x) { memcpy(B, x.B, sizeof(B)); }
    my_bitset& operator=(const my_bitset& x) { memcpy(B, x.B, sizeof(B)); return *this; }

    void set(size_t i) { B[i >> 6] |= 1LL << (i & 63); }
    void reset(size_t i) { B[i >> 6] &= ~(1LL << (i & 63)); }
    bool operator[](size_t i) const { return B[i >> 6] & (1LL << (i & 63)); }
    void clear() { memset(B, 0, sizeof(B)); }

    my_bitset& operator&=(const my_bitset& x) {
        size_t i = 0;
#ifdef USE_AVX2
        for (; i < LEN - 4; i += 4) {
            __m256i a = _mm256_load_si256((__m256i*)(B + i));
            __m256i b = _mm256_load_si256((__m256i*)(x.B + i));
            _mm256_store_si256((__m256i*)(B + i), _mm256_and_si256(a, b));
        }
#endif
        for (; i < LEN; ++i) B[i] &= x.B[i];
        return *this;
    }
    my_bitset& operator|=(const my_bitset& x) {
        size_t i = 0;
#ifdef USE_AVX2
        for (; i < LEN - 4; i += 4) {
            __m256i a = _mm256_load_si256((__m256i*)(B + i));
            __m256i b = _mm256_load_si256((__m256i*)(x.B + i));
            _mm256_store_si256((__m256i*)(B + i), _mm256_or_si256(a, b));
        }
#endif
        for (; i < LEN; ++i) B[i] |= x.B[i];
        return *this;
    }
    my_bitset& operator|=(const ll x) { B[0] |= x; return *this; }
    my_bitset& operator^=(const my_bitset& x) {
        size_t i = 0;
#ifdef USE_AVX2
        for (; i < LEN - 4; i += 4) {
            __m256i a = _mm256_load_si256((__m256i*)(B + i));
            __m256i b = _mm256_load_si256((__m256i*)(x.B + i));
            _mm256_store_si256((__m256i*)(B + i), _mm256_xor_si256(a, b));
        }
#endif
        for (; i < LEN; ++i) B[i] ^= x.B[i];
        return *this;
    }
    my_bitset& operator<<=(size_t x) {
        assert(0 <= x && x < 64);
        for (size_t i = LEN - 1; i > 0; --i) B[i] = (B[i] << x) | (B[i - 1] >> (64 - x));
        B[0] <<= x;
        return *this;
    }
    my_bitset& operator-=(const my_bitset& x) {
        for (size_t i = 0, carry = 0; i < LEN; ++i) {
            carry = __builtin_ia32_sbb_u64(carry, B[i], x.B[i], (unsigned long long int*)(B + i));
        }
        return *this;
    }

    my_bitset operator&(const my_bitset& x) const { my_bitset ret = *this; return ret &= x; }
    my_bitset operator|(const my_bitset& x) const { my_bitset ret = *this; return ret |= x; }
    my_bitset operator|(const ll x) const { my_bitset ret = *this; return ret |= x; }
    my_bitset operator^(const my_bitset& x) const { my_bitset ret = *this; return ret ^= x; }
    my_bitset operator<<(size_t x) const { my_bitset ret = *this; return ret <<= x; }
    my_bitset operator-(const my_bitset& x) const { my_bitset ret = *this; return ret -= x; }

    ll count() const {
        ll ret = 0;
        for (size_t i = 0; i < LEN; ++i) ret += __builtin_popcountll(B[i]);
        return ret;
    }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    string A, B; cin >> A >> B;
    int N = A.size(), M = B.size();

    my_bitset<MAXN> D, X, S[26];
    for (int i = 0; i < M; ++i) S[B[i] - 'A'].set(i);
    for (int i = 0; i < N; ++i) {
        X = S[A[i] - 'A'] | D;
        D = X & (X ^ (X - ((D << 1) | 1)));
    }

    cout << D.count() << '\n';

    return 0;
}