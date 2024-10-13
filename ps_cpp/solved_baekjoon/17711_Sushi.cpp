#pragma GCC optimize("O3")
#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 400'001, MAXM = 25'001, SQ = 632;
// constexpr int MAXN = 10'000, MAXM = 10'000, SQ = 100;
// 현재 손님이 들고 있는 접시의 가치
int naive[MAXN], N, M;

template <typename Cmp> struct heap {
    vector<int> _in;
    void clear() { _in.clear(); }
    void init(int* begin, int* end) {
        _in.clear();
        _in.reserve(end - begin);
        for (int* it = begin; it != end; ++it) {
            _in.push_back(*it);
        }
        make_heap(_in.begin(), _in.end(), Cmp());
    }
    void push(int v) {
        _in.push_back(v);
        push_heap(_in.begin(), _in.end(), Cmp());
    }
    int pop() {
        pop_heap(_in.begin(), _in.end(), Cmp());
        int ret = _in.back();
        _in.pop_back();
        return ret;
    }
    int size() { return _in.size(); }
};

struct Bucket {
    heap<less<int>> mx; // 최대힙
    heap<greater<int>> lazy; // 최소힙
    int left, right; // 버킷의 범위
    bool should_update_bucket = false, should_update_naive = false;

    // 버킷을 업데이트 O(sqrtN)
    void init_bucket() {
        mx.init(naive + left, naive + right + 1);
        should_update_bucket = false;
    }

    // naive 배열을 업데이트 O(sqrtNlogM)
    void init_naive() {
        for (int i = left; i <= right; ++i) {
            lazy.push(naive[i]);
            naive[i] = lazy.pop();
        }
        lazy.clear();
        should_update_naive = false;
    }

    // 버킷에 접시를 추가하고 빠져나오는 접시 반환 O(log(sqrtN))
    int _query(int p) {
        mx.push(p);
        lazy.push(p);
        return mx.pop();
    }
} pool[SQ + 5];

void init() {
    int cnt = 0;
    for (int l = 0; l < N;) {
        int r = min(N, l + SQ);
        pool[cnt].left = l; pool[cnt].right = r - 1;
        pool[cnt].init_bucket();
        cnt++;
        l = r;
    }
}

// [l, r] 구간, p: 새로 들어온 접시의 가치 O(sqrtN)
// l <= r
int _query(int l, int r, int p) {
    for (int i = l; i <= r;) {
        if (i % SQ == 0 && i + SQ - 1 <= r) {
            // bucket 단위로 처리할 수 있을 경우
            auto& b = pool[i / SQ];
            if (b.should_update_bucket) b.init_bucket();
            p = b._query(p);
            b.should_update_naive = true;
            i += SQ;
        } else {
            // 아닐 경우 naive하게 처리
            auto& b = pool[i / SQ];
            if (b.should_update_naive) b.init_naive();
            auto& cur = naive[i];
            if (cur > p) swap(cur, p);
            b.should_update_bucket = true;
            i++;
        }
    }
    return p;
}

// l > r인 경우도 처리
int query(int l, int r, int p) {
    if (l <= r) return _query(l, r, p);
    p = _query(l, N - 1, p);
    return _query(0, r, p);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    cin >> N >> M;
    for (int i = 0; i < N; ++i) cin >> naive[i];
    init();
    for (int i = 0; i < M; ++i) {
        int s, t, p; cin >> s >> t >> p; --s, --t;
        cout << query(s, t, p) << '\n';
    }

    return 0;
}