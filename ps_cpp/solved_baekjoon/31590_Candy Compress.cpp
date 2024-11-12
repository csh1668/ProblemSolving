#include <bits/stdc++.h>
#include <ext/rope>

using namespace std;
using namespace __gnu_cxx;

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n, q; cin >> n >> q;
    string s; cin >> s;
    rope<char> rp(s.c_str());

    while (q--){
        int op; cin >> op;
        if (op == 1){
            char c; int i; cin >> c >> i; i--;
            rp.insert(i, c);
        } else {
            int l, r; cin >> l >> r; l--, r--;
            cout << rp.substr(l, r - l + 1) << '\n';
            rp.erase(l, r - l + 1);
        }
    }
}