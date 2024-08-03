#include <bits/stdc++.h>
#define first x
#define second y

using namespace std;
typedef long long int ll;

constexpr int MAX = 3001, SEGSZ = 1 << 14;
// constexpr int MAX = 15, SEGSZ = 1 << 4;

struct gold
{
    int x, y;
    ll w;
} g[MAX];

int xTmp[MAX], yTmp[MAX], loc[MAX];
vector<pair<int, ll>> delta[MAX];

struct node
{
    ll lmx, rmx, mx, sum;
    node(ll v) {
        lmx = rmx = mx = sum = v;
    }
    node () : node(0) {}
    node operator+(const node& o) const{
        node ret = node();
        ret.lmx = max(lmx, sum + o.lmx);
        ret.rmx = max(o.rmx, o.sum + rmx);
        ret.mx = max({mx, o.mx, rmx + o.lmx});
        ret.sum = sum + o.sum;
        return ret;
    }
} seg[SEGSZ];

void initLoc(int s, int e, int i = 1){
    if (s == e){
        loc[s] = i;
        return;
    }
    int m = (s + e) >> 1;
    initLoc(s, m, i << 1); initLoc(m + 1, e, i << 1 | 1);
}

void add(int idx, int v){
    int i = loc[idx];
    seg[i] = node(seg[i].lmx + v);
    while (i >>= 1) seg[i] = seg[i << 1] + seg[i << 1 | 1];
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    int n; cin >> n;
    for (int i = 0; i < n; ++i) cin >> g[i].x >> g[i].y >> g[i].w, xTmp[i] = g[i].x, yTmp[i] = g[i].y;

    // 좌표 압축
    sort(xTmp, xTmp + n); sort(yTmp, yTmp + n);
    unordered_map<int, int> xComp(n), yComp(n);
    int x = 0, y = 0;
    for (int i = 0; i < n; ++i){
        if (xComp.find(xTmp[i]) == xComp.end()) xComp[xTmp[i]] = x++;
        if (yComp.find(yTmp[i]) == yComp.end()) yComp[yTmp[i]] = y++;
    }
    for (int i = 0; i < n; ++i) g[i].x = xComp[g[i].x], g[i].y = yComp[g[i].y];

    // rec[i][j] = (0, 0)에서 (i, j)까지의 가치의 누적합
    for (int i = 0; i < n; ++i){
        auto& [x, y, w] = g[i];
        delta[x].push_back({y, w});
    }
    
    initLoc(0, n - 1);
    ll ans = 0;
    for (int i = 0; i < x; ++i){
        for (auto& [y, w] : delta[i]){
            add(y, w);
        }
        auto& nd = seg[1];
        ans = max(ans, max({nd.lmx, nd.rmx, nd.mx}));
        for (int j = 0; j < i; ++j){
            for (auto& [y, w] : delta[j]){
                add(y, -w);
            }
            auto& nd = seg[1];
            ans = max(ans, max({nd.lmx, nd.rmx, nd.mx}));
        }
        for (int j = 0; j < i; ++j){
            for (auto& [y, w] : delta[j]){
                add(y, w);
            }
        }
    }

    cout << ans << '\n';
}