#include <iostream>
#include <algorithm>
#include <cmath>
#include <string.h>

using namespace std;
typedef long long int ll;
int n, sq = 223, ans = 0;

struct node
{
	int idx, l, r;
	bool operator<(const node& other) const {
		if (l / sq == other.l / sq) return r < other.r;
		return l / sq < other.l / sq;
	}
} queries[500000];


ll arr[500000];
int cnt[500001];
int res[500001];

void add_recursive(ll v){
	++cnt[v];
	if (v != 1 && cnt[v] == v){
		cnt[v] = 0; ans++;
		if (v * v <= n) add_recursive(v * v);
	}
}

inline void add(int k){
	ll val = arr[k];
	if (val > n) return;

	add_recursive(val);
}

void sub_recursive(ll v){
	if (v != 1 && cnt[v] == 0){
		if (v * v <= n) sub_recursive(v * v);
		cnt[v] = v; ans--;
	}
	cnt[v]--;
}

inline void sub(int k){
	ll val = arr[k];
	if (val > n) return;

	sub_recursive(val);
}

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);

	// freopen("test.txt", "r", stdin);

	int t; cin >> t;
	for (int test = 0; test < t; test++){
		memset(cnt, 0, sizeof(cnt)); ans = 0;
		int q; cin >> n;
		for (int i = 0; i < n; i++) cin >> arr[i];
		cin >> q;
		for (int i = 0; i < q; i++){
			int l, r; cin >> l >> r;
			queries[i] = {i, l - 1, r - 1};
		}
		sort(queries, queries + q);

		int x = 0, y = 0;
		for (int i = 0; i < q; i++){
			int &l = queries[i].l, &r = queries[i].r, &idx = queries[i].idx;
			if (i == 0) x = l, y = l - 1;
			while (y < r) add(++y);
			while (y > r) sub(y--);
			while (x > l) add(--x);
			while (x < l) sub(x++);
			res[idx] = ans;
		}

		cout << "Case #" << test + 1 << '\n';
		for (int i = 0; i < q; i++){
			cout << res[i] << '\n';
		}
		cout.flush();
	}


	return 0;//Your program should return 0 on normal termination.
}