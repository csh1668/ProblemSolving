#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 200'005;

int sz[MAXN], vis[MAXN];
char color[MAXN];
ll ans;
vector<int> G[MAXN];

struct State {
    int a, b;
    State(int a = 0, int b = 0) : a(a), b(b) {}
    State operator+(const State& rhs) const {
        return State(a + rhs.a, b + rhs.b);
    }
    State operator+(const char& c) const {
        switch (c) {
            case 'U': return State(a + 1, b + 1);
            case 'C': return State(a, b - 1);
            case 'P': return State(a - 1, b + 1);
            default: assert(0);
        }
    }
    bool operator<(const State& rhs) const {
        return a < rhs.a || (a == rhs.a && b < rhs.b);
    }
    State operator-() const { return State(-a, -b); }
};

map<State, int> mp;

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

void dfs1(int u, int p, State cur) {
    ans += mp[-cur];
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        dfs1(v, u, cur + color[v]);
    }
}

void dfs2(int u, int p, State cur) {
    mp[cur]++;
    for (auto& v : G[u]) {
        if (v == p || vis[v]) continue;
        dfs2(v, u, cur + color[v]);
    }
}

void solve(int u) {
    int cap = get_size(u, -1) / 2;
    int cen = get_centroid(u, -1, cap);
    vis[cen] = 1;

    mp.clear();
    mp[State()] = 1;

    for (auto& v : G[cen]) {
        if (vis[v]) continue;
        dfs1(v, cen, State() + color[v] + color[cen]);
        dfs2(v, cen, State() + color[v]);
    }

    for (auto& v : G[cen]) {
        if (vis[v]) continue;
        solve(v);
    }
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    cin >> (color + 1);
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    solve(1);

    cout << ans << '\n';

    return 0;
}