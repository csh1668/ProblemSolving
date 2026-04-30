#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef long long lint;


int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    string S; cin >> S;
    if (S.size() & 1) {
        cout << -1;
        return 0;
    }
    if (S == "10") {
        cout << -1;
        return 0;
    }

    int prev = -1;
    string ans;
    for (int i = 0; i < S.size(); i += 2) {
        int cnt = S[i] - '0', num = S[i + 1] - '0';
        if (prev == num || cnt == 0 || (i == 0 && num == 0)) {
            cout << -1;
            return 0;
        }
        prev = num;

        for (int j = 0; j < cnt; ++j) ans += (char)(num + '0');
    }

    cout << ans;
    

    return 0;
}