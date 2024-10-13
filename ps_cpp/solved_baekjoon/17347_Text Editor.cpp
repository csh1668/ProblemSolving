#include <bits/stdc++.h>
#include <ext/rope>

using namespace std;
using namespace __gnu_cxx;

char buffer[1234567];

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int t; cin >> t;
    rope<char> rp;

    int cursor = 0;
    while (t--){
        cin >> buffer;
        if (buffer[0] == 'M'){
            int x; cin >> x;
            cursor = x;
        } else if (buffer[0] == 'D'){
            int x; cin >> x;
            rp.erase(cursor, x);
        } else if (buffer[0] == 'P'){
            cursor--;
        } else if (buffer[0] =='N'){
            cursor++;
        } else if (buffer[0] == 'G'){
            int x; cin >> x;
            cout << rp.substr(cursor, x) << '\n';
        } else {
            // int n; cin >> n; cin.ignore();
            // for (int i = 0; i < n; ++i){
            //     buffer[i] = cin.get();
            //     while (buffer[i] == '\n'){
            //         buffer[i] = cin.get();
            //     }
            // }
            int n, cur = 0; cin >> n;
            while (cur < n){
                cin.getline(buffer + cur, n - cur + 1);
                cur += cin.gcount() - 1;
            }
            buffer[n] = '\0';
            rp.insert(cursor, buffer);
        }
    }
}