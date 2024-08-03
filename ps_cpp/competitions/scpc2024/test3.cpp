#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>

using namespace std;
typedef long long int ll;

struct node
{
	int num;
	node *adj[3];
	bool visited[3];
	int cnt = 0;

	void mark(node* other){
		for (int i = 0; i < cnt; i++){
			if (adj[i] == other){
				visited[i] = true;
				for (int j = 0; j < other->cnt; j++){
					if (other->adj[j] == this){
						other->visited[j] = true;
						return;
					}
				}
			}
		}
	}
} *nodes[300002], *x = nullptr, *y = nullptr;

int n;

void find(node* cur, ll& cnt){
	for (int i = 0; i < cur->cnt; i++){
		if (cur->visited[i]) continue;
		node* nxt = cur->adj[i];
		if ((cnt == 0 && (nxt == x || nxt == y))) continue;
		cnt++;
		cur->mark(nxt);
		if ((cnt > 0 && (nxt == x || nxt == y))) return;
		find(nxt, cnt);
		return;
	}
}


int main(int argc, char** argv)
{
    std::ios_base::sync_with_stdio(false); std::cin.tie(NULL);

	// freopen("test.txt", "r", stdin);
	int t; cin >> t;
	for (int test = 0; test < t; test++){
		memset(nodes, 0, sizeof(nodes));
		cin >> n;
		for (int i = 1; i <= n; i++){
			nodes[i] = new node;
			nodes[i]->num = i; // for debug
		}
		x = nullptr, y = nullptr;
		for (int i = 0; i < n + 1; i++){
			int a, b; cin >> a >> b;
			nodes[a]->adj[nodes[a]->cnt++] = nodes[b];
			nodes[b]->adj[nodes[b]->cnt++] = nodes[a];
			if (nodes[a]->cnt == 3) (x == nullptr ? x : y) = nodes[a];
			if (nodes[b]->cnt == 3) (x == nullptr ? x : y) = nodes[b];
		}
		ll a = 0, b = 0, res = 0;
		find(x, a);
		find(x, b);
		res += a * (a - 1) >> 1; // a choose 2
		if (b > 1)
			res += b * (b - 1) >> 1; // b choose 2

		cout << "Case #" << test + 1 << '\n';
		cout << res << endl;
	}
	return 0;
}