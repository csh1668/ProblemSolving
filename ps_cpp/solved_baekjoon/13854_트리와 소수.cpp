#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx2")

#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 50'005, MAXP = 50'000;

int sz[MAXN], cnt[MAXN], ans = 0;
bool vis[MAXN];
vector<int> G[MAXN];

constexpr auto is_prime([]() {
    array<bool, MAXP> is_prime;
    is_prime.fill(true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i < MAXP; ++i) {
        if (is_prime[i]) {
            for (int j = i + i; j < MAXP; j += i) {
                is_prime[j] = false;
            }
        }
    }
    return is_prime;
}());

constexpr auto P([]() {
    constexpr auto prime_count = []() {
        int cnt = 0;
        for (int i = 2; i < MAXP; ++i) {
            if (is_prime[i]) {
                ++cnt;
            }
        }
        return cnt;
    }();

    array<int, prime_count> primes; int cnt = 0;
    for (int i = 2; i < MAXP; ++i) {
        if (is_prime[i]) {
            // primes.push_back(i);
            primes[cnt++] = i;
        }
    }
    return primes;
}());

template <typename T>
struct my_stack {
    vector<T> v;
    my_stack(int cap) : v(cap) {}
    void push(const T &x) { v.push_back(x); }
    void push(const T &&x) { v.emplace_back(x); }
    T pop() { T x = v.back(); v.pop_back(); return x; }
    void clear() { v.clear(); }
    size_t size() const { return v.size(); }
};
// my_stack<tuple<int, int, int>> S(MAXN);

int get_size(int u, int p = -1) {
    sz[u] = 1;
    for (const auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int cap, int p = -1) {
    for (const auto &v : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, cap, u);
    }
    return u;
}

my_stack<tuple<int, int, int>> T(MAXN);

void calc(int u, int p, int d) {
    T.clear();
    T.push({u, p, d});
    while (T.size()) {
        auto [u, p, d] = T.pop();
        if (d > P.back()) continue;
        auto it = lower_bound(P.begin(), P.end(), d);
        while (it != P.end() && cnt[*it - d]) {
            ans += cnt[*it - d];
            it++;
        }

        for (const auto &v : G[u]) {
            if (v == p || vis[v]) continue;
            T.push({v, u, d + 1});
        }
    }
}

int max_depths = 0;
void update(int u, int p, int d) {
    T.clear();
    T.push({u, p, d});
    while (T.size()) {
        auto [u, p, d] = T.pop();
        if (d > P.back()) continue;
        ++cnt[d];
        max_depths = max(max_depths, d);
        for (const auto &v : G[u]) {
            if (v == p || vis[v]) continue;
            T.push({v, u, d + 1});
        }
    }
}

void solve(int u) {
    int cap = get_size(u) / 2;
    int cen = get_centroid(u, cap);
    vis[cen] = 1;

    // for (auto &i : tmp) cnt[i] = 0;
    // tmp.clear();
    for (int i = 1; i <= max_depths; ++i) cnt[i] = 0;
    cnt[0] = 1;
    max_depths = 0;

    for (const auto &v : G[cen]) {
        if (vis[v]) continue;
        calc(v, cen, 1);
        update(v, cen, 1);
    }

    for (const auto &v : G[cen]) {
        if (vis[v]) continue;
        solve(v);
    }
}



int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    // freopen("t/1.in", "r", stdin);

    int N; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        G[u].push_back(v);
        G[v].push_back(u);
    }

    solve(1);

    // cout << ans << '\n';
    double prob = 1.0 * ans / ((long long int)N * (N - 1) / 2);
    cout << fixed << setprecision(10) << prob << '\n';

    return 0;
}