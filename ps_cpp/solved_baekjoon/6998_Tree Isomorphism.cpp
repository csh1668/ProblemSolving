#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int T; cin >> T;
    while (T--) {
        vector<vector<int>> G1, G2;
        auto make_graph = [&](vector<vector<int>> &G) {
            stack<char> S;
            char c; cin >> c;
            if (c == '#') return;
            int u = 0;
            S.push(u); G.push_back({}); u++;
            while (!S.empty()) {
                cin >> c;
                if (c == '#') {
                    S.pop();
                } else {
                    int v = S.top();
                    S.push(u); G.push_back({});
                    G[u].push_back(v);
                    G[v].push_back(u);
                    u++;
                }
            }
        };

        make_graph(G1); make_graph(G2);

        auto hashing = [&](int u, int p, vector<vector<int>> &G, auto& hashing) -> string {
            string ret; vector<string> in;
            for (auto &v : G[u]) {
                if (v == p) continue;
                in.push_back("1" + hashing(v, u, G, hashing) + "0");
            }
            sort(in.begin(), in.end());
            for (auto &s : in) ret += s;
            return ret;
        };

        bool isomorphic = (hashing(0, -1, G1, hashing) + to_string(G1.size())) == (hashing(0, -1, G2, hashing) + to_string(G2.size()));
        if (isomorphic) cout << "The two trees are isomorphic.\n";
        else cout << "The two trees are not isomorphic.\n";
    }

    return 0;
}