#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MOD = 1e9 + 7;

void print_vector(const vector<int>& v, string name = "") {
    cout << name << ": ";
    for (int i = 0; i < v.size(); ++i) {
        cout << v[i] << ' ';
    }
    cout << '\n';
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, x0, a, b; cin >> N >> x0 >> a >> b;
    int Q; cin >> Q;

    vector<int> queries(Q), D(Q), L(Q, 0), R(Q, MOD), M(Q, L[0] + R[0] >> 1);

    for (int i = 0; i < Q; ++i) {
        cin >> queries[i]; queries[i]++;
    }
    sort(queries.begin(), queries.end());

    while (true) {
        fill(D.begin(), D.end(), 0);
        int v = x0;
        for (int i = 0; i < N; ++i) {
            int pos = lower_bound(M.begin(), M.end(), v) - M.begin();
            D[pos]++;
            v = ((ll)v * a + b) % MOD;
        }
        // print_vector(D, "D");

        bool done = true;
        int p = 0; // 누적합 느낌으로 관리
        for (int i = 0; i < Q; ++i) {
            p += D[i];
            if (L[i] >= R[i]) continue;
            if (p >= queries[i]) {
                R[i] = M[i];
            } else {
                L[i] = M[i] + 1;
            }
            M[i] = L[i] + R[i] >> 1;
            if (L[i] ^ R[i]) done = false;
        }

        // print_vector(L, "L");
        // print_vector(R, "R");
        // print_vector(M, "M");

        // cout << '\n';

        if (done) {
            break;
        }
    }

    // for (int i = 0; i < Q; ++i) {
    //     cout << L[i] << '\n';
    // }
    cout << accumulate(L.begin(), L.end(), 0) << '\n';

    return 0;
}