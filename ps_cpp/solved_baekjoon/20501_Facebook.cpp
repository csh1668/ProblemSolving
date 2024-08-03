#pragma GCC optimize ("O3")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,avx,avx2")
#pragma GCC optimize ("unroll-loops") 
#pragma GCC target("popcnt")
#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

bitset<2010> relations[2010];

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, q; cin >> n;
    for (int i = 0; i < n; ++i){
        string s; cin >> s;
        relations[i] = bitset<2010>(s);
    }
    cin >> q;
    for (int i = 0; i < q; ++i){
        int a, b; cin >> a >> b;
        cout << (relations[a - 1] & relations[b - 1]).count() << '\n';
    }
}