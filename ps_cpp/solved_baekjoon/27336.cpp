#pragma GCC optimization("unroll-loops")
#pragma GCC optimize("O3")

#include <iostream>
#include <vector>
#include <algorithm>
#include <array>

using std::vector;

constexpr int MAX = 1000000001;

inline int min(const int& a, const int& b, const int& c, const int& d){
	int res = a;
	if (res > b)
		res = b;
	if (res > c)
		res = c;
	if (res > d)
		res = d;
	return res;
}

inline int max(const int& a, const int& b, const int& c, const int& d){
	int res = a;
	if (res < b)
		res = b;
	if (res < c)
		res = c;
	if (res < d)
		res = d;
	return res;
}

inline int cal_diff(const std::array<vector<int>, 4>& arr, const int* indices){
	int min_val = min(arr[0][indices[0]], arr[1][indices[1]], arr[2][indices[2]], arr[3][indices[3]]);
	int max_val = max(arr[0][indices[0]], arr[1][indices[1]], arr[2][indices[2]], arr[3][indices[3]]);
	return max_val - min_val;
}

int main(){
	int indices[] = {0, 0, 0, 0};
	int n, m;
	std::cin >> n;
	m = n - 1;
	std::array<vector<int>, 4> arr;
	for (int i = 0; i < 4; ++i){
		arr[i].resize(n);
		for (int j = 0; j < n; ++j)
			std::cin >> arr[i][j];
		std::sort(arr[i].begin(), arr[i].begin() + n);
	}
	int res = cal_diff(arr, indices);
	while (indices[0] != m || indices[1] != m || indices[2] != m || indices[3] != m){
		int nxt_idx = 0, nxt_diff = MAX;
		for(int i = 0; i < 4; ++i){
			if (indices[i] + 1 < n){
				indices[i] += 1;
				int tmp = cal_diff(arr, indices);
				if (nxt_diff > tmp){
					nxt_diff = tmp;
					nxt_idx = i;
				}
				indices[i] -= 1;
			}
		}
		indices[nxt_idx] += 1;
		if (res > nxt_diff){
			res = nxt_diff;
		}
	}
	std::cout << res;
}