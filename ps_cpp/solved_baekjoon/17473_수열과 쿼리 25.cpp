#pragma GCC optimize ("O3")
#pragma GCC optimize("Ofast")
#pragma GCC target("avx2")

// #include <iostream>
#include <bits/stdc++.h>

using namespace std;

typedef long long int ll;
typedef unsigned char bt;

struct node
{
	int mx, orv, andv;
	// mx: 구간 최댓값
	// orv: 구간 내 모든 요소들을 or한 결과
	// andv: 구간 내 모든 요소들을 and한 결과
	//
	// or 쿼리를 업데이트 하는 순간: orv와 x의 and 연산 결과가 0일 때 => 그냥 덧셈 해주면 됨
	/*
	0110 1100 0100 1110
	mx = 1110 orv = 1110 andv = 0100
	or = 0101
	0111 1101 0101 1111
	mx = 1111 orv = 1111 andv = 0101

	2 5 10 7 8
	x = 9 or
	11 13 11 15 9
	x = 9 and
	0 1 8 1 8

	x = 50 or
	50 55 58 55 58
	x = 10 or
	10 15 15 15 10

	8 24 32 40 48 56
	x = 7 or (orv & and == 0)

	51 50 2 65 11 1280
	x = 204 (1100 1100) or

	*/
} tree[524288];
int input[200000];
int lazy[524288];

inline node combine(node a, node b) {
	return { max(a.mx, b.mx), a.orv | b.orv, a.andv & b.andv };
}

node init(int s, int e, int i = 1) {
	if (s == e)
		return tree[i] = { input[s], input[s], input[s] };
	const int m = (s + e) >> 1;
	return tree[i] = combine(init(s, m, i << 1), init(m + 1, e, i << 1 | 1));
}

inline void propagate(int s, int e, int i) {
	if (lazy[i] != 0) {
		tree[i].mx += lazy[i];
		tree[i].orv += lazy[i];
		tree[i].andv += lazy[i];
		if (s != e) {
			for (auto j : { i << 1, i << 1 | 1 }) {
				lazy[j] += lazy[i];
			}
		}

		lazy[i] = 0;
	}
}

int search_max(int s, int e, int l, int r, int i = 1) {
	propagate(s, e, i);
	if (e < l || s > r)
		return 0;
	if (l <= s && e <= r)
		return tree[i].mx;
	const int m = (s + e) >> 1;
	return max(search_max(s, m, l, r, i << 1), search_max(m + 1, e, l, r, i << 1 | 1));
}

void or_range(int s, int e, int l, int r, int x, int i = 1) {
	propagate(s, e, i);
	// break condition
	if (e < l || s > r)
		return;
	// tag condition
	// x의 모든 1인 비트에 대해 구간 내 모든 비트가 1인 비트 제거
	x -= (x & tree[i].andv);
	// x의 모든 1인 비트에 대해 구간 내 모든 비트가 0이라면
	if (l <= s && e <= r && (x & ~tree[i].orv) == x) {
		// 그냥 더해주면 됨
		lazy[i] += x;
		propagate(s, e, i);
		return;
	}
	// else
	if (s == e)
		return;
	const int m = (s + e) >> 1;
	or_range(s, m, l, r, x, i << 1);
	or_range(m + 1, e, l, r, x, i << 1 | 1);
	tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
}

void and_range(int s, int e, int l, int r, int x, int i = 1) {
	propagate(s, e, i);
	// break condition
	if (e < l || s > r)
		return;
	// tag condition
	// x의 모든 0인 비트에 대해 구간 내 모든 비트가 0인 비트 제거 (1로 변경)
	x += (~x & ~tree[i].orv) & ((1 << 20) - 1);
	// x의 모든 0인 비트에 대해 구간 내 모든 비트가 1이라면
	if (l <= s && e <= r && (~x & tree[i].andv) == (~x & (1 << 20) - 1)) {
		// 그냥 빼주면 됨
		lazy[i] -= ((1 << 20) - 1 - x);
		propagate(s, e, i);
		return;
	}
	// else
	if (s == e)
		return;
	const int m = (s + e) >> 1;
	and_range(s, m, l, r, x, i << 1);
	and_range(m + 1, e, l, r, x, i << 1 | 1);
	tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
}

int main() {
	cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);

	int n, m; cin >> n;
	for (int i = 0; i < n; ++i)
		cin >> input[i];
	cin >> m;

	init(0, n - 1);
	for (int i = 0; i < m; ++i) {
		int mode, l, r, x; cin >> mode >> l >> r;
		if (mode == 3) {
			cout << search_max(0, n - 1, l - 1, r - 1) << '\n';
			continue;
		} cin >> x;
		if (mode == 2) {
			or_range(0, n - 1, l - 1, r - 1, x);
		} else {
			and_range(0, n - 1, l - 1, r - 1, x);
		}
	}
	return 0;
}