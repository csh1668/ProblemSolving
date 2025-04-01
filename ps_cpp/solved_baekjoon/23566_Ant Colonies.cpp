#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'002, SIZE = 1 << (33 - __builtin_clz(MAXN));
// constexpr int MAXN = 100, SIZE = 1 << (33 - __builtin_clz(MAXN));

struct Node {
    int left, right, mn, cnt;
    Node(bool on = false) : left(on ? 0 : -1), right(on ? 0 : -1), mn(INT_MAX), cnt(1) {}
    static Node zero() { Node ret = Node(); ret.cnt = 0; return ret; }
    Node operator+(const Node& o) const {
        Node ret;
        ret.cnt = cnt + o.cnt;
        ret.left = (left == -1) ? ((o.left == -1) ? -1 : o.left + cnt) : left;
        ret.right = (o.right == -1) ? ((right == -1) ? -1 : right) : o.right + cnt;
        ret.mn = min({mn, o.mn, (right == -1 || o.left == -1) ? INT_MAX : (o.left + cnt) - right});
        return ret;
    }
} T[SIZE];

Node& init(int l, int r, int i = 1) {
    if (l == r) return T[i] = Node();
    int m = l + r >> 1;
    return T[i] = init(l, m, i << 1) + init(m + 1, r, i << 1 | 1);
}

void _update(int l, int r, int pos, bool on, int i = 1) {
    if (l == r) {
        T[i] = Node(on);
        return;
    }
    int m = l + r >> 1;
    if (pos <= m) {
        _update(l, m, pos, on, i << 1);
    } else {
        _update(m + 1, r, pos, on, i << 1 | 1);
    }
    T[i] = T[i << 1] + T[i << 1 | 1];
}

Node _query(int l, int r, int ql, int qr, int i = 1) {
    if (qr < l || r < ql) return Node::zero();
    if (ql <= l && r <= qr) 
        return T[i];
    int m = l + r >> 1;
    return _query(l, m, ql, qr, i << 1) + _query(m + 1, r, ql, qr, i << 1 | 1);
}

int sz[MAXN], dep[MAXN], par[MAXN], top[MAXN], in[MAXN];
vector<int> adj[MAXN], inp[MAXN];

bitset<MAXN> vis;
void dfs_0(int u = 1) {
    vis[u] = 1;
    for (auto &v : inp[u]) {
        if (vis[v]) continue;
        vis[v] = 1;
        adj[u].push_back(v);
        dfs_0(v);
    }
}

void dfs_1(int u = 1) {
    sz[u] = 1;
    for (auto &v : adj[u]) {
        dep[v] = dep[u] + 1; par[v] = u;
        dfs_1(v); sz[u] += sz[v];
        if (sz[v] > sz[adj[u][0]]) swap(v, adj[u][0]);
    }
}

int _order = 0;
void dfs_2(int u = 1) {
    in[u] = ++_order;
    for (auto &v : adj[u]) {
        top[v] = (v == adj[u][0] ? top[u] : v);
        dfs_2(v);
    }
}

void update(int u, bool on) {
    _update(1, MAXN, in[u], on);
}

int query(int u, int v) {
    Node r1 = Node::zero(), r2 = Node::zero();
    while (top[u] ^ top[v]) {
        if (dep[top[u]] > dep[top[v]]) {
            r1 = _query(1, MAXN, in[top[u]], in[u]) + r1;
            u = par[top[u]];
        } else {
            r2 = _query(1, MAXN, in[top[v]], in[v]) + r2;
            v = par[top[v]];
        }
    }

    if (dep[u] > dep[v]) r1 = _query(1, MAXN, in[v], in[u]) + r1;
    else r2 = _query(1, MAXN, in[u], in[v]) + r2;
    swap(r1.left, r1.right);
    if (r1.left != -1)
        r1.left = r1.cnt - r1.left - 1;
    if (r1.right != -1)
        r1.right = r1.cnt - r1.right - 1;
    auto ret = r1 + r2;
    return ret.mn;
}

struct Event {
    bool op; // 0: update, 1: query
    int a, b; // 0: turn on a-th node if b == 1, else turn off. 1: query(a, b)
    int idx;
};

vector<Event> E[MAXN]; // 색깔별 이벤트
int ans[MAXN], color[MAXN];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    string file = "data/secret/antcolonies001";
    freopen((file + ".in").c_str(), "r", stdin);
    ifstream answer((file + ".ans").c_str());
    #endif

    int N, Q; cin >> N >> Q;
    init(1, MAXN);
    fill(ans, ans + MAXN, -2);

    for (int u = 1; u <= N; ++u) {
        int c; cin >> c;
        E[c].push_back({0, u, 1, -1});
        color[u] = c;
    }

    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        inp[u].push_back(v);
        inp[v].push_back(u);
    }

    dfs_0(); dfs_1(); dfs_2();
    init(1, MAXN);

    for (int i = 0; i < Q; ++i) {
        char op; cin >> op;
        if (op == 'U') {
            int u, c; cin >> u >> c;
            E[color[u]].push_back({0, u, 0, i});
            E[c].push_back({0, u, 1, i});
            color[u] = c;
        } else {
            int u, v, c; cin >> u >> v >> c;
            E[c].push_back({1, u, v, i});
        }
    }

    set<int> S;
    for (int c = 1; c <= N; ++c) {
        S.clear();
        for (auto &e : E[c]) {
            if (e.op == 0) { // update
                update(e.a, e.b);
                if (e.b) S.insert(e.a);
                else S.erase(e.a);
            } else { // query
                ans[e.idx] = query(e.a, e.b);
            }
        }
        for (auto &u : S) update(u, 0);
    }

    #ifdef ONLINE_JUDGE
    for (int i = 0; i < Q; ++i) {
        if (ans[i] != -2) cout << (ans[i] != INT_MAX ? ans[i] : -1) << '\n';
    }
    #else
    for (int i = 0; i < Q; ++i) {
        if (ans[i] != -2) {
            int right_ans; answer >> right_ans;
            ans[i] = (ans[i] != INT_MAX ? ans[i] : -1);
            if (ans[i] != right_ans) {
                cout << "Wrong Answer on query " << i << '\n';
                cout << "Expected: " << right_ans << '\n';
                cout << "Received: " << ans[i] << '\n';
                return 0;
            }
        }
    }
    #endif

    return 0;
}