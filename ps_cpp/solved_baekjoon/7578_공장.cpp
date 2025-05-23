#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 500'002, SZ = 1 << 20, OFF = 1 << 19;
typedef long long ll;

int T[SZ];

void update(int pos, int v) {
    pos |= OFF;
    T[pos] += v;
    for (pos >>= 1; pos; pos >>= 1) {
        T[pos] = T[pos << 1] + T[pos << 1 | 1];
    }
}

int query(int l, int r) {
    l |= OFF; r |= OFF;
    int ans = 0;
    while (l <= r) {
        if (l & 1) ans += T[l++];
        if (!(r & 1)) ans += T[r--];
        l >>= 1; r >>= 1;
    }
    return ans;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    map<int, int> ord;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        ord[x] = i;
    }

    ll ans = 0;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        x = ord[x];
        update(x, 1);
        ans += query(x + 1, N - 1);
    }

    cout << ans << '\n';

    return 0;
}