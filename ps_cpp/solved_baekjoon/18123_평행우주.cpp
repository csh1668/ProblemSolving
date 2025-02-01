#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

int sz[32];
vector<int> G[32];

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto &v : G[u]) {
        if (v == p) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto &v : G[u]) {
        if (v == p) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

string hashing(int u, int p) {
    string ret; vector<string> in;
    for (auto &v : G[u]) {
        if (v == p) continue;
        in.push_back('1' + hashing(v, u) + '0');
    }
    sort(in.begin(), in.end());
    for (auto &s : in) ret += s;
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; cin >> N;
    unordered_set<string> S;
    while (N--) {
        int M; cin >> M;
        for (int i = 0; i < 32; ++i) G[i].clear();
        for (int i = 0; i < M - 1; ++i) {
            int u, v; cin >> u >> v;
            G[u].push_back(v);
            G[v].push_back(u);
        }

        int cap = get_size(0, -1) / 2;
        int cen = get_centroid(0, -1, cap);
        int another_cen = -1;

        if (M % 2 == 0) {
            for (auto &v : G[cen]) {
                if (sz[v] == cap) {
                    another_cen = v;
                    break;
                }
            }
        }

        if (another_cen != -1) {
            const int w = 30;
            G[cen].erase(find(G[cen].begin(), G[cen].end(), another_cen));
            G[another_cen].erase(find(G[another_cen].begin(), G[another_cen].end(), cen));
            G[w].push_back(cen);
            G[cen].push_back(w);
            G[w].push_back(another_cen);
            G[another_cen].push_back(w);
            cen = w;
        }

        string h = hashing(cen, -1);
        assert(h.size() < 64);
        // S.insert(stoll(h, 0, 2));
        // S.insert(format("{0}.{1},{2}", h, (another_cen == -1 ? 0 : 1), M));
        S.insert(h + "." + to_string(M));
    }

    cout << S.size() << '\n';

    return 0;
}