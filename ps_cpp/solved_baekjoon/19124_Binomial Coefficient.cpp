/*
ref: https://simonlindholm.github.io/files/bincoef.pdf
N!/(K!(N-K)!) = (Nodd/(Kodd(N-K)odd)) * 2 ^ (Neven - Keven - (N - K)even)

Nodd = 1 * 3 * 5 * ... * N = N!!
Neven = Sigma(i=1, N) x//2^i
*/

#pragma GCC optimize ("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")
#pragma GCC optimize ("unroll-loops") 
#include<bits/stdc++.h>

typedef long long int ll;
using namespace std;

// unsigned int를 사용하면 mod 2^32가 자동으로 됨
uint32_t pow_mod(uint32_t a, ll e){
    uint32_t res = 1;
    while (e){
        if (e & 1)
            res *= a;
        a = a * a;
        e >>= 1;
    }
    return res;
}


int main(){
    ll n, k;
    cin >> n >> k;

    ll e = 0;
    map<uint32_t, int> m;
    int accMult = 0;
    
    for (int i = 0; i < 3; i++){
        ll x = (i == 0 ? n : i == 1 ? k : n - k);
        int mult = (i == 0 ? 1 : -1);
        while (x > 0){
            m[(uint32_t)x + 1] += mult;
            accMult += mult;
            x /= 2;
            e += x * mult;
        }
    }

    uint32_t cur = 0, res = 1, resdiv = 1;
    for (auto pair : m){
        uint32_t lim = pair.first, ilim = lim / 2;
        uint32_t product = 1;
        for (; cur < ilim; cur++)
            product *= cur * 2 + 1;
        if (accMult > 0) res *= pow_mod(product, accMult);
        else resdiv *= pow_mod(product, -accMult);
        accMult -= pair.second;
    }

    res *= pow_mod(2, e);
    res *= pow_mod(resdiv, (1ll << 32) - 1);

    cout << res << endl;
}