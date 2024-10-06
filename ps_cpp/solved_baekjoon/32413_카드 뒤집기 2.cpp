#include <bits/stdc++.h>

using namespace std;
constexpr int sz = 400005, sq = 635;

int a[sz >> 1], b[sz >> 1];

struct max_segtree
{
    int tree[1 << (33 - __builtin_clz(sz))], sgsz = 1 << (32 - __builtin_clz(sz));
    max_segtree() { fill(tree, tree + (1 << (33 - __builtin_clz(sz))), -1); }
    void init(){
        for (int i = sgsz - 1; i > 0; --i)
            tree[i] = max(tree[i << 1], tree[i << 1 | 1]);
    }
    void update(int i, int val){
        --i |= sgsz; tree[i] = val;
        while (i >>= 1) tree[i] = max(tree[i << 1], tree[i << 1 | 1]);
    }
    int query(int l, int r) {
        --l |= sgsz; --r |= sgsz;
        int ret = -1;
        while (l <= r){
            if (l & 1) ret = max(ret, tree[l++]);
            if (!(r & 1)) ret = max(ret, tree[r--]);
            l >>= 1; r >>= 1;
        }
        return ret;
    }
} sg;


struct sqrt_decompisition_sort
{
    int naive[sz], bucket[sq][sq]; bool sorted[sq];
    sqrt_decompisition_sort() {
        fill(naive, naive + sz, 0);
        // 초기에는 모든 값을 최댓값으로 초기화
        for (int i = 0; i < sq; ++i) fill(bucket[i], bucket[i] + sq, numeric_limits<int>::max());
    }

    void add(int idx, int val){
        naive[idx] += val;
        auto& b = bucket[idx / sq];
        int offset = idx % sq;
        b[offset] = val;
        sorted[idx / sq] = false;
    }

    int query(int l, int r, int k){
        int ret = 0;
        for (int i = l; i <= r;){
            if (i % sq == 0 && i + sq - 1 <= r){
                // bucket 단위로 처리할 수 있을 경우
                // 해당 bucket에서 k보다 큰 값의 개수를 구함
                if (!sorted[i / sq]) {
                    sort(bucket[i / sq], bucket[i / sq] + sq);
                    sorted[i / sq] = true;
                }
                ret += (bucket[i / sq] + sq) - upper_bound(bucket[i / sq], bucket[i / sq] + sq, k);
                // ret = 0;
                i += sq;
            } else {
                // 아닐 경우 naive하게 처리
                if (naive[i] > k) ret++;
                i++;
            }
        }
        return ret;
    }
} sd2;

int main(){
    // ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);

    // freopen("test.txt", "r", stdin);
    
    int n; cin >> n;
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];

    sg.init();

    for (int i = 0; i < n; ++i){
        int lo = a[i], hi = b[i];
        if (lo > hi) swap(lo, hi);

        if (lo == hi) {
            cout << lo << ' ';
            sg.update(lo, i);
            sd2.add(i, lo);
            continue;
        }

        int l = sg.query(lo + 1, hi);
        if (l == -1) l = 0;
        else a[i] = hi, b[i] = lo;

        int flip_cnt = sd2.query(l, i - 1, hi); // i - 1

        if (flip_cnt & 1) swap(a[i], b[i]);
        cout << a[i] << ' ';

        sg.update(a[i], i);
        sd2.add(i, a[i]);
    }
}