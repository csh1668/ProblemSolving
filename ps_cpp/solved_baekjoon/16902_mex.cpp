#include <bits/stdc++.h>

using namespace std;
constexpr int MAX = 300'002, LOG = __lg(MAX) + 1;

struct Node {
    int child[2];
    int cnt;
    Node() : cnt(0) { }
};
vector<Node> T(1);
int X;

int& child(int cur, int B, int i) {
    return T[cur].child[B ^ (X >> i) & 1];
}

void insert(int x) {
    int cur = 0;
    for (int i = LOG - 1; i >= 0; i--) {
        int B = (x >> i) & 1;
        if (!child(cur, B, i)) {
            child(cur, B, i) = T.size();
            T.emplace_back();
        }
        T[cur].cnt++;
        cur = child(cur, B, i);
    }
    T[cur].cnt++;
}

bool exists(int x) {
    int cur = 0;
    for (int i = LOG - 1; i >= 0; i--) {
        int B = (x >> i) & 1;
        if (!child(cur, B, i)) {
            return false;
        }
        cur = child(cur, B, i);
    }
    return true;
}

int mex() {
    int cur = 0, ret = 0;
    for (int i = LOG - 1; i >= 0; i--) {
        auto &c0 = child(cur, 0, i), &c1 = child(cur, 1, i);
        
        if (!c0) {
            return ret;
        } else if (c0 && T[c0].cnt < (1 << i)) {
            cur = c0;
        } else if (c1) {
            cur = c1;
            ret |= 1 << i;
        } else {
            ret |= 1 << i;
            return ret;
        }
    }

    return ret;
}

void debug() {
    for (int i = 0; i < T.size(); ++i) {
        auto &node = T[i];
        cout << i << ": " << node.cnt << '\n';
        for (int j = 0; j < 2; ++j) {
            cout << "  " << j << ": " << node.child[j] << '\n';
        }
    }
    cout << endl;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    for (int i = 0; i < N; ++i) {
        int x; cin >> x;
        if (!exists(x)) insert(x);
    }

    // debug();

    while (M--) {
        int x; cin >> x;
        X ^= x;
        cout << mex() << '\n';
    }

    return 0;
}