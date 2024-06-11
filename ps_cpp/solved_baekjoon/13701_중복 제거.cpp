#include <bits/stdc++.h>

using namespace std;
const int MAX = 1 << 25;

bitset<MAX> bset;

int main() {
	cin.tie(0); cout.tie(0); ios_base::sync_with_stdio(false);
	int v;
	while (cin >> v){
		if (!bset[v]){
			cout << v << " ";
			bset[v] = true;
		}
	}
	cout << endl;

	return 0;
}