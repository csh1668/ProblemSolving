#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;
constexpr int MAXN = 100'005;

struct Fenwick {
    ll T[MAXN];
    void add(int i, ll x) { for (; i < MAXN; i += i & -i) T[i] += x; }
    ll prefix(int i) { ll s = 0; for (; i > 0; i -= i & -i) s += T[i]; return s; }
    ll sum(int i, int j) { return prefix(j) - prefix(i - 1); }
} fw1, fw2;

vector<int> C[MAXN];
int S[MAXN], E[MAXN];

int _order = 0;
void dfs(int u) {
    S[u] = ++_order;
    for (auto &v : C[u]) {
        dfs(v);
    }
    E[u] = _order;
}

int main() {
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int N, M; cin >> N >> M;
    int x; cin >> x;
    for (int i = 2; i <= N; ++i) {
        cin >> x;
        C[x].push_back(i);
    }

    dfs(1);

    bool direction = true;
    while (M--) {
        int op; cin >> op;
        if (op == 1) {
            int i, w; cin >> i >> w;
            if (direction) {
                // S[i] ~ E[i]에 w를 더한다
                fw1.add(S[i], w);
                fw1.add(E[i] + 1, -w);
            } else {
                // S[i]에 w를 더한다
                fw2.add(S[i], w);
            }
        } else if (op == 2) {
            int i; cin >> i;
            cout << fw1.prefix(S[i]) + fw2.sum(S[i], E[i] - 1) << '\n';
        } else {
            direction = !direction;
        }
    }

    return 0;
}