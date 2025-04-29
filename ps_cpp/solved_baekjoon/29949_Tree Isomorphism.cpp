#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long ull;
constexpr int MAXN = 100'003;

int A_sz[MAXN], B_sz[MAXN];
vector<int> A[MAXN], B[MAXN];
ull A_map[MAXN], B_map[MAXN];

int get_size(int u, int p, vector<int> *G, int *sz) {
    sz[u] = 1;
    for (auto &v : G[u]) {
        if (v == p) continue;
        sz[u] += get_size(v, u, G, sz);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap, vector<int> *G, int *sz) {
    for (auto &v : G[u]) {
        if (v == p) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap, G, sz);
    }
    return u;
}

template<typename T> ull combine_hashes(const T& h) {
    ull res = 14695981039346656037ULL;
    for (const auto& x : h) {
        res ^= x;
        res *= 1099511628211ULL;
    }
    return res + 42;
}

ull hashing(int u, int p, vector<int> *G, int *sz, ull* h) {
    vector<ull> in;
    for (auto &v : G[u]) {
        if (v == p) continue;
        in.emplace_back(hashing(v, u, G, sz, h));
    }
    sort(in.begin(), in.end());
    in.emplace_back(sz[u]);
    return h[u] = combine_hashes(in);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
    
    int N; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        A[u].push_back(v);
        A[v].push_back(u);
    }
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        B[u].push_back(v);
        B[v].push_back(u);
    }

    auto process = [&](vector<int> *G, int *sz, int& root, ull* h) {
        int cap = get_size(0, -1, G, sz) / 2;
        int cen = get_centroid(0, -1, cap, G, sz);
        int another_cen = -1;
        if (N % 2 == 0) {
            for (auto &v : G[cen]) {
                if (sz[v] == cap) {
                    another_cen = v;
                    break;
                }
            }
        }

        if (another_cen != -1) {
            const int w = N;
            G[cen].erase(find(G[cen].begin(), G[cen].end(), another_cen));
            G[another_cen].erase(find(G[another_cen].begin(), G[another_cen].end(), cen));
            G[cen].push_back(w);
            G[w].push_back(cen);
            G[another_cen].push_back(w);
            G[w].push_back(another_cen);
            cen = w;
        }

        root = cen;
        get_size(cen, -1, G, sz);
        return hashing(cen, -1, G, sz, h);
    };

    int A_root, B_root;
    ull A_hash = process(A, A_sz, A_root, A_map), B_hash = process(B, B_sz, B_root, B_map);
    
    if (A_hash != B_hash) {
        cout << "EI\n";
        return 0;
    }
    cout << "JAH\n";

    vector<int> ans(N + 1);
    auto dfs = [&](int a, int pa, int b, int pb, auto &&dfs) -> void {
        ans[a] = b;
        vector<pair<ull, int>> in1, in2;
        for (auto &v : A[a]) {
            if (v == pa) continue;
            in1.emplace_back(A_map[v], v);
        }
        for (auto &v : B[b]) {
            if (v == pb) continue;
            in2.emplace_back(B_map[v], v);
        }

        assert(in1.size() == in2.size());
        sort(in1.begin(), in1.end());
        sort(in2.begin(), in2.end());
        for (int i = 0; i < in1.size(); ++i) {
            dfs(in1[i].second, a, in2[i].second, b, dfs);
        }
    };

    dfs(A_root, -1, B_root, -1, dfs);
    for (int i = 0; i < N; ++i) {
        cout << ans[i] << '\n';
    }

    return 0;
}