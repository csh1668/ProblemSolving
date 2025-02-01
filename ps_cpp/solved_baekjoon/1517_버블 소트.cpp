#pragma GCC optimize("O3")

#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 500'001, SIZE = 1 << 20;

struct FenwickTree {
    int tree[SIZE];
    FenwickTree() { fill(tree, tree + SIZE, 0); }
    void update(int i, int d) {
        for (; i < SIZE; i += i & -i) tree[i] += d;
    }
    int _query(int i) {
        int ret = 0;
        for (; i > 0; i -= i & -i) ret += tree[i];
        return ret;
    }
    int query(int l, int r) { return _query(r) - _query(l - 1); }
} ft;

template <typename T>
struct MyQueue {
    vector<T> v;
    int l;
    MyQueue() : l(0) {}
    void push(int x) { v.push_back(x); }
    void pop() { ++l; }
    T& front() { return v[l]; }
    bool empty() { return l == v.size(); }
};

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    map<int, MyQueue<int>> m;

    for (int i = 1; i <= N; ++i) {
        int x; cin >> x;
        m[x].push(i);
        ft.update(i, 1);
    }

    ll ans = 0;
    auto it = m.begin();
    while (it != m.end()) {
        auto& x = *it;
        
        while (!x.second.empty()) {
            ans += ft.query(1, x.second.front() - 1);
            ft.update(x.second.front(), -1);
            x.second.pop();
        }

        ++it;
    }

    cout << ans << '\n';

    return 0;
}