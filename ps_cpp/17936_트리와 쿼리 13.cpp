#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// reference: https://mzhang2021.github.io/cp-blog/ds5/
namespace ultimate_link_cut_tree {

	struct lazy {
		int set, add;
		lazy(int s = 1, int a = 0) : set(s), add(a) {}
		bool has_lazy() {
			return set != 1 || add != 0;
		}
		lazy& operator+=(const lazy& o) {
			set = set * o.set;
			add = add * o.set + o.add;
			return *this;
		}
	};
	
	struct node_data {
		int sum, mn, mx;
		int cnt;
		node_data() : sum(0), mn(INT_MAX), mx(INT_MIN), cnt(0) {}
		node_data(int v) : sum(v), mn(v), mx(v), cnt(1) {}
		node_data(int sum, int mn, int mx, int cnt) : sum(sum), mn(mn), mx(mx), cnt(cnt) {}
		node_data operator+(const node_data& o) const {
			return node_data(sum + o.sum, min(mn, o.mn), max(mx, o.mx), cnt + o.cnt);
		}
		node_data& operator+=(const lazy& o) {
			if (cnt == 0) return *this;
			sum = o.set * sum + o.add * cnt;
			mn = o.set * mn + o.add;
			mx = o.set * mx + o.add;
			return *this;
		}
	};
	
	struct node {
		node *p, *ch[4];
		node_data val, path, sub, all;
		lazy path_lazy, sub_lazy;
		bool rev, fake;
		node() { init(), fake = true; }
		template<typename... Args>
		node(Args... args) { init(), val = path = all = node_data(args...); }
		inline void init() {
			p = ch[0] = ch[1] = ch[2] = ch[3] = 0;
			val = path = sub = all = node_data();
			path_lazy = sub_lazy = lazy();
			rev = false;
			fake = false;
		}
	};
	using node_ptr = node*;
	
	namespace allocator {
		vector<node_ptr> free_nodes;
		node_ptr new_fake_node() {
			if (free_nodes.empty()) {
				return new node();
			}
			auto n = free_nodes.back();
			free_nodes.pop_back();
			n->fake = true;
			return n;
		}
		template<typename... Args>
		node_ptr new_node(Args... args) {
			if (free_nodes.empty()) {
				return new node(args...);
			}
			auto n = free_nodes.back();
			free_nodes.pop_back();
			n->val = n->path = n->all = node_data(args...);
			return n;
		}
		void free_node(node_ptr n) {
			if (n == 0) return;
			n->init();
			free_nodes.push_back(n);
		}
	}
	
	// 스플레이 트리 상에서 n의 두 자식을 뒤집는다.
	void reverse(node_ptr n) {
		if (n == 0) return;
		swap(n->ch[0], n->ch[1]);
		n->rev ^= 1;
	}
	
	void add_lazy_path(node_ptr n, const lazy& l) {
		if (n == 0 || n->fake) return;
		n->val += l; n->path += l;
		n->all = n->sub + n->path;
		n->path_lazy += l;
	}
	
	void add_lazy_sub(node_ptr n, const lazy& l, bool is_virt = false) {
		if (n == 0) return;
		n->sub += l;
		n->sub_lazy += l;
		if (is_virt && !n->fake) {
			add_lazy_path(n, l);
		} else {
			n->all = n->sub + n->path;
		}
	}
	
	void push(node_ptr n) {
		if (n == 0) return;
		if (n->rev) {
			for (int i = 0; i < 2; ++i) reverse(n->ch[i]);
			n->rev = false;
		}
		if (n->path_lazy.has_lazy()) {
			for (int i = 0; i < 2; ++i) add_lazy_path(n->ch[i], n->path_lazy);
			n->path_lazy = lazy();
		}
		if (n->sub_lazy.has_lazy()) {
			for (int i = 0; i < 4; ++i) add_lazy_sub(n->ch[i], n->sub_lazy, i >= 2);
			n->sub_lazy = lazy();
		}
	}
	
	void pull(node_ptr n) {
		if (n == 0) return;
		// fake node: subtree, all
		// not fake node: path, sub, all;
		if (!n->fake) {
			// n->path = n->ch[0]->path + n->ch[1]->path + n->val;
			n->path = n->val;
			for (int i = 0; i < 2; ++i) if (n->ch[i]) n->path = n->path + n->ch[i]->path;
		}
		// n->sub = n->ch[0]->sub + n->ch[1]->sub + n->ch[2]->all + n->ch[3]->all;
		n->sub = node_data();
		for (int i = 0; i < 2; ++i) if (n->ch[i]) n->sub = n->sub + n->ch[i]->sub;
		for (int i = 2; i < 4; ++i) if (n->ch[i]) n->sub = n->sub + n->ch[i]->all;
		n->all = n->sub + n->path;
	}
	
	inline void set_child(node_ptr p, node_ptr c, int idx) {
		p->ch[idx] = c;
		if (c != 0) c->p = p;
		pull(p);
	}
	
	inline int dir(node_ptr n, bool is_virt = false) {
		if (n->p == 0) return -1;
		int offset = is_virt ? 2 : 0;
		for (int i = offset; i < offset + 2; ++i) {
			if (n->p->ch[i] == n) return i;
		}
		return -1;
	}
	
	void rotate(node_ptr n, bool is_virt = false) {
		auto p = n->p, g = p->p;
		int nd = dir(n, is_virt), pd = dir(p, is_virt);
		if (pd == -1 && is_virt == false) pd = dir(p, true);
		auto c = n->ch[nd ^ 1];
		set_child(p, c, nd);
		set_child(n, p, nd ^ 1);
		if (pd != -1) {
			set_child(g, n, pd);
		} else {
			n->p = g;
		}
	}
	
	void splay(node_ptr n, bool is_virt = false) {
		push(n);
		while (dir(n, is_virt) != -1 && (is_virt == false || n->p->fake)) {
			auto p = n->p, g = p->p;
			push(g), push(p), push(n);
			int nd = dir(n, is_virt), pd = dir(p, is_virt);
			if (pd != -1 && (is_virt == false || g->fake)) rotate(nd == pd ? p : n, is_virt);
			rotate(n, is_virt);
		}
	}
	
	void add_virtual_child(node_ptr p, node_ptr c) {
		if (c == 0) return;
		for (int i = 2; i < 4; ++i) {
			if (p->ch[i] == 0) {
				set_child(p, c, i);
				return;
			}
		}
		auto fake_node = allocator::new_fake_node();
		set_child(fake_node, p->ch[2], 2);
		set_child(fake_node, c, 3);
		set_child(p, fake_node, 2);
	}
	
	void remove_virtual_child(node_ptr c) {
		static function<void(node_ptr)> force_push = [&](node_ptr n) {
			if (n == 0) return;
			if (n->fake) force_push(n->p);
			push(n);
		};
		auto p = c->p;
		force_push(p);
		if (p->fake) {
			auto g = p->p;
			// g -> p -> c이고 p가 fake node라면
			// p와 c를 제거하여 g와 p의 다른 자식을 연결
			set_child(g, p->ch[dir(c, true) ^ 1], dir(p, true));
			allocator::free_node(p);
			if (g->fake) splay(g, true);
		} else {
			set_child(p, 0, dir(c, true));
		}
		c->p = 0;
	}
	
	node_ptr find_non_fake_ancestor(node_ptr n) {
		auto p = n->p;
		if (p->fake) {
			splay(p, true);
			return p->p;
		}
		return p;
	}
	
	void access(node_ptr n) {
		splay(n);
		// n의 오른쪽 자식들을 virtual child로 만든다
		add_virtual_child(n, n->ch[1]);
		set_child(n, 0, 1);
		for (; n->p; splay(n)) {
			auto p = find_non_fake_ancestor(n);
			splay(p); // fake node가 아닌 부모를 찾는다.
			remove_virtual_child(n); // 
			add_virtual_child(p, p->ch[1]);
			set_child(p, n, 1);
		}
	}
	
	void make_root(node_ptr n) {
		access(n);
		reverse(n);
	}
	
	void link(node_ptr u, node_ptr v) {
		make_root(u);
		access(v);
		add_virtual_child(v, u);
	}
	
	void cut(node_ptr u, node_ptr v) {
		make_root(u);
		access(v);
		u->p = 0;
		set_child(v, 0, 0);
	}

	void cut_parent(node_ptr root, node_ptr n) {
		make_root(root);
		access(n);
		if (n->ch[0] == 0) return;
		n->ch[0] = n->ch[0]->p = 0;
		pull(n);
	}

	node_ptr find_parent(node_ptr n) {
		access(n);
		auto p = n->ch[0];
		if (p == 0) return 0;
		for (push(p); p->ch[1]; p = p->ch[1]) push(p);
		splay(p);
		return p;
	}
	
	node_ptr find_root(node_ptr n) {
		access(n);
		for (push(n); n->ch[0]; n = n->ch[0]) push(n);
		splay(n);
		return n;
	}
	
	bool is_connected(node_ptr u, node_ptr v) {
		make_root(u);
		return find_root(u) == find_root(v);
	}
	
	void update_path(node_ptr u, node_ptr v, const lazy& l) {
		make_root(u);
		access(v);
		add_lazy_path(v, l);
	}
	
	void update_subtree(node_ptr root, node_ptr n, const lazy& l) {
		make_root(root);
		access(n);
		n->val += l;
		for (int i = 2; i < 4; ++i) add_lazy_sub(n->ch[i], l, true);
	}

	void update_vertex(node_ptr n, const lazy& l) {
		access(n);
		n->val += l;
		pull(n);
	}
	
	node_data query_path(node_ptr u, node_ptr v) {
		make_root(u);
		access(v);
		return v->path;
	}
	
	node_data query_subtree(node_ptr root, node_ptr n) {
		make_root(root);
		access(n);
		node_data ret = n->val;
		for (int i = 2; i < 4; ++i) if (n->ch[i]) ret = ret + n->ch[i]->all;
		return ret;
	}
	
}

using namespace ultimate_link_cut_tree;

int main() {
	ios::sync_with_stdio(false); cin.tie(nullptr);

	int N, M; cin >> N >> M;

	// if (N < 100000) assert(0);
	vector<node_ptr> nodes(N + 1);
	for (int i = 1; i <= N; ++i) nodes[i] = ultimate_link_cut_tree::allocator::new_node(0);

	vector<pair<int, int>> E;
	for (int i = 0; i < N - 1; ++i) {
		int u, v; cin >> u >> v;
		// link(nodes[u], nodes[v]);
		E.emplace_back(u, v);
	}
	for (int i = 1; i <= N; ++i) {
		int x; cin >> x;
		nodes[i] = ultimate_link_cut_tree::allocator::new_node(x);
	}
	for (auto [u, v] : E) {
		link(nodes[u], nodes[v]);
	}
	int r; cin >> r;

	while (M--) {
		int op; cin >> op;
		if (op == 0) {
			int x, y; cin >> x >> y;
			update_subtree(nodes[r], nodes[x], lazy(0, y));
		} else if (op == 1) {
			cin >> r;
		} else if (op == 2) {
			int x, y, z; cin >> x >> y >> z;
			update_path(nodes[x], nodes[y], lazy(0, z));
		} else if (op == 3) {
			int x; cin >> x;
			cout << query_subtree(nodes[r], nodes[x]).mn << '\n';
		} else if (op == 4) {
			int x; cin >> x;
			cout << query_subtree(nodes[r], nodes[x]).mx << '\n';
		} else if (op == 5) {
			int x, y; cin >> x >> y;
			update_subtree(nodes[r], nodes[x], lazy(1, y));
		} else if (op == 6) {
			int x, y, z; cin >> x >> y >> z;
			update_path(nodes[x], nodes[y], lazy(1, z));
		} else if (op == 7) {
			int x, y; cin >> x >> y;
			cout << query_path(nodes[x], nodes[y]).mn << '\n';
		} else if (op == 8) {
			int x, y; cin >> x >> y;
			cout << query_path(nodes[x], nodes[y]).mx << '\n';
		} else if (op == 9) {
			int x, y; cin >> x >> y;
			make_root(nodes[r]);
			auto xp = find_parent(nodes[x]);
			cut_parent(nodes[r], nodes[x]);
			if (is_connected(nodes[x], nodes[y])) {
				if (xp != 0) link(nodes[x], xp);
			} else {
				link(nodes[x], nodes[y]);
			}
		} else if (op == 10) {
			int x, y; cin >> x >> y;
			cout << query_path(nodes[x], nodes[y]).sum << '\n';
		} else if (op == 11) {
			int x; cin >> x;
			cout << query_subtree(nodes[r], nodes[x]).sum << '\n';
		}
	}

	return 0;
}