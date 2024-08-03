#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>

using namespace std;
typedef long long int ll;

int a[100000], b[100000];


int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);

	int t; cin >> t;
	for (int test = 0; test < t; test++){
		int n; ll l; cin >> n >> l;
		for (int i = 0; i < n; i++) cin >> a[i];
		for (int i = 0; i < n; i++) cin >> b[i];
		sort(a, a + n); sort(b, b + n);
		int res = -1;
		do
		{
			int score = 0;
			for (int i = 0; i < n; i++){
				score = max(score, abs(a[i] - b[i]));
			}
			if (score <= l){
				res = max(res, score);
			}
		} while (next_permutation(b, b + n));

		cout << "Case #" << test + 1 << '\n';
		cout << res << endl;
		
	}

	return 0;
}