#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <utility>

using namespace std;
using namespace __gnu_pbds;
typedef long long int ll;
typedef pair<int, int> undirected_edge;
template<typename T, typename U> using hashmap = gp_hash_table<T, U, hash<T>>;

namespace std {
  template<> struct hash<undirected_edge> {
    inline size_t operator()(const undirected_edge &e) const {
      size_t a = e.first, b = e.second;
      if (a > b) swap(a, b);
      a = hash<int>()(a); b = hash<int>()(b);
      a *= 0x9e3779b97f4a7c15;
      return a ^ (b + (a << 6) + (a >> 2));
    }
  };
}

class LCT {
public:
  struct Node {
    Node *l, *r, *p;
    int cnt, v; ll sum; bool flip;
    Node(int v = 0) : l(0), r(0), p(0), cnt(1), v(v), sum(v), flip(0) {}
    bool is_root() { return !p || (p->l != this && p->r != this); }
    bool is_left() { return p && p->l == this; }
    void push() { if (flip) { swap(l, r); l && (l->flip ^= 1); r && (r->flip ^= 1); flip = 0; } }
    void pull() { cnt = 1, sum = v; if (l) cnt += l->cnt, sum += l->sum; if (r) cnt += r->cnt, sum += r->sum; }
    void rotate() {
        if (is_left()) {
            if (r) r->p = p;
            p->l = r; r = p;
        } else {
            if (l) l->p = p;
            p->r = l; l = p;
        }
        if (!p->is_root()) (p->is_left() ? p->p->l : p->p->r) = this;
        auto t = p; p = t->p; t->p = this;
        t->pull(); pull();
    }
  };
  vector<Node> vertices;
  hashmap<undirected_edge, Node*> edges;
  LCT(int N) : vertices(N) {}

  void link(int u, int v, int w) {
    auto u_node = &vertices[u];
    auto v_node = &vertices[v];
    auto e = new Node(w);
    edges[undirected_edge(min(u, v), max(u, v))] = e;
    make_root(u_node); link(u_node, e);
    make_root(v_node); link(v_node, e);
  }

  void cut(int u, int v) {
    auto u_node = &vertices[u];
    auto v_node = &vertices[v];
    auto it = edges.find(undirected_edge(min(u, v), max(u, v)));
    if (it == edges.end()) throw invalid_argument("edge not found");
    auto e = it->second;
    edges.erase(it->first);
    make_root(u_node); cut(e);
    make_root(e); cut(v_node);
    delete e;
  }

  bool connected(int u, int v) {
    auto u_node = &vertices[u];
    auto v_node = &vertices[v];
    make_root(u_node); return find_root(v_node) == u_node;
  }

  ll query(int u, int v) {
    auto u_node = &vertices[u];
    auto v_node = &vertices[v];
    auto lca = find_lca(u_node, v_node);
    ll ret = lca->v;
    access(u_node); splay(lca);
    if (lca->r) ret += lca->r->sum;
    access(v_node); splay(lca);
    if (lca->r) ret += lca->r->sum;
    return ret;
  }

private:
  void splay(Node *x) {
    for (; !x->is_root(); x->rotate()) {
      if (!x->p->is_root()) x->p->p->push();
      x->p->push(); x->push();
      if (x->p->is_root()) continue;
      if (x->is_left() == x->p->is_left()) x->p->rotate();
      else x->rotate();
    }
    x->push();
  }

  void access(Node *x) {
    splay(x); x->r = 0; x->pull();
    for (; x->p; splay(x)) {
      splay(x->p); x->p->r = x; x->p->pull();
    }
  }

  // c의 부모가 p가 된다
  void link(Node *c, Node *p) {
    access(c); access(p);
    c->l = p; p->p = c; c->pull();
  }

  // x와 x의 부모의 연결을 제거한다
  void cut(Node *x) {
    access(x); if (!x->l) return;
    x->l->p = 0; x->l = 0; x->pull();
  }

  Node *find_root(Node *x) {
    access(x); while (x->l) x = x->l;
    splay(x); return x;
  }

  void make_root(Node *x) {
    access(x); splay(x); x->flip ^= 1;
  }

  Node *find_lca(Node *u, Node *v) {
    access(u); access(v); splay(u);
    return u->p ? u->p : u;
  }
};

class ETT {
public:
  struct Node {
    Node *l, *r, *p;
    int cnt, prio, idx;
    Node(int idx) : l(0), r(0), p(0), cnt(1), prio(rand()), idx(idx) {}

    void pull() { cnt = 1; if (l) cnt += l->cnt; if (r) cnt += r->cnt; }

    void set_child(Node *c, bool is_left) {
      if (is_left) l = c;
      else r = c;
      if (c) c->p = this;
      pull();
    }

    Node *find_root() {
      auto x = this;
      while (x->p) x = x->p;
      return x;
    }

    Node *find_predecessor() {
      auto x = this;
      if (x->l) {
        x = x->l;
        while (x->r) x = x->r;
        return x;
      } else {
        while (x->p && x == x->p->l) x = x->p;
        return x->p;
      }
    }
    
    int get_idx() { // 1-indexed
      int idx = l ? l->cnt + 1 : 1;
      Node *x = this, *p = x->p;
      while (p) {
        if (x == p->r) idx += p->l ? p->l->cnt + 1 : 1;
        x = p; p = x->p;
      }
      return idx;
    }

    Node *split() {
      int k = get_idx();
      auto root = find_root();
      auto [l, r] = split(root, k);
      return r;
    }

    static Node* with_null_parent(Node *x) {
      if (x && x->p) x->p = 0;
      return x;
    }

    static pair<Node*, Node*> split(Node *root, int k) { // 1-indexed
      if (!root) return {0, 0};
      int left_cnt = root->l ? root->l->cnt : 0;
      if (k <= left_cnt) {
        auto [l, r] = split(root->l, k);
        root->set_child(r, true);
        return {with_null_parent(l), with_null_parent(root)};
      } else {
        auto [l, r] = split(root->r, k - left_cnt - 1);
        root->set_child(l, false);
        return {with_null_parent(root), with_null_parent(r)};
      }
    }

    static Node* merge_roots(Node *l, Node *r) {
      if (!l || !r) return with_null_parent(l ? l : r);
      if (l->prio > r->prio) {
        l->set_child(merge_roots(l->r, r), false);
        return l;
      } else {
        r->set_child(merge_roots(l, r->l), true);
        return r;
      }
    }

    static Node* merge(Node *l, Node *r) {
      if (!l || !r) return l ? l : r;
      l = l->find_root(); r = r->find_root();
      if (l == r) throw invalid_argument("l and r are the same node");
      return merge_roots(l, r);
    }
  };

  vector<Node> vertices;
  hashmap<undirected_edge, pair<Node*, Node*>> edges;
  ETT(int N) {
    vertices.reserve(N);
    for (int i = 0; i < N; ++i) vertices.emplace_back(i);
  }

  bool connected(int u, int v) {
    auto u_node = &vertices[u];
    auto v_node = &vertices[v];
    return u_node->find_root() == v_node->find_root();
  }

  void link(int u, int v) {
    if (u > v) swap(u, v);
    auto edge = undirected_edge(u, v);

    auto u_to_v = new Node(-1);
    auto v_to_u = new Node(-1);
    edges[edge] = {u_to_v, v_to_u};

    auto u_self = &vertices[u];
    auto v_self = &vertices[v];
    auto u_succ = u_self->split();
    auto v_succ = v_self->split();

    Node::merge(u_self, u_to_v);
    Node::merge(u_self, v_succ);
    Node::merge(u_self, v_self);
    Node::merge(u_self, v_to_u);
    Node::merge(u_self, u_succ);
  }

  void cut(int u, int v) {
    if (u > v) swap(u, v);
    auto edge = undirected_edge(u, v);
    if (edges.find(edge) == edges.end()) throw invalid_argument("edge not found");

    auto [u_to_v, v_to_u] = edges[edge];
    edges.erase(edge);

    auto u_to_v_succ = u_to_v->split();
    bool order = u_to_v->find_root() == v_to_u->find_root();

    auto v_to_u_succ = v_to_u->split();
    auto u_to_v_pred = u_to_v->find_predecessor();
    if (u_to_v_pred) u_to_v_pred->split();
    auto v_to_u_pred = v_to_u->find_predecessor();
    if (v_to_u_pred) v_to_u_pred->split();

    if (order) {
      Node::merge(v_to_u_pred, u_to_v_succ);
    } else {
      Node::merge(u_to_v_pred, v_to_u_succ);
    }

    delete u_to_v; delete v_to_u;
  }

  void debug(Node* x) {
    if (!x) return;
    debug(x->l);
    cout << x->idx << ' ';
    debug(x->r);
  }
};

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  srand(time(0));

  int N, Q; cin >> N;
  LCT lct(N); ETT ett(N);
  for (int i = 0; i < N - 1; ++i) {
    int u, v, w; cin >> u >> v >> w;
    lct.link(u, v, w);
    ett.link(u, v);
  }

  cin >> Q;
  while (Q--) {
    int op; cin >> op;
    if (op == 1) {
      int u, v, w, x; cin >> u >> v >> w >> x;
      lct.cut(u, v); ett.cut(u, v);
      lct.link(v, w, x); ett.link(v, w);
    } else {
      int k; cin >> k;
      vector<pair<int, int>> S(k);
      for (auto &[order, xk] : S) {
        cin >> xk;
        order = ett.vertices[xk].get_idx();
      }
      sort(S.begin(), S.end());

      ll ans = 0;
      for (int i = 0; i < k; ++i) {
        int u = S[i].second, v = S[(i + 1) % k].second;
        ans += lct.query(u, v);
      }

      cout << ans / 2 << '\n';
    }
  }

  return 0;
}