#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

struct Trie {
    struct node {
        node* chr[2]; int cnt;

        ~node() {
            for (int i = 0; i < 2; ++i) if (chr[i]) delete chr[i];
        }
    };
    node* root;
    Trie() {
        root = new node();
    }
    void insert(int x) {
        node* cur = root;
        for (int i = 29; i >= 0; --i) {
            int bit = (x >> i) & 1;
            if (!cur->chr[bit]) cur->chr[bit] = new node();
            cur = cur->chr[bit];
            cur->cnt++;
        }
    }
    int find_min(int x) {
        int ret = 0; node* cur = root;
        for (int i = 29; i >= 0; --i) {
            int bit = (x >> i) & 1;
            node *nxt = cur->chr[bit], *other = cur->chr[!bit];
            if (nxt && nxt->cnt) cur = nxt;
            else if (other && other->cnt) cur = other, ret |= 1 << i;
            else return INT_MAX;
        }
        return ret;
    }

    ~Trie() {
        delete root;
    }
};

int N, arr[200000];

ll mst(int s, int e, int depths = 29) {
    if (s >= e || depths < 0) return 0;
    if (arr[s] & (1 << depths) || ~arr[e] & (1 << depths)) return mst(s, e, depths - 1);
    int m = e;
    while (m >= s && (arr[m] & (1 << depths))) --m;
    m++;
    Trie t;
    for (int i = m; i <= e; ++i) t.insert(arr[i]);
    int x = INT_MAX;
    for (int i = s; i < m; ++i) x = min(x, t.find_min(arr[i]));
    if (x == INT_MAX) x = 0;
    return x + mst(s, m - 1, depths - 1) + mst(m, e, depths - 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N;
    for (int i = 0; i < N; ++i) cin >> arr[i];
    sort(arr, arr + N);

    cout << mst(0, N - 1) << '\n';

    return 0;
}