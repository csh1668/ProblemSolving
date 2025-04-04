#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'005, LOG = 19;

vector<pair<int, short>> G[MAXN];
bitset<MAXN> color;

// 센트로이드 트리
int sz[MAXN], cen_par[MAXN];
bitset<MAXN> vis;

int get_size(int u, int p) {
    sz[u] = 1;
    for (auto &[v, _] : G[u]) {
        if (v == p || vis[v]) continue;
        sz[u] += get_size(v, u);
    }
    return sz[u];
}

int get_centroid(int u, int p, int cap) {
    for (auto &[v, _] : G[u]) {
        if (v == p || vis[v]) continue;
        if (sz[v] > cap) return get_centroid(v, u, cap);
    }
    return u;
}

void decompose(int u = 1, int p = 0) {
    int cen = get_centroid(u, -1, get_size(u, -1) / 2);
    vis[cen] = 1;
    cen_par[cen] = p;
    for (auto &[v, _] : G[cen]) {
        if (vis[v]) continue;
        decompose(v, cen);
    }
}
// ==========

// LCA
int dep[MAXN], par[MAXN][LOG], dist[MAXN][LOG];

void build_table(int u = 1, int p = 0, int d = 0) {
    dep[u] = dep[p] + 1;
    par[u][0] = p;
    dist[u][0] = d;
    for (int i = 1; i < LOG; ++i) {
        par[u][i] = par[par[u][i - 1]][i - 1];
        dist[u][i] = dist[u][i - 1] + dist[par[u][i - 1]][i - 1];
    }
    for (auto &[v, w] : G[u]) {
        if (v == p) continue;
        build_table(v, u, d + w);
    }
}

int lca(int u, int v) {
    if (dep[u] < dep[v]) swap(u, v);
    for (int i = LOG - 1; i >= 0; --i) {
        if (dep[u] - dep[v] >= (1 << i)) u = par[u][i];
    }
    if (u == v) return u;
    for (int i = LOG - 1; i >= 0; --i) {
        if (par[u][i] != par[v][i]) {
            u = par[u][i];
            v = par[v][i];
        }
    }
    return par[u][0];
}

int get_dist(int u, int v) {
    return dist[u][0] + dist[v][0] - 2 * dist[lca(u, v)][0];
}
// ==========

// subtree[i][j]: 센트로이드 트리에서 i의 자식인 j를 루트로하는 서브트리에서의 정점과 i사이의 거리 모음
unordered_map<int, multiset<int>> subtree[MAXN];
// subtree_ans[i]: subtree[i][j]들 중 최댓값
multiset<int> subtree_ans[MAXN];
// ans: 각 센트로이드에서의 정답 모음
multiset<int> ans;
// prev_ans: 각 센트로이드에서의 이전 정답
// cnt: 흰색 정점의 개수
int prev_ans[MAXN], cnt;

void update_ans(int u) {
    if (prev_ans[u] != INT_MIN) {
        ans.erase(ans.find(prev_ans[u]));
    }

    if (subtree_ans[u].size() >= 2) {
        int mx = *subtree_ans[u].rbegin();
        int smx = *next(subtree_ans[u].rbegin());
        ans.insert(mx + smx);
        prev_ans[u] = mx + smx;
    } else {
        prev_ans[u] = INT_MIN;
    }
}

void update(int u) {
    color[u] = !color[u];
    if (color[u]) cnt++;
    else cnt--;

    int v = u, p = u;
    while (1) {
        int d = get_dist(u, v);
        auto &s = subtree[p][v];
        auto &s_ans = subtree_ans[v];
        if (color[u]) {
            if (s.size() && *s.rbegin() < d) {
                // d가 s의 새로운 최댓값이 된다
                s_ans.erase(s_ans.find(*s.rbegin())); // 기존 최댓값 제거
                s.insert(d);
                s_ans.insert(d);
                update_ans(v);
            } else if (s.size() == 0) {
                // d가 s의 최댓값이 된다
                s.insert(d);
                s_ans.insert(d);
                update_ans(v);
            } else {
                // d는 별 볼일 없는 값이지만 나중을 위해 추가한다
                s.insert(d);
            }
        } else {
            int mx = *s.rbegin();
            s.erase(s.find(d));
            if (s.size() == 0 || *s.rbegin() < d) {
                s_ans.erase(s_ans.find(d));

                if (s.size()) s_ans.insert(*s.rbegin());
                update_ans(v);
            }
        }

        if (cen_par[v] == 0) break;
        p = v;
        v = cen_par[p];
    }
}



int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    fill(prev_ans, prev_ans + MAXN, INT_MIN);

    int N, M; cin >> N;
    for (int i = 0; i < N - 1; ++i) {
        int u, v, w; cin >> u >> v >> w;
        G[u].emplace_back(v, w);
        G[v].emplace_back(u, w);
    }

    decompose();
    build_table();

    for (int i = 1; i <= N; ++i) {
        update(i);
    }

    cin >> M;
    while (M--) {
        int op; cin >> op;
        if (op == 1) {
            int i; cin >> i;
            update(i);
        } else {
            if (cnt == 0) {
                cout << -1 << '\n';
            } else if (cnt == 1) {
                cout << 0 << '\n';
            } else {
                cout << *ans.rbegin() << '\n';
            }
        }
    }

    return 0;
}