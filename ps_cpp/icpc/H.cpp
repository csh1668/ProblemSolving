#include <algorithm>
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef long long lint;


int N;

struct pseg {
    
    struct node {
        int mx = 0;
        int l = 0, r = 0;
    };

    void make_child (int n) {
        if (nodes[n].l) return;

        nodes[n].l = get_node();
        nodes[n].r = get_node();
    }

    int get_node () {
        int g = nodes.size();
        nodes.emplace_back();;;;
        return g;
    }

    pseg () : nodes(2) {}

    vector<node> nodes;

    int query (int n, int s, int e, int l = 0, int r = 505050) {
        if (e < l || r < s || !n) return 0;

        if (s <= l && r <= e) return nodes[n].mx;

        int mid = l + r >> 1;

        return max(query(nodes[n].l, s, e, l, mid),
                    query(nodes[n].r, s, e, mid+1, r));
    }

    void update (int n, int idx, int l, int r, int value) {
        if (l == r) {
            nodes[n].mx = max(nodes[n].mx, value);
            return;
        }

        int mid = l + r >> 1;

        make_child(n);

        if (idx <= mid) update(nodes[n].l, idx, l, mid, value);
        else update(nodes[n].r, idx, mid+1, r, value);

        pull(n);
    }

    void pull (int n) {
        make_child(n);
        nodes[n].mx = max(nodes[n].mx, nodes[nodes[n].l)
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

    vector<pair<int, int>> points;

    for (int i=0;i<N;i++) {
        int x, y; cin >> x >> y;
        points.emplace_back(x-y, y+x);
    }


    vector<ll> x_tmp, y_tmp;
    for (int i = 0; i < N; ++i) {
        auto &[a, b] = points[i];
        x_tmp.push_back(a);
        y_tmp.push_back(b);
    }
    sort(x_tmp.begin(), x_tmp.end());
    sort(y_tmp.begin(), y_tmp.end());
    for (int i = 0; i < N; ++i) {
        auto &[a, b] = points[i];
        a = lower_bound(x_tmp.begin(), x_tmp.end(), a) - x_tmp.begin();
        b = lower_bound(y_tmp.begin(), y_tmp.end(), b) - y_tmp.begin();

        a++;b++;
    }

    sort(points.begin(), points.end(), [](const auto &a, const auto &b) {
        auto& [ax, ay] = a;
        auto& [bx, by] = b;
        return (ax + ay) < (bx + by);
    });

    int ans = 0;

    for (auto [x, y] : points) {
        int res = query(1, {x-1, y-1}, 0, 505050);
        // cout << x << ' ' << y << ' ' << res <<  '\n';

        update(1, {x, y}, 0, 505050, res+1);
        ans = max(ans, res + 1);
    }

    cout << ans;
    return 0;
}