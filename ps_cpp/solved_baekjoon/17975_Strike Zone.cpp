#include <bits/stdc++.h>

using namespace std;
typedef long long int ll;

constexpr int MAX = 2001, SEGSZ = 1 << 13;

struct ball
{
    int x, y; bool isP1;
} balls[MAX];

int xTmp[MAX], yTmp[MAX], loc[MAX];
vector<pair<int, int>> delta[MAX];

struct node
{
    ll lmx, rmx, mx, sum;
    node(ll v) {
        lmx = rmx = mx = sum = v;
    }
    node() : node(0) {}
    node operator+(const node& r) const{
        node ret = node();
        ret.lmx = max(lmx, sum + r.lmx);
        ret.rmx = max(r.rmx, rmx + r.sum);
        ret.mx = max({mx, r.mx, rmx + r.lmx});
        ret.sum = sum + r.sum;
        return ret;
    }
} seg[SEGSZ];

void initLoc(int s, int e, int i = 1){
    if (s == e) {
        loc[s] = i;
        return;
    }
    const int m = (s + e) >> 1;
    initLoc(s, m, i << 1); initLoc(m + 1, e, i << 1 | 1);
}

void add(int idx, int v){
    int i = loc[idx];
    seg[i] = node(seg[i].lmx + v);
    while (i >>= 1) seg[i] = seg[i << 1] + seg[i << 1 | 1];
}

inline ll getMax(){
    return max({seg[1].lmx, seg[1].rmx, seg[1].mx});
}

int main(){
    ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    // 입력
    int n1, n2, n, c1, c2; cin >> n1;
    for (int i = 0; i < n1; ++i) cin >> balls[i].x >> balls[i].y, balls[i].isP1 = true, xTmp[i] = balls[i].x, yTmp[i] = balls[i].y;
    cin >> n2; n = n1 + n2;
    for (int i = n1; i < n; ++i) cin >> balls[i].x >> balls[i].y, balls[i].isP1 = false, xTmp[i] = balls[i].x, yTmp[i] = balls[i].y;
    cin >> c1 >> c2;

    // 좌표 압축
    int x = 0, y = 0;
    sort(xTmp, xTmp + n); sort(yTmp, yTmp + n);
    unordered_map<int, int> xComp(n), yComp(n);
    for (int i = 0; i < n; ++i){
        if (xComp.find(xTmp[i]) == xComp.end()) xComp[xTmp[i]] = x++;
        if (yComp.find(yTmp[i]) == yComp.end()) yComp[yTmp[i]] = y++;
    }
    for (int i = 0; i < n; ++i) balls[i].x = xComp[balls[i].x], balls[i].y = yComp[balls[i].y];

    // 스위핑
    for (int i = 0; i < n; ++i){
        auto& b = balls[i];
        delta[b.x].push_back({b.y, b.isP1 ? c1 : -c2});
    }


    // 쿼리 수행
    initLoc(0, n - 1);
    ll ans = 0;
    for (int i = 0; i < n; ++i){
        for (auto& d : delta[i])
            add(d.first, d.second);
        ans = max(ans, getMax());
        for (int j = 0; j < i; ++j){
            for (auto& d : delta[j])
                add(d.first, -d.second);
            ans = max(ans, getMax());
        }
        for (int j = 0; j < i; ++j){
            for (auto& d : delta[j])
                add(d.first, d.second);
        }
    }

    cout << ans << '\n';
}