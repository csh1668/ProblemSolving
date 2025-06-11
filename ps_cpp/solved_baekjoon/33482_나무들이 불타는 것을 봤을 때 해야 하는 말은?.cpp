#include <bits/stdc++.h>

using namespace std;

struct Node {
    Node *l, *r;
    int prior, cnt;
    bool rev;

    int val, xor_sum;
    Node(int v) : l(0), r(0), prior(rand()), cnt(1), rev(0), val(v), xor_sum(v) {}
    void push() {
        if (rev) {
            swap(l, r);
            if (l) l->rev ^= 1;
            if (r) r->rev ^= 1;
            rev = 0;
        }
    }
    void pull() {
        cnt = 1; xor_sum = val;
        if (l) cnt += l->cnt, xor_sum ^= l->xor_sum;
        if (r) cnt += r->cnt, xor_sum ^= r->xor_sum;
    }
};
typedef Node* pNode;

pair<pNode, pNode> split(pNode t, int k) { // 1-based
    if (!t) return {0, 0};
    t->push();
    int lcnt = t->l ? t->l->cnt : 0;
    if (k <= lcnt) {
        auto [l, r] = split(t->l, k);
        t->l = r;
        t->pull();
        return {l, t};
    } else {
        auto [l, r] = split(t->r, k - lcnt - 1);
        t->r = l;
        t->pull();
        return {t, r};
    }
}

pNode merge(pNode left, pNode right) {
    if (!left || !right) return left ? left : right;
    if (left->prior > right->prior) {
        left->push();
        left->r = merge(left->r, right);
        left->pull();
        return left;
    } else {
        right->push();
        right->l = merge(left, right->l);
        right->pull();
        return right;
    }
}

tuple<pNode, pNode, pNode> split(pNode t, int l, int r) {
    auto [A, BC] = split(t, l - 1);
    auto [B, C] = split(BC, r - l + 1);
    return {A, B, C};
}

pNode flip(pNode t, int l, int r) {
    if (!t || l >= r) return t;
    auto [A, B, C] = split(t, l, r);
    if (B) B->rev ^= 1;
    return merge(A, merge(B, C));
}

pNode shift(pNode t, int l, int r, int k) { // right-shift
    k %= (r - l + 1);
    if (!t || l >= r || k == 0) return t;
    t = flip(t, l, r);
    int mid = l + k - 1;
    t = flip(t, l, mid);
    t = flip(t, mid + 1, r);
    return t;
}

pNode flip(pNode t) {
    if (!t) return t;
    t->rev ^= 1;
    t->push();
    return t;
}

pNode shift(pNode t, int k) {
    if (!t || k == 0) return t;
    return shift(t, 1, t->cnt, k);
}

void debug(pNode t) {
    if (!t) return;
    t->push();
    debug(t->l);
    cout << t->val << " ";
    debug(t->r);
}

// HLD
// constexpr int MAXN = 1'000'002;
constexpr int MAXN = 10;
int sz[MAXN], dep[MAXN], par[MAXN], top[MAXN], ord[MAXN];
vector<int> adj[MAXN], inp[MAXN];

void dfs_0(int u = 1) {
    static bitset<MAXN> vis;
    vis[u] = 1;
    for (int v : inp[u]) {
        if (vis[v]) continue;
        vis[v] = 1;
        adj[u].push_back(v);
        dfs_0(v);
    }
}
void dfs_1(int u = 1) {
    sz[u] = 1;
    for (auto &v : adj[u]) {
        dep[v] = dep[u] + 1;
        par[v] = u;
        dfs_1(v);
        sz[u] += sz[v];
        if (sz[v] > sz[adj[u][0]])
            swap(adj[u][0], v);
    }
}
void dfs_2(int u = 1, int _ord = 0) {
    ord[u] = ++_ord;
    for (auto &v : adj[u]) {
        top[v] = (v == adj[u][0] ? top[u] : v);
        if (v == adj[u][0]) {
            dfs_2(v, _ord);
        } else {
            dfs_2(v);
        }
    }
}

pNode T[MAXN];
pNode build(int u) {
    pNode t = new Node(u);
    for (int i = 0; i < adj[u].size(); ++i) {
        auto& v = adj[u][i];
        if (i == 0) { // heavy child
            t = merge(t, build(v));
        } else { // light child
            build(v);
        }
    }

    if (top[u] == u) {
        T[u] = t;
    }

    return t;
}

struct Info {
    pNode A, C;
    int B_size, T_pos;
};

int query(int u, int v, int k) {
    pNode r1 = 0, r2 = 0;
    queue<Info> r1_info;
    stack<Info> r2_info;

    while (top[u] != top[v]) {
        if (dep[top[u]] > dep[top[v]]) {
            auto t = T[top[u]];
            auto [A, B, C] = split(t, ord[top[u]], ord[u]);
            r1_info.push({A, C, B->cnt, top[u]});
            r1 = merge(B, r1);
            u = par[top[u]];
        } else {
            auto t = T[top[v]];
            auto [A, B, C] = split(t, ord[top[v]], ord[v]);
            r2_info.push({A, C, B->cnt, top[v]});
            r2 = merge(B, r2);
            v = par[top[v]];
        }
    }

    if (dep[u] > dep[v]) {
        auto t = T[top[u]];
        auto [A, B, C] = split(t, ord[v], ord[u]);
        r1_info.push({A, C, B->cnt, top[u]});
        r1 = merge(B, r1);
    } else {
        auto t = T[top[v]];
        auto [A, B, C] = split(t, ord[u], ord[v]);
        r2_info.push({A, C, B->cnt, top[v]});
        r2 = merge(B, r2);
    }
    r1 = flip(r1);

    auto r = merge(r1, r2);
    r = shift(r, k);
    int ret = r->xor_sum;

    // r1를 먼저 재구성
    while (!r1_info.empty()) {
        auto [A, C, B_size, T_pos] = r1_info.front();
        r1_info.pop();

        auto [X, Y] = split(r, B_size);

        X = flip(X);

        auto t = merge(A, merge(X, C));
        T[T_pos] = t;
        r = Y;
    }

    while (!r2_info.empty()) {
        auto [A, C, B_size, T_pos] = r2_info.top();
        r2_info.pop();

        auto [X, Y] = split(r, B_size);

        // X = flip(X);

        auto t = merge(A, merge(X, C));
        T[T_pos] = t;
        r = Y;
    }

    return ret;
}

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, Q; cin >> N >> Q;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        inp[u].push_back(v);
        inp[v].push_back(u);
    }
    dfs_0(1); dfs_1(1); 
    top[1] = 1;
    dfs_2(1);

    build(1);

    // test
    // for (int i = 1; i <= N; ++i) {
    //     cout << "Node " << i << ": ";
    //     cout << "Top: " << top[i] << ", ";
    //     debug(T[i]);
    //     cout << "\n";
    // }
    // cout << flush;

    while (Q--) {
        int u, v, k; cin >> u >> v >> k;
        cout << query(u, v, k) << "\n";
    }

    return 0;
}