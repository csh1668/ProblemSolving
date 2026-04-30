#include <bits/stdc++.h>
using namespace std;
using ll = long long;
typedef long long lint;


int N;

int dp[10101];

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    cin >> N;
    vector<int> A, B;
    
    fill(dp, dp+10101, 1e9);

    for (int i=0;i<N;i++) {
        int x; cin >> x;
        A.push_back(x);
    }

    for (int i=0;i<N;i++) {
        int x; cin >> x;
        B.push_back(x);
    }

    dp[0] = 1;

    for (int i=1;i<N;i++) {
        int mnA = A[i], mnB = B[i];
        int idxA = i, idxB = i;

        for (int j=i;j>=0;j--) {
            if (A[j] < mnA) {
                mnA = A[j];
                idxA = j;
            }

            if (B[j] < mnB) {
                mnB = B[j];
                idxB = j;
            }

            if (idxA == idxB) {
                dp[i] = min(dp[i], (j-1 >= 0 ? dp[j-1] : 0) + 1);
            }
        }
    }

    cout << dp[N-1];
    return 0;
}