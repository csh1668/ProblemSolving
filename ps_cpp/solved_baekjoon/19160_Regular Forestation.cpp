#include <bits/stdc++.h>

using namespace std;

int sz[4002];
set<int> G[4002];

int get_size(int u, int p = -1) {
    sz[u] = 1;
    for (auto &v : G[u]) {
        if (v == p) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p = -1, int cap = -1) {
    for (auto &v : G[u]) {
        if (v == p) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

string hashing(int u, int p = -1) {
    string ret; vector<string> in;
    for (auto &v : G[u]) {
        if (v == p) continue;
        in.push_back("1" + hashing(v, u) + "0");
    }
    sort(in.begin(), in.end());
    for (auto &s : in) ret += s;
    return ret;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    string file = "39";
    freopen(("secret/forest_1_" + file + ".in").c_str(), "r", stdin);
    ifstream answer(("secret/forest_1_" + file + ".ans"));
    #endif

    const int w = 4001;

    int N; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v; cin >> u >> v;
        // G[u].push_back(v);
        // G[v].push_back(u);
        G[u].insert(v); G[v].insert(u);
    }

    int mx_iso = -1;
    for (int u = 1; u <= N; ++u) {
        if (G[u].size() < 2) continue;

        int prev_sz(-1);
        string prev_tree("");
        bool all_iso(true);

        int test = 0;

        for (auto& v : G[u]) {
            int cap = get_size(v, u);
            if (prev_sz != -1 && prev_sz != sz[v]) {
                all_iso = false;
                break;
            }
            prev_sz = sz[v];
        }

        if (!all_iso) continue;

        for (auto &v : G[u]) {
            if (!G[w].empty()) {
                // int x = G[w].front(), y = G[w].back();
                assert(G[w].size() == 2);
                int x = *G[w].begin(), y = *G[w].rbegin();
                // G[x].erase(find(G[x].begin(), G[x].end(), w));
                // G[y].erase(find(G[y].begin(), G[y].end(), w));
                // G[x].push_back(y);
                // G[y].push_back(x);
                G[x].erase(w); G[y].erase(w);
                G[x].insert(y); G[y].insert(x);
                G[w].clear();
            }

            int& cap = sz[v];
            test++;

            int cen = get_centroid(v, u, cap / 2);
            int anothor_cen = -1;

            if (cap % 2 == 0) {
                for (auto &w : G[cen]) {
                    if (w != u && sz[w] == cap / 2) {
                        anothor_cen = w;
                        break;
                    }
                }
            }

            if (anothor_cen != -1) {
                // G[cen].erase(find(G[cen].begin(), G[cen].end(), anothor_cen));
                // G[anothor_cen].erase(find(G[anothor_cen].begin(), G[anothor_cen].end(), cen));
                // G[cen].push_back(w);
                // G[w].push_back(cen);
                // G[anothor_cen].push_back(w);
                // G[w].push_back(anothor_cen);
                G[cen].erase(anothor_cen); G[anothor_cen].erase(cen);
                G[cen].insert(anothor_cen); G[anothor_cen].insert(cen);
                G[w].insert(cen); G[w].insert(anothor_cen);
                cen = w;
            }

            G[v].erase(u);
            string tree = hashing(cen);
            G[v].insert(u);
            if (prev_tree != "" && prev_tree != tree) {
                all_iso = false;
                break;
            }
            prev_tree = tree;
        }

        // cout << "u: " << u << " test: " << test << endl;

        if (all_iso) {
            mx_iso = max(mx_iso, static_cast<int>(G[u].size()));
        }
    }

    cout << mx_iso << '\n';

    #ifndef ONLINE_JUDGE
    int ans; answer >> ans;
    cout << "Answer: " << ans << '\n';
    #endif

    return 0;
}