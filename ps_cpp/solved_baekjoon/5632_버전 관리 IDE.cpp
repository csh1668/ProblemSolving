#include <bits/stdc++.h>
#include <ext/rope>

using namespace std;
using namespace __gnu_cxx;

rope<char> ropes[50001];

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int t, vnow = 0, cntc = 0; cin >> t;
    for (int i = 0; i < t; ++i){
        int op; cin >> op;
        if (op == 1){
            int p; string s; cin >> p >> s;
            p -= cntc;
            ropes[vnow + 1] = ropes[vnow];
            ++vnow;
            ropes[vnow].insert(p, s.c_str());
        } else if (op == 2){
            int p, c; cin >> p >> c;
            p -= cntc, c -= cntc;
            ropes[vnow + 1] = ropes[vnow];
            ++vnow;
            ropes[vnow].erase(p - 1, c);
        } else {
            int v, p, c; cin >> v >> p >> c;
            v -= cntc, p -= cntc, c -= cntc;
            auto res = ropes[v].substr(p - 1, c);
            cout << res << '\n';
            for (int i = 0; i < res.size(); ++i)
                if (res[i] == 'c') ++cntc;
        }
    }
}