#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
using namespace std;
using ll = long long;
constexpr int MAXN = 150'002;
// constexpr int MAXN = 20;

struct cd_info { int cen, id, in, out; };
struct edge { int v, d, id; };

struct segtree {
	struct node {
		ll lmx, sum;
		node() : lmx(-1e18), sum(0) {}
		node(ll v) : lmx(v), sum(v) {}
		node(ll lmx, ll sum) : lmx(lmx), sum(sum) {}
		inline node operator+(const node &o) const {
			return node(max(lmx, sum + o.lmx), sum + o.sum);
		}
	};
	int n, offset;
	vector<node> t;
	segtree(int n = 1, const vector<ll> &a = vector<ll>(1, 0)) : n(n), t(1 << (33 - __builtin_clz(n))), offset(1 << (32 - __builtin_clz(n))) {
		for (int i = 0; i < n; ++i) t[i | offset] = node(a[i]);
		for (int i = offset - 1; i > 0; --i) t[i] = t[i << 1] + t[i << 1 | 1];
	}
	void update(int i, ll v) {
		i |= offset; t[i] = node(v);
		while (i >>= 1) t[i] = t[i << 1] + t[i << 1 | 1];
	}
	node query(int l, int r) {
		node lhs = node(0), rhs = node(0);
		for (l |= offset, r |= offset; l <= r; l >>= 1, r >>= 1) {
			if (l & 1) lhs = lhs + t[l++];
			if (!(r & 1)) rhs = t[r--] + rhs;
		}
		return lhs + rhs;
	}
	void debug() {
		for (int i = 1; i < offset; ++i) {
			cout << i << ' ' << t[i].lmx << ' ' << t[i].sum << '\n';
		}
		cout << '\n';

		cout << "debug_leaf: ";
		for (int i = offset; i < offset + n; ++i) {
			cout << t[i].lmx << ' ';
		}
		cout << '\n';
	}
};

struct heap {
	priority_queue<ll> pq, del; int sz;
	void push(ll x) { pq.push(x); ++sz; }
	void erase(ll v) { del.push(v); --sz; }
	void _update() {
		while (!del.empty() && !pq.empty() && del.top() == pq.top()) {
			pq.pop(); del.pop();
		}
	}
	ll top() { _update(); return pq.top(); }
	ll pop() { _update(); ll ret = pq.top(); pq.pop(); --sz; return ret; }
	int size() { return sz; }
};

vector<edge> G[MAXN];
int sz[MAXN]; bool vis[MAXN];
vector<cd_info> chain_vertex[MAXN], chain_edge[MAXN];
vector<pair<int, int>> order[MAXN];
segtree seg[MAXN];
heap h[MAXN];
vector<ll> mx_sub[MAXN];

int get_size(int u, int p) {
	sz[u] = 1;
	for (auto &[v, d, id] : G[u]) {
		if (v == p || vis[v]) continue;
		sz[u] += get_size(v, u);
	}
	return sz[u];
}

int get_centroid(int u, int p, int cap) {
	for (auto &[v, d, id] : G[u]) {
		if (v == p || vis[v]) continue;
		if (sz[v] > cap / 2) return get_centroid(v, u, cap);
	}
	return u;
}

void cd(int u, int p) {
	int size = get_size(u, p);
	int cen = get_centroid(u, p, size);
	
	vis[cen] = true;
	chain_vertex[cen].emplace_back(cen, -1, 0, (size << 1) - 1);
	order[cen].resize(G[cen].size());
	mx_sub[cen].resize(G[cen].size());
	vector<ll> a(size << 1);
	int cur = 0;
	for (int id = 0; id < G[cen].size(); ++id) {
		auto &[v, d, eid] = G[cen][id];
		if (v == p || vis[v]) continue;
		order[cen][id].first = cur + 1;
		ll x = 0, mx = 0;
		function<void(int, int, int, int)> dfs = [&](int cv, int cp, int ck, int d) {
			++cur;
			a[cur] = d;
			x += d;
			mx = max(mx, x);
			chain_vertex[cv].emplace_back(cen, id, cur, -1); int c_vertex = chain_vertex[cv].size() - 1;
			chain_edge[ck].emplace_back(cen, id, cur, -1); int c_edge = chain_edge[ck].size() - 1;
			for (auto &[nv, nd, nk] : G[cv]) {
				if (nv == cp || vis[nv]) continue;
				dfs(nv, cv, nk, nd);
			}
			++cur;
			a[cur] = -d;
			x -= d;
			chain_vertex[cv][c_vertex].out = cur;
			chain_edge[ck][c_edge].out = cur;
		};
		dfs(v, cen, eid, d);
		order[cen][id].second = cur;
		mx_sub[cen][id] = mx;
		h[cen].push(mx);
	}
	seg[cen] = segtree(size << 1, a);

	for (auto &[v, d, id] : G[cen]) {
		if (v == p || vis[v]) continue;
		cd(v, cen);
	}
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

		int N, Q; cin >> N >> Q;

		// if (N != 150'000) assert(0);  // 정답 방지

		for (int i = 1; i < N; ++i) {
			int u, v, w; cin >> u >> v >> w;
			G[u].emplace_back(v, w, i);
			G[v].emplace_back(u, w, i);
		}

		cd(1, 0);

		while (Q--) {
			int op; cin >> op;
			if (op == 1) {
				int k, x; cin >> k >> x;
				for (auto &c : chain_edge[k]) {
					auto &s = seg[c.cen];
					s.update(c.in, x);
					s.update(c.out, -x);

					h[c.cen].erase(mx_sub[c.cen][c.id]);
					const auto &[l, r] = order[c.cen][c.id];
					ll new_mx = max(s.query(l, r).lmx, 0LL);
					h[c.cen].push(new_mx);
					mx_sub[c.cen][c.id] = new_mx;
				}
			} else {
				int p; cin >> p;
				ll ans = 0;
				for (auto &c : chain_vertex[p]) {
					auto &s = seg[c.cen];
					if (c.id != -1) {
						// 센트로이드에서 p까지의 거리
						ll cen_to_p = s.query(0, c.in).sum;
						// 센트로이드에서 다른 서브트리까지의 최대 거리
						ll cen_to_other = (s.query(0, order[c.cen][c.id].first - 1) + s.query(order[c.cen][c.id].second + 1, s.n - 1)).lmx;
						// 센트로이드가 루트인 트리 상에서 p의 서브트리까지의 최대 거리
						ll p_subtree = s.query(c.in + 1, c.out - 1).lmx;
						cen_to_other = max(cen_to_other, 0LL);
						p_subtree = max(p_subtree, 0LL);
						ll dist = cen_to_p + cen_to_other + p_subtree;
						ans = max(ans, dist);
					} else {
						// p가 c.cen인 경우에는 마치 Dynamic Diameter나 트쿼5처럼 계산해준다
						auto &pq = h[c.cen];
						ll dist;
						if (pq.size() == 0) {
							dist = 0;
						} else if (pq.size() == 1) {
							dist = pq.top();
						} else {
							ll mx1 = pq.pop(), mx2 = pq.top();
							dist = mx1 + mx2;
							pq.push(mx1);
						}
						ans = max(ans, dist);
					}
				}
				cout << ans << '\n';
			}
		}

    return 0;
}