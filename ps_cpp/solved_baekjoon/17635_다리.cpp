#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 50'001, MAXM = 100'001, SQ = 300;
template<typename T> using vstack = stack<T, vector<T>>;

bool change[MAXM];

struct Bridge {
    int u, v, d;
} bridges[MAXM];

// Union-Find with rollback
struct UnionFind {
    struct Rollback { int u, v, s; };
    int par[MAXN], sz[MAXN];
    vstack<Rollback> his;
    void init(int n) {
        iota(par, par + n + 1, 0);
        fill(sz, sz + n + 1, 1);
        his = vstack<Rollback>();
    }
    int find(int x, bool rollbackable = false) {
        if (x == par[x]) return x;
        if (rollbackable) return find(par[x], true);
        return par[x] = find(par[x]);
    }
    bool merge(int u, int v, bool rollbackable = false) {
        u = find(u, rollbackable), v = find(v, rollbackable);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        if (rollbackable) his.push({u, v, sz[u]});
        par[v] = u;
        sz[u] += sz[v];
        return true;
    }
    void rollback() {
        auto [u, v, s] = his.top(); his.pop();
        par[v] = v;
        sz[u] = s;
    }
    void rollback_all() { while (!his.empty()) rollback(); }
} uf;

struct Operation {
    enum { print, add } op;
    int u, w, i;
    // 무게가 w인 자동차로 섬 s에서 출발해서 도착할 수 있는 섬의 수를 구하는 연산
    static Operation Print(int u, int w, int i) {
        return {print, u, w, i};
    }
    // 다리를 건설하는 연산
    static Operation Add(int u, int w) {
        return {add, u, w, -1};
    }

    bool operator>(const Operation& rhs) const {
        if (w == rhs.w) return op > rhs.op;
        return w > rhs.w;
    }
};

struct Query {
    int op, a, b;
} queries[MAXM];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    for (int i = 1; i <= M; ++i) cin >> bridges[i].u >> bridges[i].v >> bridges[i].d;
    int Q; cin >> Q;
    for (int i = 0; i < Q; ++i) cin >> queries[i].op >> queries[i].a >> queries[i].b;

    // 쿼리를 sqrtQ개씩 묶어서 처리할거임
    vector<Operation> udt;
    vector<pair<Query, int>> modify; // 1번 쿼리 모음
    vector<pair<int, int>> ans; // 2번 쿼리의 답
    unordered_map<int, int> tmp; // 무게 제한이 변화하는 다리의 최초 무게 제한
    for (int l = 0; l < Q;) {
        int r = min(Q, l + SQ);
        udt.clear(); modify.clear(); ans.clear(); tmp.clear();
        ans.reserve(r - l);
        uf.init(N);
        fill(change, change + M + 1, false);
        // 쿼리 중 무게 제한이 바뀌는 다리 체크
        for (int i = l; i < r; ++i) {
            auto& [op, a, b] = queries[i];
            if (op == 1) change[a] = true, modify.push_back({queries[i], i}), tmp[a] = bridges[a].d;
            else udt.push_back(Operation::Print(a, b, i));
        }
        // 무게 제한이 바뀌지 않는 다리 추가
        for (int i = 1; i <= M; ++i) {
            if (!change[i]) 
                udt.push_back(Operation::Add(i, bridges[i].d));
        }
        sort(udt.begin(), udt.end(), greater<>());
        // 연산 수행
        for (auto& [op, u, w, i] : udt) {
            if (op == Operation::add) {
                uf.merge(bridges[u].u, bridges[u].v);
            } else {
                auto mp(tmp); // i번 다리의 최종 무게
                for (auto& [q, idx] : modify) {
                    if (idx >= i) break;
                    mp[q.a] = q.b;
                }
                for (auto& [b, d] : mp) {
                    if (d >= w)
                        uf.merge(bridges[b].u, bridges[b].v, true);
                }
                int res = uf.sz[uf.find(u, true)];
                ans.push_back({res, i});
                uf.rollback_all();
            }
        }
        sort(ans.begin(), ans.end(), [](const pair<int, int>& lhs, const pair<int, int>& rhs) { return lhs.second < rhs.second; });
        for (auto& [res, _] : ans) cout << res << '\n';

        // 1번 쿼리 모두 적용
        for (auto& [q, _] : modify) {
            auto& [op, a, b] = q;
            bridges[a].d = b;
        }

        l = r;
    }


    return 0;
}