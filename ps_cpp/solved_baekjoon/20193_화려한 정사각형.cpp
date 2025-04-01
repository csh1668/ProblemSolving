#include <bits/stdc++.h>

using namespace std;
constexpr int MAXN = 100'002, MAXCOORD = 250'002, SIZE = 1 << (33 - __builtin_clz(MAXCOORD));

struct Point {
    int x, y, k;
} P[MAXN];
vector<int> X[MAXCOORD];

int T[SIZE], L[SIZE];

multiset<int> S[MAXN];

void reset() {
    // fill(T, T + SIZE, 0);
    // fill(L, L + SIZE, 0);
    memset(T, 0, sizeof T);
    memset(L, 0, sizeof L);

    for (int i = 1; i < MAXN; ++i) {
        S[i].clear();
        S[i].insert(INT_MIN);
        S[i].insert(INT_MAX);
    }
}

void push(int l, int r, int i) {
    if (L[i] == 0) return;
    T[i] += L[i];
    if (l ^ r) {
        L[i << 1] += L[i];
        L[i << 1 | 1] += L[i];
    }
    L[i] = 0;
}

void update(int l, int r, int ql, int qr, int v, int i = 1) {
    push(l, r, i);
    if (r < ql || qr < l) return;
    if (ql <= l && r <= qr) {
        L[i] += v;
        push(l, r, i);
        return;
    }
    int m = l + r >> 1;
    update(l, m, ql, qr, v, i << 1); update(m + 1, r, ql, qr, v, i << 1 | 1);
    T[i] = max(T[i << 1], T[i << 1 | 1]);
}

int& top(int l, int r) {
    push(l, r, 1);
    return T[1];
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, K; cin >> N >> K;
    for (int i = 1; i <= N; ++i) {
        auto &p = P[i];
        cin >> p.x >> p.y >> p.k;
        X[p.x].push_back(i);
    }

    auto can = [&](int len) -> bool {
        reset();

        /*
        사고 실험...
        len = 5이고 아래 좌표들이 모두 같은 k일때
        y = 1 (1~5)
        y = 7 (7~11)인 상태에서
        y = 3 (3~7)을 추가하려고 할 때 중복으로 더하는 경우가 발생한다.
        이를 방지하기 위해서는....

        y = 3를 추가할 때, multiset에 3 바로 이전 값이 1이므로 1을 가져와서 1 + 5 = 6과 3 중 더 큰 값으로 추가한다
        y = 3과정에서 7을 추가하려고 한다면 3 바로 다음값이 7이므로 y + len = 3 + 5 = 8과 7중 더 작은 값으로 추가한다
        즉 구간이 [6~7]이 된다.
        */
        for (int i = 1; i < MAXCOORD; ++i) {
            for (auto &j : X[i]) {
                auto &p = P[j];
                auto it = S[p.k].insert(p.y);
                int left = max(p.y, *prev(it) + len);
                int right = min(p.y + len, *next(it));
                update(1, MAXCOORD - 1, left, right - 1, 1);
                
                // cout << "i: " << i << ", j: " << j << ", p.k: " << p.k << ", p.y: " << p.y << ", left: " << left << ", right: " << right << "top: " << top(1, MAXCOORD - 1) << endl;
            }

            int cnt = top(1, MAXCOORD - 1);
            // cout << "cnt: " << cnt << endl;
            // assert(0 <= cnt && cnt <= K);
            if (cnt == K) 
                return true;


            if (i - len + 1 < 0) continue;
            for (auto &j : X[i - len + 1]) {
                auto &p = P[j];
                auto it = S[p.k].lower_bound(p.y);
                int left = max(p.y, *prev(it) + len);
                int right = min(p.y + len, *next(it));
                update(1, MAXCOORD - 1, left, right - 1, -1);
                S[p.k].erase(it);
            }
        }

        return false;
    };

    int lo = 1, hi = MAXCOORD;
    while (lo < hi) {
        int mid = lo + hi >> 1;
        if (can(mid)) hi = mid;
        else lo = mid + 1;
    }

    cout << lo - 1 << '\n';

    return 0;
}