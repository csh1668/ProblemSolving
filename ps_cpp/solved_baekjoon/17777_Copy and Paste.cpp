#include <bits/stdc++.h>
#include <ext/rope>

using namespace std;
using namespace __gnu_cxx;

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, q; cin >> n;
    string s; cin >> s;
    rope<char> rp(s.c_str());

    cin >> q;
    while (q--){
        int a, b, c; cin >> a >> b >> c;
        auto sub = rp.substr(a, b - a);
        // rp.erase(a, b - a + 1);
        rp.insert(c, sub);
        if (rp.size() > n) rp.erase(n, rp.size() - n);
    }

    cout << rp << '\n';
}