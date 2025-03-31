#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long int ull;
constexpr int MAXN = 17'005;

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    string S, T; cin >> S >> T;
    S = ' ' + S; T = ' ' + T;
    int N = S.size(), M = T.size();

    // track 00: 왼쪽, 01: 위쪽, 10: 대각선
    vector<pair<bitset<MAXN>, bitset<MAXN>>> track(N);
    
    for (int i = 0; i < N; ++i) {
        auto &[b1, b2] = track[i];
        b2[0] = 1;
    }

    vector<int> prev_cnt(M), cur_cnt(M);
    iota(prev_cnt.begin(), prev_cnt.end(), 0);

    for (int i = 1; i < N; ++i) {
        cur_cnt[0] = i;
        for (int j = 1; j < M; ++j) {
            int left = cur_cnt[j - 1] + 1,
                up = prev_cnt[j] + 1,
                diag = prev_cnt[j - 1] + (S[i] == T[j] ? 0 : 1),
                mn = min({left, up, diag});

                cur_cnt[j] = mn;

                auto &[b1, b2] = track[i];
                if (mn == left) {
                    // 00
                    b1[j] = b2[j] = 0;
                } else if (mn == up) {
                    // 01
                    b2[j] = 1;
                } else {
                    // 10
                    b1[j] = 1;
                }
        }

        swap(prev_cnt, cur_cnt);
    }

    vector<pair<char, char>> inst;
    int i = N - 1, j = M - 1;
    while (i || j) {
        auto &[b1, b2] = track[i];
        if (b1[j] && !b2[j]) { // 10
            if (S[i] == T[j]) {
                inst.emplace_back('c', T[j]);
            } else {
                inst.emplace_back('m', T[j]);
            }
            --i; --j;
        } else if (!b1[j] && b2[j]) { // 01
            inst.emplace_back('d', S[i]);
            --i;
        } else { // 00
            inst.emplace_back('a', T[j]);
            --j;
        }
    }

    reverse(inst.begin(), inst.end());
    for (auto &[op, c] : inst) {
        cout << op << ' ' << c << '\n';
    }

    return 0;
}