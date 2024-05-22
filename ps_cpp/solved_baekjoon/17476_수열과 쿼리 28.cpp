#pragma GCC optimize ("O3")
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")
#include<bits/stdc++.h>

// #include <algorithm>
// #include <iostream>
// #include <stdlib.h>

using namespace std;
typedef long long ll;


struct node
{
	ll range_min, range_sum, range_max;
};

inline ll sqrt_ll(ll a){
	return (ll)sqrt(a);
}

inline node combine(node a, node b) {
	return { min(a.range_min, b.range_min), a.range_sum + b.range_sum, max(a.range_max, b.range_max) };
}

int arr[100000];
ll add_lazy[262144];
ll set_lazy[262144];
node tree[262144];

node init(int left, int right, int i) {
	if (left == right) {
		return tree[i] = { arr[left], arr[left], arr[left] };
	}
	int mid = (left + right) >> 1;
	return tree[i] = combine(init(left, mid, i << 1), init(mid + 1, right, i << 1 | 1));
}

void propagate_lazy(int start, int end, int i) {
	if (set_lazy[i] == 0){
		tree[i].range_max += add_lazy[i];
		tree[i].range_min += add_lazy[i];
		tree[i].range_sum += (end - start + 1) * add_lazy[i];
		if (start != end){
			add_lazy[i << 1] += add_lazy[i];
			add_lazy[i << 1 | 1] += add_lazy[i];
		}
	} else {
		tree[i].range_max = set_lazy[i] + add_lazy[i];
		tree[i].range_min = set_lazy[i] + add_lazy[i];
		tree[i].range_sum = (end - start + 1) * (set_lazy[i] + add_lazy[i]);
		if (start != end){
			add_lazy[i << 1] = add_lazy[i];
			add_lazy[i << 1 | 1] = add_lazy[i];
			set_lazy[i << 1] = set_lazy[i];
			set_lazy[i << 1 | 1] = set_lazy[i];
		}
	}
	add_lazy[i] = 0; set_lazy[i] = 0;
}

ll search_range(int start, int end, int left, int right, int i = 1) {
	propagate_lazy(start, end, i);
	if (end < left || start > right)
		return 0;
	if (left <= start && end <= right)
		return tree[i].range_sum;
	int mid = (start + end) >> 1;
	ll left_seg = search_range(start, mid, left, right, i << 1), right_seg = search_range(mid + 1, end, left, right, i << 1 | 1);
	return left_seg + right_seg;
}

void add_range(int start, int end, int left, int right, int val, int i = 1) {
	propagate_lazy(start, end, i);
	// break condition
	if (end < left || start > right)
		return;
	// tag condition
	if (left <= start && end <= right) {
		// tag
		add_lazy[i] = val;
		propagate_lazy(start, end, i);
		return;
	}
	int mid = (start + end) >> 1;
	add_range(start, mid, left, right, val, i << 1);
	add_range(mid + 1, end, left, right, val, i << 1 | 1);
	tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
	return;
}

void sqrt_range(int start, int end, int left, int right, int i = 1) {
	propagate_lazy(start, end, i);
	// break condition
	if (end < left || start > right)
		return;
	// tag condition
	if (left <= start && end <= right) {
		if (sqrt_ll(tree[i].range_min) == sqrt_ll(tree[i].range_max)){
			set_lazy[i] = sqrt_ll(tree[i].range_max);
			propagate_lazy(start, end, i);
			return;
		}
		if (tree[i].range_min + 1 == tree[i].range_max){
			add_lazy[i] = sqrt_ll(tree[i].range_min) - tree[i].range_min;
			propagate_lazy(start, end, i);
			return;
		}
	}
	int mid = (start + end) >> 1;
	sqrt_range(start, mid, left, right, i << 1);
	sqrt_range(mid + 1, end, left, right, i << 1 | 1);
	tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
	return;
}

int main()
{
	cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
	// freopen("input.txt", "rw", stdin);
	// freopen("output.txt", "w", stdout);

	int n, q; cin >> n;
	for (int i = 0; i < n; i++)
		cin >> arr[i];
	cin >> q;
	init(0, n - 1, 1);

	for (int i = 0; i < q; i++) {
		int mode; cin >> mode;
		if (mode == 1) {
			int l, r, c; cin >> l >> r >> c;
			add_range(0, n - 1, l - 1, r - 1, c);
		}
		else if (mode == 2) {
			int l, r; cin >> l >> r;
			sqrt_range(0, n - 1, l - 1, r - 1);
		}
		else {
			int l, r; cin >> l >> r;
			cout << search_range(0, n - 1, l - 1, r - 1) << '\n'; // TODO: change this to '\n'
		}
	}
	return 0;
}
