#include <iostream>
#include <algorithm>

using namespace std;
typedef long long int ll;

int arr[300000];

int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);

	int t; cin >> t;
	for (int i = 0; i < t; ++i){
		int n; ll ans = 0; cin >> n;
		for (int j = 0; j < n; j++) cin >> arr[j];
		sort(arr, arr + n);
		for (int j = 0; j < (n >> 2); j++){
			ans += abs(arr[j] - arr[n - (n >> 2) + j]) << 1;
		}
		cout << "Case #" << i + 1 << '\n' << ans << endl;
	}


	return 0;
}