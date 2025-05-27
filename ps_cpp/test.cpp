#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

int main() {
    ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int H, W; cin >> H >> W;
    ll A[H][W];
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            cin >> A[i][j];
        }
    }

    auto idx = [&](int i, int j) { return i * W + j; };

    int MX = H * W + 3;
    int src = MX - 2, sink = MX - 1;

    vector<vector<int>> adj(MX);
    vector<vector<ll>> cst(MX, vector<ll>(MX, 0));
    vector<vector<ll>> cap(MX, vector<ll>(MX, 0));
    vector<vector<ll>> flw(MX, vector<ll>(MX, 0));

    auto add_edge = [&](int u, int v, ll cost, ll capacity) {
        adj[u].push_back(v);
        adj[v].push_back(u);
        cst[u][v] = cost;
        cst[v][u] = -cost;
        cap[u][v] = capacity;
    };

    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            if ((i + j) % 2 == 0) {
                add_edge(src, idx(i, j), 0, 1);
                if (i + 1 < H) {
                    add_edge(idx(i, j), idx(i + 1, j), A[i][j] + A[i + 1][j], 1);
                }
                if (j + 1 < W) {
                    add_edge(idx(i, j), idx(i, j + 1), A[i][j] + A[i][j + 1], 1);
                }
                if (i - 1 >= 0) {
                    add_edge(idx(i, j), idx(i - 1, j), A[i][j] + A[i - 1][j], 1);
                }
                if (j - 1 >= 0) {
                    add_edge(idx(i, j), idx(i, j - 1), A[i][j] + A[i][j - 1], 1);
                }
            } else {
                add_edge(idx(i, j), sink, 0, 1);
            }
        }
    }

    vector<ll> gains;
    vector<ll> dist(MX);
    vector<int> par(MX);
    vector<bool> in_queue(MX);
    while (1) {
        fill(dist.begin(), dist.end(), LLONG_MAX);
        fill(par.begin(), par.end(), -1);
        fill(in_queue.begin(), in_queue.end(), false);
        dist[src] = 0;
        queue<int> q;
        q.push(src);
        in_queue[src] = true;

        while (!q.empty()) {
            int u = q.front(); q.pop();
            in_queue[u] = false;
            for (int v : adj[u]) {
                if (cap[u][v] - flw[u][v] > 0 && dist[u] + cst[u][v] < dist[v]) {
                    dist[v] = dist[u] + cst[u][v];
                    par[v] = u;
                    if (!in_queue[v]) {
                        q.push(v);
                        in_queue[v] = true;
                    }
                }
            }
        }

        if (par[sink] == -1) break;

        int p = sink;
        ll gain = 0;
        while (p != src) {
            gain += cst[par[p]][p];
            flw[par[p]][p]++;
            flw[p][par[p]]--;
            p = par[p];
        }
        gains.push_back(gain);
    }

    ll s = 0, ans = 0;
    for (int i = 0; i < H; ++i) {
        for (int j = 0; j < W; ++j) {
            s += A[i][j];
        }
    }
    ans = s;

    for (int i = 0; i < gains.size(); ++i) {
        s -= gains[i];
        ans = max(ans, s);
    }

    cout << ans;


    return 0;
}