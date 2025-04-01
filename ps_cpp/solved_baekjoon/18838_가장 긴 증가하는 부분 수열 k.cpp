#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 100'005, SIZE = 1 << (33 - __builtin_clz(MAXN));
constexpr ll INF = (ll)1e18 + 5;

struct Node {
    int len; ll cnt;
    Node(int len = 0, ll cnt = 0) : len(len), cnt(cnt) {}
    Node operator+(const Node& o) const {
        if (len > o.len) return *this;
        if (len < o.len) return o;
        return Node(len, min(cnt + o.cnt, INF));
    }
} T[SIZE];

void add(int l, int r, int pos, Node v, int i = 1) {
    if (pos < l || r < pos) return;
    if (l == r) { T[i] = T[i] + v; return; }
    int m = l + r >> 1;
    add(l, m, pos, v, i << 1); add(m + 1, r, pos, v, i << 1 | 1);
    T[i] = T[i << 1] + T[i << 1 | 1];
}

Node query(int l, int r, int ql, int qr, int i = 1) {
    if (r < ql || qr < l) return Node();
    if (ql <= l && r <= qr) return T[i];
    int m = l + r >> 1;
    return query(l, m, ql, qr, i << 1) + query(m + 1, r, ql, qr, i << 1 | 1);
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N; ll K; cin >> N >> K;
    vector<int> A(N);
    for (int i = 0; i < N; ++i) {
        cin >> A[i];
    }

    vector<pair<int, ll>> L(N); // L[i]: i에서 시작하는 LIS의 길이/개수
    // 가장 긴 증가하는 부분 수열 6에서는 L[i]: i에서 끝나는... 이었지만 이제는 시작하는... 으로 바뀌었다
    // => 수열을 뒤집어서 가장 긴 "감소하는" 부분 수열 문제를 해결하면 된다
    for (int i = N - 1; i >= 0; --i) {
        L[i] = {1, 1};
        auto q = query(1, N, A[i] + 1, N);
        if (q.len) L[i] = {q.len + 1, q.cnt};
        add(1, N, A[i], Node(q.len + 1, q.cnt == 0 ? 1 : q.cnt));
    }

    int k = (*max_element(L.begin(), L.end(), [](const pair<int, ll>& a, const pair<int, ll>& b) { return a.first < b.first; })).first;
    ll all_cnt = accumulate(L.begin(), L.end(), 0LL, [&](ll a, const pair<int, ll>& b) { return min(a + (b.first == k ? b.second : 0), INF); });

    assert(all_cnt <= INF && all_cnt >= 0);
    // 전체 LIS의 개수가 K보다 작으면 -1 출력
    if (all_cnt < K) {
        cout << "-1\n";
        return 0;
    }

    vector<vector<int>> sweep(k + 1); // sweep[i]: LIS의 길이가 i인 인덱스들의 모음
    for (int i = 0; i < N; ++i) {
        sweep[L[i].first].push_back(i);
    }

    for (int i = 1; i <= k; ++i) { // A[i]를 기준으로 오름차순 정렬을 수행한다
        sort(sweep[i].begin(), sweep[i].end(), [&](int a, int b) { return A[a] < A[b]; });
    }

    vector<int> ans;
    int start = 0;
    for (int i = k; i >= 1; --i) { // 역추적을 잘 하면 된다
        for (auto &j : sweep[i]) {
            if (j < start || A[j] < (ans.empty() ? 0 : ans.back())) continue;
            if (L[j].second < K) {
                K -= L[j].second;
            } else {
                ans.push_back(A[j]);
                start = j + 1;
                break;
            }
        }
    }

    for (int i = 0; i < k; ++i) {
        cout << ans[i] << ' ';
    }
    cout << '\n';

    return 0;
}