#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef long long lint;


int N;

struct pseg {
    
    struct node {
        int mx = 0;
        node *l=0, *r=0;
    };

    void make_child (node *n) {
        if (n->l) return;

        n->l = new node();
        n->r = new node();
    }

    node* root = new node();

    int query (node *n, int s, int e, int l = 0, int r = 505050) {
        if (e < l || r < s || !n) return 0;

        if (s <= l && r <= e) return n->mx;

        int mid = l + r >> 1;

        return max(query(n->l, s, e, l, mid),
                    query(n->r, s, e, mid+1, r));
    }

    void update (node *n, int idx, int l, int r, int value) {
        if (l == r) {
            n->mx = max(n->mx, value);
            return;
        }

        int mid = l + r >> 1;

        make_child(n);

        if (idx <= mid) update(n->l, idx, l, mid, value);
        else update(n->r, idx, mid+1, r, value);

        pull(n);
    }

    void pull (node *n) {
        make_child(n);
        n->mx = max(n->l->mx, n->r->mx);
    }
};

pseg tree[600000*4];
int mxx[600000 * 4];

void update (int n, pair<int, int> p, int l, int r, int value) {
    int idx = p.first;

    tree[n].update(tree[n].root, p.second, 0, 505050, value);

    if (l == r) {
        return;
    }

    int mid = l + r >> 1;

    if (idx <= mid) update(n << 1, p, l, mid, value);
    else update(n << 1  | 1, p, mid+1, r, value);
}

int query (int n, pair<int, int> p, int l, int r) {
    if (r < 0 || p.first < l) return 0;
    if (0 <= l && r <= p.first) {
        return tree[n].query(tree[n].root, 0, p.second, 0, 505050);
    }

    int mid = l + r >> 1;
    
    return max(query(n << 1, p, l, mid), query(n << 1 | 1, p, mid+1, r));
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    cin >> N;

    // vector<pair<int, int>> points;
    vector<tuple<int, int, int, int>> points;

    for (int i=0;i<N;i++) {
        int x, y; cin >> x >> y;
        points.emplace_back(x-y, y+x, x, y);
    }


    vector<ll> x_tmp, y_tmp;
    for (int i = 0; i < N; ++i) {
        auto &[a, b, c, d] = points[i];
        x_tmp.push_back(a);
        y_tmp.push_back(b);
    }
    sort(x_tmp.begin(), x_tmp.end());
    sort(y_tmp.begin(), y_tmp.end());
    for (int i = 0; i < N; ++i) {
        auto &[a, b, c, d] = points[i];
        a = lower_bound(x_tmp.begin(), x_tmp.end(), a) - x_tmp.begin();
        b = lower_bound(y_tmp.begin(), y_tmp.end(), b) - y_tmp.begin();

        a++;b++;
    }

    sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        auto& [ax, ay, gg , ggg] = a;
        auto& [bx, by, ggggg, gggggg] = b;
        return (ax + ay) < (bx + by);
    });

    int ans = 0;

    for (auto [x, y, a, b] : points) {
        int res = query(1, {x-1, y-1}, 0, 505050);
        // cout << x << ' ' << y << ' ' << res <<  '\n';

        update(1, {x, y}, 0, 505050, res+1);
        ans = max(ans, res + 1);
    }

    cout << ans;
    return 0;
}