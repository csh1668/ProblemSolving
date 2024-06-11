#pragma GCC optimize ("O3")
#pragma GCC optimize("Ofast")
#pragma GCC optimization("unroll-loops")
#pragma GCC target("avx,avx2,fma")
#include<bits/stdc++.h>

#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Author : jinhan814
 * Date : 2021-05-06
 * Source : https://blog.naver.com/jinhan814/222266396476
 * Description : FastIO implementation for cin, cout. (mmap ver.)
 */
constexpr int SZ = 1 << 20;

class INPUT {
private:
	char* p;
	bool __END_FLAG__, __GETLINE_FLAG__;
public:
	explicit operator bool() { return !__END_FLAG__; }
    INPUT() {
        struct stat st; fstat(0, &st);
        p = (char*)mmap(0, st.st_size, PROT_READ, MAP_SHARED, 0, 0);
    }
	bool IsBlank(char c) { return c == ' ' || c == '\n'; }
	bool IsEnd(char c) { return c == '\0'; }
	char _ReadChar() { return *p++; }
	char ReadChar() {
		char ret = _ReadChar();
		for (; IsBlank(ret); ret = _ReadChar());
		return ret;
	}
	template<typename T> T ReadInt() {
		T ret = 0; char cur = _ReadChar(); bool flag = 0;
		for (; IsBlank(cur); cur = _ReadChar());
		if (cur == '-') flag = 1, cur = _ReadChar();
		for (; !IsBlank(cur) && !IsEnd(cur); cur = _ReadChar()) ret = 10 * ret + (cur & 15);
		if (IsEnd(cur)) __END_FLAG__ = 1;
		return flag ? -ret : ret;
	}
	string ReadString() {
		string ret; char cur = _ReadChar();
		for (; IsBlank(cur); cur = _ReadChar());
		for (; !IsBlank(cur) && !IsEnd(cur); cur = _ReadChar()) ret.push_back(cur);
		if (IsEnd(cur)) __END_FLAG__ = 1;
		return ret;
	}
	double ReadDouble() {
		string ret = ReadString();
		return stod(ret);
	}
	string getline() {
		string ret; char cur = _ReadChar();
		for (; cur != '\n' && !IsEnd(cur); cur = _ReadChar()) ret.push_back(cur);
		if (__GETLINE_FLAG__) __END_FLAG__ = 1;
		if (IsEnd(cur)) __GETLINE_FLAG__ = 1;
		return ret;
	}
	friend INPUT& getline(INPUT& in, string& s) { s = in.getline(); return in; }
} _in;

class OUTPUT {
private:
	char write_buf[SZ];
	int write_idx;
public:
	~OUTPUT() { Flush(); }
	explicit operator bool() { return 1; }
	void Flush() {
        write(1, write_buf, write_idx);
		write_idx = 0;
	}
	void WriteChar(char c) {
		if (write_idx == SZ) Flush();
		write_buf[write_idx++] = c;
	}
	template<typename T> int GetSize(T n) {
		int ret = 1;
		for (n = n >= 0 ? n : -n; n >= 10; n /= 10) ret++;
		return ret;
	}
	template<typename T> void WriteInt(T n) {
		int sz = GetSize(n);
		if (write_idx + sz >= SZ) Flush();
		if (n < 0) write_buf[write_idx++] = '-', n = -n;
		for (int i = sz; i --> 0; n /= 10) write_buf[write_idx + i] = n % 10 | 48;
		write_idx += sz;
	}
	void WriteString(string s) { for (auto& c : s) WriteChar(c); }
	void WriteDouble(double d) { WriteString(to_string(d)); }
} _out;

/* operators */
INPUT& operator>> (INPUT& in, char& i) { i = in.ReadChar(); return in; }
INPUT& operator>> (INPUT& in, string& i) { i = in.ReadString(); return in; }
template<typename T, typename std::enable_if_t<is_arithmetic_v<T>>* = nullptr>
INPUT& operator>> (INPUT& in, T& i) {
	if constexpr (is_floating_point_v<T>) i = in.ReadDouble();
	else if constexpr (is_integral_v<T>) i = in.ReadInt<T>(); return in; }

OUTPUT& operator<< (OUTPUT& out, char i) { out.WriteChar(i); return out; }
OUTPUT& operator<< (OUTPUT& out, string i) { out.WriteString(i); return out; }
template<typename T, typename std::enable_if_t<is_arithmetic_v<T>>* = nullptr>
OUTPUT& operator<< (OUTPUT& out, T i) {
	if constexpr (is_floating_point_v<T>) out.WriteDouble(i);
	else if constexpr (is_integral_v<T>) out.WriteInt<T>(i); return out; }

/* macros */
#define fastio 1
#define cin _in
#define cout _out
#define istream INPUT
#define ostream OUTPUT
/////////////////////////////////////////////////////////////////////////////////////////////

// #include <algorithm>
// #include <iostream>
// #include <stdlib.h>

// FastIO를 사용해야 하나..?
// 비재귀로 구현해야 하나..?
// https://velog.io/@statco19/segment-tree-non-recursive

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

inline void propagate_lazy(int start, int end, int i) {
	if (set_lazy[i] == 0){
		if (add_lazy[i] != 0){
			tree[i].range_max += add_lazy[i];
			tree[i].range_min += add_lazy[i];
			tree[i].range_sum += (end - start + 1) * add_lazy[i];
			if (start != end){
				add_lazy[i << 1] += add_lazy[i];
				add_lazy[i << 1 | 1] += add_lazy[i];
			}
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
		auto sqrt_min = sqrt_ll(tree[i].range_min);
		if (sqrt_min == sqrt_ll(tree[i].range_max)){
			set_lazy[i] = sqrt_min;
			propagate_lazy(start, end, i);
			return;
		}
		if (tree[i].range_min + 1 == tree[i].range_max){
			add_lazy[i] = sqrt_min - tree[i].range_min;
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
	int t; cin >> t;
	for(int i = 0; i < t; i++){
		int n, q; cin >> n >> q;
		for (int i = 0; i < n; i++)
			cin >> arr[i];
		init(0, n - 1, 1);
		memset(add_lazy, 0, 262144 * sizeof(ll));
		memset(set_lazy, 0, 262144 * sizeof(ll));
		// fill_n(add_lazy, 262144, 0);
		// fill_n(set_lazy, 262144, 0);
		for (int i = 0; i < q; i++) {
			int mode; cin >> mode;
			if (mode == 3) {
				int l, r, c; cin >> l >> r >> c;
				add_range(0, n - 1, l - 1, r - 1, c);
			}
			else if (mode == 1) {
				int l, r; cin >> l >> r;
				sqrt_range(0, n - 1, l - 1, r - 1);
			}
			else {
				int l, r; cin >> l >> r;
				cout << search_range(0, n - 1, l - 1, r - 1) << '\n'; // TODO: change this to '\n'
			}
		}
	}
	return 0;
}
