#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 100'001, OFFSET = (1 << (32 - __builtin_clz(MAX)));

int mx[OFFSET << 1], mn[OFFSET << 1];

void init(int n) {
    for (int i = 0; i < n; ++i)
        mx[i + OFFSET] = mn[i + OFFSET] = i;
    for (int i = OFFSET - 1; i; --i)
        mx[i] = max(mx[i << 1], mx[i << 1 | 1]),
        mn[i] = min(mn[i << 1], mn[i << 1 | 1]);
}

void update(int i, int v) {
    i |= OFFSET;
    mn[i] = mx[i] = v;
    while (i >>= 1) {
        mx[i] = max(mx[i << 1], mx[i << 1 | 1]);
        mn[i] = min(mn[i << 1], mn[i << 1 | 1]);
    }
}

pair<int, int> query(int l, int r) {
    l |= OFFSET, r |= OFFSET;
    int mxv = 0, mnv = MAX;
    while (l <= r) {
        if (l & 1) {
            mxv = max(mxv, mx[l]);
            mnv = min(mnv, mn[l++]);
        }
        if (~r & 1) {
            mxv = max(mxv, mx[r]);
            mnv = min(mnv, mn[r--]);
        }
        l >>= 1, r >>= 1;
    }
    return {mnv, mxv};
}


int main() {
    ios_base::sync_with_stdio(false); cin.tie(nullptr); cout.tie(nullptr);

    int t; cin >> t;
    while (t--) {
        int n, k; cin >> n >> k;
        init(n);
        while (k--) {
            int q, a, b; cin >> q >> a >> b;
            if (q == 0) {
                int pa = mn[OFFSET + a], pb = mn[OFFSET + b];
                update(a, pb);
                update(b, pa);
            } else {
                auto [mnv, mxv] = query(a, b);
                if (mnv == a && mxv == b) {
                    cout << "YES\n";
                } else {
                    cout << "NO\n";
                }
            }
        }
    }

    return 0;
}