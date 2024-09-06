#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

ll a[5001];

inline ll eval(tuple<ll, ll, ll> t){
    return abs(get<0>(t) + get<1>(t) + get<2>(t));
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n; cin >> n;
    for (int i = 0; i < n; ++i) cin >> a[i];
    a[n] = 1e18;
    sort(a, a + n);

    tuple<ll, ll, ll> ans = {1e12, 1e12, 1e12};

    for (int i = 0; i < n - 2; ++i){
        for (int j = i + 1; j < n - 1; ++j) {
            ll t = -(a[i] + a[j]);
            int k = lower_bound(a + j + 1, a + n, t) - a;
            if (abs(-t + a[k]) < eval(ans)){
                ans = {a[i], a[j], a[k]};
            }
            if (k > j + 1 && abs(-t + a[k - 1]) < eval(ans)){
                ans = {a[i], a[j], a[k - 1]};
            }
        }
    }

    cout << get<0>(ans) << ' ' << get<1>(ans) << ' ' << get<2>(ans) << '\n';
}