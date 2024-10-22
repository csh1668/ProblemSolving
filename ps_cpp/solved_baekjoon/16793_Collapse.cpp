#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
// #pragma GCC target("avx2")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>

using namespace std;
using namespace __gnu_pbds;
constexpr int MAX = 100'005, SQ = 400;
template<typename T> using vstack = stack<T, vector<T>>;

// PBDS는 신이고 나는 무적이다...
template<typename T, typename U> using hashmap = gp_hash_table<T, U, std::hash<T>>;
template<typename T> using hashset = gp_hash_table<T, null_type, hash<T>, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;

struct UnionFind {
    struct Rollback { int u, v, s; }; vstack<Rollback> his;
    int par[MAX], sz[MAX], comp;
    inline void init(int n) {
        iota(par, par + n + 1, 0);
        fill(sz, sz + n + 1, 1);
        comp = n;
        his = vstack<Rollback>();
    } 
    int find(int x, bool rollbackable = false) {
        if (x == par[x]) return x;
        if (rollbackable) return find(par[x], true);
        return par[x] = find(par[x]);
    }
    inline bool merge(int u, int v, bool rollbackable = false) {
        u = find(u, rollbackable), v = find(v, rollbackable);
        if (u == v) return false;
        if (sz[u] < sz[v]) swap(u, v);
        if (rollbackable) his.push({u, v, sz[u]});
        par[v] = u;
        sz[u] += sz[v];
        --comp;
        return true;
    }
    inline void rollback() {
        auto [u, v, s] = his.top(); his.pop();
        par[v] = v;
        sz[u] = s;
        ++comp;
    }
    inline void rollback_all() { while (!his.empty()) rollback(); }
} uf1, uf2;

struct Plan { int op, a, b; } plans[MAX];
struct Query { int w, p, order; } queries[MAX];
struct Modify {
    int a, b, op;
    Modify() : a(0), b(0), op(0){}
    Modify(int a, int b, int op) : a(a), b(b), op(op) {}
    inline bool operator<(const Modify& o) const {
        if (a != o.a) return a < o.a;
        if (b != o.b) return b < o.b;
        return op < o.op;
    }
    inline bool operator==(const Modify& o) const {
        return a == o.a && b == o.b && op == o.op;
    }
};

struct Operation {
    enum { QUERY, CONSTRUCT, DESTROY } op;
    int u, v, idx;
    inline static Operation Query(int w, int p, int idx) { return {QUERY, w, p, idx}; }
    inline static Operation Construct(int u, int v, int idx) { return {CONSTRUCT, u, v, idx}; }
    inline static Operation Destroy(int u, int v, int idx) { return {DESTROY, u, v, idx}; }
};

const int RAND = chrono::high_resolution_clock::now().time_since_epoch().count();

inline size_t hash_combine(size_t a, size_t b) {
    a *= 0x9e3779b97f4a7c15;
    return a ^ (b + (a << 6) + (a >> 2));
}

inline size_t hashing(size_t x) {
    x += 0x9e3779b97f4a7c15 + RAND;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
    x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
    return x ^ (x >> 31);
}

namespace std {
    template <> struct hash<Modify> {
        inline std::size_t operator()(const Modify& m) const {
            return hash_combine(hash_combine(hashing(m.a), hashing(m.b)), hashing(m.op));
        }
    };
    template <> struct hash<pair<int, int>> {
        inline std::size_t operator()(const pair<int, int>& p) const {
            return hash_combine(hashing(p.first), hashing(p.second));
        }
    };
}

int ans[MAX];

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    #ifndef ONLINE_JUDGE
    string file = "04-09";
    // string file = "04-16";
    freopen(("data/in/" + file + ".txt").c_str(), "r", stdin);
    ifstream ans_stream("data/out/" + file + ".txt");
    time_t start = clock();
    time_t measure = 0;
    #endif

    // TODO: 상수 커팅하기 (최적화 하기)
    int N, M, Q; cin >> N >> M >> Q;
    // if (M <= 5000 || Q <= 5000) return 0; // 부분 정답 방지용

    vector<pair<int, int>> compress(M);
    for (int i = 0; i < M; ++i) {
        cin >> plans[i].op >> plans[i].a >> plans[i].b;
        // ensure a < b
        if (plans[i].a > plans[i].b) swap(plans[i].a, plans[i].b);
        compress[i] = {plans[i].a, plans[i].b};
    }
    for (int i = 0; i < Q; ++i) cin >> queries[i].w >> queries[i].p, queries[i].order = i;

    vector<int> ord(M);
    sort(compress.begin(), compress.end());
    compress.erase(unique(compress.begin(), compress.end()), compress.end());
    for (int i = 0; i < M; ++i) {
        ord[i] = lower_bound(compress.begin(), compress.end(), make_pair(plans[i].a, plans[i].b)) - compress.begin();
    }

    #ifndef ONLINE_JUDGE
    vector<Query> queries_copy(queries, queries + Q);
    #endif

    sort(queries, queries + Q, [&](const Query &a, const Query &b) { return a.w < b.w; });

    vector<Operation> ops; // 건설하거나 제거하거나 쿼리를 처리하는 연산
    // hashset<pair<int, int>> edges; // 모든 간선 목록
    hashset<int> edges; // 모든 간선 목록
    vector<pair<Modify, int>> modify; // 변경되는 간선 목록 -> 인덱스
    // hashset<pair<int, int>> modify_set; // 변경되는 간선 목록
    // hashset<int> modify_set; // 변경되는 간선 목록
    bool modify_set[MAX]; // 변경되는 간선 목록
    hashmap<pair<int, int>, bool> tmp; // 변경되는 간선들 중, 날짜 범위 이전에도 존재하는 간선 목록

    // 날짜를 묶어서 처리할 것임
    // 음 아닌가 쿼리를 묶어서 처리할까??
    // for (int l = 0, r; l < M; l = r) {
    for (int l = 0, r; l < Q; l = r) {
        #ifndef ONLINE_JUDGE
        cout << "Current block: " << l << endl;
        #endif

        // r = min(M, l + SQ);
        r = min(Q, l + SQ);
        uf1.init(N); uf2.init(N); edges.clear();

        edges.resize(M); edges.set_loads({0.25, 0.5});

        ops.clear(); modify.clear(); tmp.clear();
        ops.reserve(r - l + Q); modify.reserve(r - l);
        fill(modify_set, modify_set + MAX, false);
        // 날짜 범위 중에 변경되는 간선들 계산
        int left = queries[l].w, right = queries[r - 1].w + 1;
        for (int i = left; i < right; ++i) {
            modify.emplace_back(Modify(plans[i].a, plans[i].b, plans[i].op), i);
            // modify_set.insert({plans[i].a, plans[i].b});
            modify_set[ord[i]] = true;
        }
        // 날짜 범위 중 변경되지 않는 간선들 계산
        int cnt = 0;
        for (int i = 0; i < left; ++i) {
            if (modify_set[ord[i]]) {
                if (plans[i].op == 0) {
                    tmp[{plans[i].a, plans[i].b}] = true;
                } else {
                    // assert(it != tmp.end());
                    tmp[{plans[i].a, plans[i].b}] = false;
                }
                continue;
            } else if (plans[i].op == 0) {
                // edges.insert({plans[i].a, plans[i].b});
                edges.insert(ord[i]);
            } else {
                // edges.erase({plans[i].a, plans[i].b});
                edges.erase(ord[i]);
            }
        }
        // 날짜 범위 중 변경되지 않는 간선을 연산에 추가
        for (const auto& i : edges) {
            const auto& [a, b] = compress[i];
            ops.push_back(Operation::Construct(a, b, -1));
        }
        // 날짜 범위에 포함되는 쿼리를 연산에 추가
        for (int i = l; i < r; ++i) {
            ops.push_back(Operation::Query(queries[i].w, queries[i].p, queries[i].order));
        }
        // 1~W까지의 연결 컴포넌트의 개수 계산
        sort(ops.begin(), ops.end(), [&](const Operation &a, const Operation &b) { 
            if (a.v == b.v) return a.op > b.op;
            return a.v < b.v;
        });
        for (const auto& [op, u, v, idx] : ops) {
            if (op == Operation::CONSTRUCT) {
                uf1.merge(u, v);
            } else if (op == Operation::DESTROY) {
                assert(0);
            } else {
                auto mp(tmp); // 변경 사항이 있는 간선의 최종 상태

                for (const auto& m : modify) {
                    const auto& [p, ord] = m;
                    const auto& [a, b, o] = p;
                    if (ord > u) break;
                    if (uf1.find(a) == uf1.find(b)) continue;
                    if (o == 0) mp[{a, b}] = true;
                    else {
                        // auto it = mp.find({a, b});
                        // assert(it != mp.end());
                        // mp.erase(it);
                        mp[{a, b}] = false;
                    }
                }
                for (const auto& [key, value] : mp) {
                    if (!value) continue;
                    const auto& [a, b] = key;
                    if (b > v) continue;
                    uf1.merge(a, b, true);
                }
                ans[idx] = uf1.comp - (N - v - 1);
                // assert(ans[idx] >= 0);
                uf1.rollback_all();
            }
        }
        // W~N까지의 연결 컴포넌트의 개수 계산
        sort(ops.begin(), ops.end(), [&](const Operation &a, const Operation &b) {
            int au = a.u, av = a.v, bu = b.u, bv = b.v;
            if (a.op == Operation::QUERY) swap(au, av);
            if (b.op == Operation::QUERY) swap(bu, bv);
            if (au == bu) return a.op < b.op;
            return au > bu;
        });
        for (const auto& [op, u, v, idx] : ops) {
            if (op == Operation::CONSTRUCT) {
                uf2.merge(u, v);
            } else if (op == Operation::DESTROY) {
                assert(0);
            } else {
                auto mp(tmp); // 변경 사항이 있는 간선의 최종 상태
                for (const auto& m : modify) {
                    const auto& [p, ord] = m;
                    const auto& [a, b, op] = p;
                    if (ord > u) break;
                    if (op == 0) mp[{a, b}] = true;
                    else {
                        // auto it = mp.find({a, b});
                        // assert(it != mp.end());
                        // mp.erase(it);
                        mp[{a, b}] = false;
                    }
                }
                for (const auto& [key, value] : mp) {
                    if (!value) continue;
                    const auto& [a, b] = key;
                    if (a <= v) continue;
                    uf2.merge(a, b, true);
                }
                ans[idx] += uf2.comp - (v + 1);
                // assert(ans[idx] >= 0);
                uf2.rollback_all();
            }
        }
    }

    #ifdef ONLINE_JUDGE
    for (int i = 0; i < Q; ++i) cout << ans[i] << '\n';
    #else
    for (int i = 0; i < Q; ++i) {
        int x; ans_stream >> x;
        if (ans[i] != x) {
            cout << "Mismatch at " << i << ": output->" << ans[i] << " but expected->" << x << '\n';
            cout << "Query: " << queries_copy[i].w << ' ' << queries_copy[i].p << '\n';

            multiset<pair<int, int>> mp;
            for (int j = 0; j <= queries_copy[i].w; j++) {
                if (plans[j].op == 0) mp.emplace(plans[j].a, plans[j].b);
                else {
                    auto it = mp.find({plans[j].a, plans[j].b});
                    mp.erase(it);
                }
            }
            cout << "Plans: " << mp.size() << '\n';
            for (const auto& [a, b] : mp) cout << a << ' ' << b << '\n';
            UnionFind uf; uf.init(N);
            for (auto [a, b] : mp) if (b > queries_copy[i].p) uf.merge(a, b);
            cout << "UF1: " << uf.comp << '\n';
            uf.init(N);
            for (auto [a, b] : mp) if (a <= queries_copy[i].p) uf.merge(a, b);
            cout << "UF2: " << uf.comp << '\n';

            break;
        }
    }
    cout << "Passed\n";
    cout << "Elapsed time: " << (double)(clock() - start) / CLOCKS_PER_SEC << "s\n";
    cout << "Measure time: " << (double)measure / CLOCKS_PER_SEC << "s\n";
    #endif

    return 0;
}