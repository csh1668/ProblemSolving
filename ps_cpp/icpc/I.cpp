#include <bits/stdc++.h>
#include <iomanip>
using namespace std;
using ll = long long;
typedef long long lint;


int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);


    ll k;

    cin >> k;
    int flag = 0;
    if (k > 0) flag = 1, k = -k;

    set<int> ans;

    for (ll i = 0; i <= 1e8;i++) {
        if (i + k == 0) continue;
        bool jung = (i * i) % (i+k) == 0;
        if (!jung) continue;

        long long p = -i * i / (i+k);

        ans.insert(p);
    }

    long long sum = 0;

    // for (auto i : ans) cout << i <<  ' '; cout << endl;
    // for (auto i : ans) sum += i;

    // if (flag) sum = - sum;
    if (flag) k = -k;

    cout << ans.size() << ' ' << (ll) ans.size() * k * 2;
    return 0;
}