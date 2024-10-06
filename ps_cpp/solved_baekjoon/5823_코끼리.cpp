#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#include <bits/stdc++.h>

using namespace std;
constexpr int SZ = 150'005, SQ = 400;
// 코끼리의 번호 -> 위치
int N, L, M, elephant_pos[SZ], sorted[SZ];
struct Bucket {
    struct Elephant {
        // pos: 이 코끼리의 위치
        // camera: 이 코끼리에서 시작해서 Bucket의 마지막 코끼리를 찍기 위한 카메라의 개수
        // last: 마지막 코끼리까지 찍었을 때 가장 오른쪽 카메라가 커버하는 위치
        int pos, camera, last;
        Elephant(int pos = 0, int camera = 1, int last = 0) : pos(pos), camera(camera), last(last) {}
        inline bool operator<(const Elephant &o) const { return pos < o.pos; }
    };
    vector<Elephant> elephants;
    
    void _build() {
        int size = elephants.size();
        for (int i = size - 1; i >= 0; --i) {
            auto& cur = elephants[i];
            int cover = cur.pos + L;
            cur.last = cover;
            // nxt: 현재 코끼리 + L를 초과하는 가장 왼쪽에 있는 코끼리
            auto nxtIt = upper_bound(elephants.begin() + i, elephants.end(), Elephant(cover));
            if (nxtIt == elephants.end()) {
                cur.camera = 1;
                continue;
            } // 현재 코끼리 + L가 버킷의 마지막 코끼리까지 커버하는 경우
            auto& nxt = *nxtIt;
            cur.camera = nxt.camera + 1;
            cur.last = nxt.last;
        }
    }

    void remove(int v) {
        // assert(elephants.size() > 0);
        auto it = lower_bound(elephants.begin(), elephants.end(), Elephant(v));
        // if (it == elephants.end()) exit(1);
        elephants.erase(it);
        _build();
    }

    void add(int v) {
        auto it = lower_bound(elephants.begin(), elephants.end(), Elephant(v));
        elephants.insert(it, Elephant(v, 1, v + L));
        _build();
    }

    static Bucket dummy(int pos) { 
        Bucket b;
        b.elephants.emplace_back(Elephant(pos));
        return b;
    }

    int size() { return elephants.size(); }

    void init(int l, int r, int exclude = -1) {
        elephants.clear();
        for (int i = l; i < r; ++i) {
            if (i == exclude) continue;
            auto& cur = sorted[i];
            elephants.emplace_back(cur, 1, cur + L);
        }

        _build();
    }

    inline bool operator<(const Bucket &o) const {
        return elephants.front().pos < o.elephants.front().pos;
    }
} pool[SQ * 2]; int pool_size = 0;

void init() {
    for (int i = 0; i < N; ++i) sorted[i] = elephant_pos[i];
    sort(sorted, sorted + N);
    pool_size = 0;

    for (int l = 0; l < N;) {
        int r = min(N, l + SQ);
        pool[pool_size++].init(l, r);
        l = r;
    }
}

// i번째 코끼리의 위치를 v로 변경
void update(int i, int v) {
    // i번째 코끼리를 현재 버킷에서 제거
    int prev_pos = elephant_pos[i];
    auto pb = prev(upper_bound(pool, pool + pool_size, Bucket::dummy(prev_pos)));
    pb->remove(prev_pos);
    // i번째 코끼리의 위치를 v로 변경
    elephant_pos[i] = v;
    // i번째 코끼리를 새로운 버킷에 추가
    auto nb = prev(upper_bound(pool, pool + pool_size, Bucket::dummy(v)));
    if (nb < pool) nb = pool;
    nb->add(v);
}

// 필요한 카메라의 총 개수
int evaluate() {
    int ret = 0, last = -1;

    for (int i = 0; i < pool_size; ++i) {
        auto& b = pool[i]; auto& e = b.elephants;
        if (b.size() == 0) continue;
        if (last == -1) {
            ret += e.front().camera;
            last = e.front().last;
            continue;
        }
        if (last >= e.back().pos) continue;
        auto it = upper_bound(e.begin(), e.end(), Bucket::Elephant(last));
        // if (it == e.end()) continue;
        auto& cur = *it;
        ret += cur.camera;
        last = cur.last;
    }

    return ret;
}

#ifdef ONLINE_JUDGE
struct Query { int i, v; } q[SZ];
#else
struct TestQuery { int i, v, expected; } q[SZ];
#endif


int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("elephants-test/subtask4/grader.in.6", "r", stdin);
#endif

    cin >> N >> L >> M;
    for (int i = 0; i < N; ++i) cin >> elephant_pos[i];
#ifdef ONLINE_JUDGE
    for (int i = 0; i < M; ++i) cin >> q[i].i >> q[i].v;
#else
    for (int i = 0; i < M; ++i) cin >> q[i].i >> q[i].v >> q[i].expected;
#endif

    for (int l = 0; l < M;) {
        int r = min(M, l + SQ);
        init();
        for (int i = l; i < r; ++i) {
#ifdef ONLINE_JUDGE
            auto& [idx, v] = q[i];
#else
            auto& [idx, v, expected] = q[i];
#endif
            update(idx, v);
            int res = evaluate();
#ifdef ONLINE_JUDGE
            cout << res << '\n';
#else
            if (res != expected) cout << "Wrong Answer: " << i << " " << res << " " << q[i].expected << '\n';
            else cout << i << '\n';
#endif
        }
        l = r;
    }

    return 0;
}