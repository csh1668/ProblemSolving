#pragma GCC optimize("O3", "inline", "omit-frame-pointer", "unroll-loops", "fast-math", "no-stack-protector", "tree-vectorize")

#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

#define likely(x) __builtin_expect(!!(x), 1) // 주로 true
#define unlikely(x) __builtin_expect(!!(x), 0) // 주로 false

struct sum_add_monoid {
  using Value = ll;   // aggregated segment value (sum)
  using Update = ll;  // lazy update (add)
  static constexpr bool has_lazy = true;

  static inline Value identity_value() { return 0LL; }
  static inline Update identity_update() { return 0LL; }
  static inline Value merge(const Value& a, const Value& b) { return a + b; }
  static inline void apply_in_place(Value& value, const Update& upd, int count) {
    value += upd * static_cast<long long>(count);
  }
  static inline Update compose(const Update& existing, const Update& incoming) { return existing + incoming; }
  static inline bool is_noop(const Update& u) { return u == 0; }
};

struct maximum_subarray_monoid {
  static constexpr ll MIN = -1e18;
  struct Value {
    ll lmx, rmx, mx, sum;
    Value(ll lmx, ll rmx, ll mx, ll sum) : lmx(lmx), rmx(rmx), mx(mx), sum(sum) {}
    Value() : lmx(MIN), rmx(MIN), mx(MIN), sum(0) {}
    Value(ll x) : lmx(x), rmx(x), mx(x), sum(x) {}
  };
  using Update = ll;
  static constexpr bool has_lazy = true;

  static inline Value identity_value() { return Value(); }
  static inline Update identity_update() { return 0; }

  static inline Value merge(const Value& a, const Value& b) {
    Value ret;
    ret.sum = a.sum + b.sum;
    ret.lmx = max(a.lmx, a.sum + b.lmx);
    ret.rmx = max(b.rmx, b.sum + a.rmx);
    ret.mx = max({a.mx, b.mx, a.rmx + b.lmx});
    return ret;
  }

  static inline void apply_in_place(Value& value, const Update& upd, int count) {
    value.sum += upd * count;
    value.lmx += upd;
    value.rmx += upd;
    value.mx += upd;
  }

  static inline Update compose(const Update& existing, const Update& incoming) {
    return existing + incoming;
  }

  static inline bool is_noop(const Update& u) { return u == 0; }
};

template <typename T>
struct value_only_monoid {
  using Value = T;
  struct Update {};
  static constexpr bool has_lazy = false;
  static inline Value identity_value() { return Value(); }
  static inline Update identity_update() { return {}; }
  static inline Value merge(const Value& a, const Value&) { return a; }
  static inline void apply_in_place(Value&, const Update&, int) {}
  static inline Update compose(const Update&, const Update&) { return {}; }
  static inline bool is_noop(const Update&) { return true; }
};

template <class monoid>
struct persistent_weight_balanced_tree {
  static constexpr int ALPHA = 5;

  struct node_data {
    typename monoid::Value value;
    typename monoid::Update lazy;
    bool rev;

    static node_data empty() { return node_data{monoid::identity_value(), monoid::identity_update(), false}; }
    static node_data create(typename monoid::Value v) { return node_data{v, monoid::identity_update(), false}; }
  };

  struct node; // 전방 선언

  inline static std::vector<node*> free_list_{};

  struct node_ptr {
    node* p;
    node_ptr() : p(nullptr) {}
    explicit node_ptr(node* raw, bool add_ref = true) : p(raw) {
      if (p && add_ref) retain(p);
    }
    node_ptr(const node_ptr& other) : p(other.p) { if (p) retain(p); } // 복사 생성자
    node_ptr(node_ptr&& other) noexcept : p(other.p) { other.p = nullptr; } // 이동 생성자
    node_ptr& operator=(const node_ptr& other) { // 복사 대입 연산자
      if (this == &other) return *this;
      if (p) release(p);
      p = other.p;
      if (p) retain(p);
      return *this;
    }
    node_ptr& operator=(node_ptr&& other) noexcept { // 이동 대입 연산자
      if (this == &other) return *this;
      if (p) release(p);
      p = other.p;
      other.p = nullptr;
      return *this;
    }
    ~node_ptr() { if (p) release(p); }
    node* get() const { return p; }
    node* operator->() const { return p; }
    explicit operator bool() const { return p != nullptr; }
  };

  struct node {
    node* left;
    node* right;
    int ref_count;
    int leaf_count;
    node_data data;

    node() : left(nullptr), right(nullptr), ref_count(1), leaf_count(0), data(node_data::empty()) {}

    __attribute__((always_inline)) inline bool is_leaf() const { return left == nullptr && right == nullptr; }
  };

  __attribute__((always_inline)) inline static node* allocate_node() {
    if (!free_list_.empty()) {
      node* n = free_list_.back();
      free_list_.pop_back();
      n->ref_count = 1;
      n->leaf_count = 0;
      return n;
    }
    return new node();
  }

  __attribute__((always_inline)) inline static void free_node(node* t) {
    t->data = node_data::empty();
    t->left = nullptr;
    t->right = nullptr;
    free_list_.push_back(t);
  }

  inline static void retain(node* t) {
    ++t->ref_count;
  }
  inline static void release(node* t) {
    if (--t->ref_count == 0) {
      node* l = t->left;
      node* r = t->right;
      free_node(t);
      if (l) release(l);
      if (r) release(r);
    }
  }

  inline static void set_left(node* parent, node* child) {
    if (!parent) return;
    if (parent->left == child) return;
    node* old = parent->left;
    parent->left = child;
    if (child) retain(child);
    if (old) release(old);
  }

  inline static void set_right(node* parent, node* child) {
    if (!parent) return;
    if (parent->right == child) return;
    node* old = parent->right;
    parent->right = child;
    if (child) retain(child);
    if (old) release(old);
  }

  inline node_ptr clone(const node_ptr& t) const {
    if (!t) return node_ptr();
    node* n = allocate_node();
    n->data = t->data;
    n->left = t->left; if (n->left) retain(n->left);
    n->right = t->right; if (n->right) retain(n->right);
    n->ref_count = 1;
    n->leaf_count = t->leaf_count;
    return node_ptr(n, false);
  }

  inline node_ptr clone(node* t) const {
    if (!t) return node_ptr();
    node* n = allocate_node();
    n->data = t->data;
    n->left = t->left; if (n->left) retain(n->left);
    n->right = t->right; if (n->right) retain(n->right);
    n->ref_count = 1;
    n->leaf_count = t->leaf_count;
    return node_ptr(n, false);
  }

  inline node_ptr make_unique(node_ptr t) const {
    if (!t) return node_ptr();
    if (t->ref_count == 1) return std::move(t); // exclusively owned, safe to mutate in place
    return clone(t);
  }

  inline node_ptr pull(node_ptr t) const {
    if (!t) return node_ptr();
    node* p = t.get();
    int left_count = p->left ? p->left->leaf_count : 0;
    int right_count = p->right ? p->right->leaf_count : 0;
    p->leaf_count = left_count + right_count + (p->is_leaf() ? 1 : 0);
    if (p->is_leaf()) return std::move(t);
    p->data.value = monoid::merge(p->left ? p->left->data.value : monoid::identity_value(),
                                  p->right ? p->right->data.value : monoid::identity_value());
    return std::move(t);
  }

  inline node_ptr push(node_ptr t) const {
    if (!t) return node_ptr();
    node* p = t.get();
    if constexpr (monoid::has_lazy) {
      if (!p->data.rev && monoid::is_noop(p->data.lazy)) return std::move(t); // nothing to propagate, avoid clone
    } else {
      if (!p->data.rev) return std::move(t);
    }
    t = make_unique(std::move(t));
    p = t.get();
    // handle reverse
    if (p->data.rev) {
      std::swap(p->left, p->right);
      if (p->left) {
        node* c = p->left;
        if (c->ref_count > 1) { node_ptr c2 = clone(c); c = c2.get(); set_left(p, c); }
        c->data.rev = !c->data.rev;
      }
      if (p->right) {
        node* c = p->right;
        if (c->ref_count > 1) { node_ptr c2 = clone(c); c = c2.get(); set_right(p, c); }
        c->data.rev = !c->data.rev;
      }
      p->data.rev = false;
    }
    if constexpr (monoid::has_lazy) {
      const auto upd = p->data.lazy;
      if (!p->is_leaf() && !monoid::is_noop(upd)) {
        if (p->left) {
          node* child = p->left;
          if (child->ref_count > 1) {
            node_ptr l = clone(child);
            child = l.get();
            set_left(p, child);
          }
          monoid::apply_in_place(child->data.value, upd, child->leaf_count);
          child->data.lazy = monoid::compose(child->data.lazy, upd);
        }
        if (p->right) {
          node* child = p->right;
          if (child->ref_count > 1) {
            node_ptr r = clone(child);
            child = r.get();
            set_right(p, child);
          }
          monoid::apply_in_place(child->data.value, upd, child->leaf_count);
          child->data.lazy = monoid::compose(child->data.lazy, upd);
        }
      }
      p->data.lazy = monoid::identity_update();
    }
    return pull(std::move(t));
  }

  node_ptr merge(node_ptr left, node_ptr right) const {
    node* L = left.get();
    node* R = right.get();
    int left_count = L ? L->leaf_count : 0;
    int right_count = R ? R->leaf_count : 0;

    bool balanced = left_count * ALPHA >= right_count && right_count * ALPHA >= left_count;

    if (balanced) {
      if (!L && !R) return node_ptr();
      if (!L) return std::move(right);
      if (!R) return std::move(left);
      node* ret = allocate_node();
      set_left(ret, L);
      set_right(ret, R);
      return pull(node_ptr(ret, false));
    } else if (left_count > right_count * ALPHA) {
      if constexpr (monoid::has_lazy) {
        if (L && (L->data.rev || !monoid::is_noop(L->data.lazy))) left = push(std::move(left)); else left = make_unique(std::move(left));
      } else {
        if (L && L->data.rev) left = push(std::move(left)); else left = make_unique(std::move(left));
      }
      L = left.get();
      node* left_left = L->left;
      node* left_right_raw = L->right;
      
      if (left_right_raw) retain(left_right_raw);
      node_ptr left_right_ptr(left_right_raw, false);
      auto left_right = merge(std::move(left_right_ptr), std::move(right));
      
      int left_left_count = left_left ? left_left->leaf_count : 0;
      int left_right_count = left_right ? left_right->leaf_count : 0;
      if (left_left_count * ALPHA >= left_right_count) {
        set_left(L, left_left);
        set_right(L, left_right.get());
        return pull(std::move(left));
      } else {
        set_left(L, left_left);
        node* lr = left_right.get();
        set_right(L, lr ? lr->left : nullptr);
        if (lr) {
          node_ptr r_clone = clone(lr);
          node* r = r_clone.get();
          set_left(r, L);
          pull(std::move(left));
          return pull(std::move(r_clone));
        } else {
          return pull(std::move(left));
        }
      }
    } else {
      if constexpr (monoid::has_lazy) {
        if (R && (R->data.rev || !monoid::is_noop(R->data.lazy))) right = push(std::move(right)); else right = make_unique(std::move(right));
      } else {
        if (R && R->data.rev) right = push(std::move(right)); else right = make_unique(std::move(right));
      }
      R = right.get();
      node* right_left_raw = R->left;
      node* right_right = R->right;
      
      if (right_left_raw) retain(right_left_raw);
      node_ptr right_left_ptr(right_left_raw, false);
      auto right_left = merge(std::move(left), std::move(right_left_ptr));
      
      int right_left_count = right_left ? right_left->leaf_count : 0;
      int right_right_count = right_right ? right_right->leaf_count : 0;
      if (right_left_count <= right_right_count * ALPHA) {
        set_left(R, right_left.get());
        set_right(R, right_right);
        return pull(std::move(right));
      } else {
        node* rl = right_left.get();
        set_left(R, rl ? rl->right : nullptr);
        set_right(R, right_right);
        if (rl) {
          node_ptr l_clone = clone(rl);
          node* l = l_clone.get();
          set_right(l, R);
          pull(std::move(right));
          return pull(std::move(l_clone));
        } else {
          return pull(std::move(right));
        }
      }
    }
  }

  pair<node_ptr, node_ptr> split(node_ptr t, int k) const { // 0-based
    if (!t) return {node_ptr(), node_ptr()};
    if constexpr (monoid::has_lazy) {
      if (t->data.rev || !monoid::is_noop(t->data.lazy)) t = push(std::move(t));
    } else {
      if (t->data.rev) t = push(std::move(t));
    }
    node* p = t.get();
    if (k <= 0) return {node_ptr(), std::move(t)};
    if (k >= p->leaf_count) return {std::move(t), node_ptr()};

    node* left_raw = p->left;
    node* right_raw = p->right;
    
    if (left_raw) retain(left_raw);
    if (right_raw) retain(right_raw);
    node_ptr left(left_raw, false);
    node_ptr right(right_raw, false);
    
    int left_count = left ? left->leaf_count : 0;
    if (k < left_count) {
      auto pr = split(std::move(left), k);
      return {std::move(pr.first), merge(std::move(pr.second), std::move(right))};
    } else if (k > left_count) {
      auto pr = split(std::move(right), k - left_count);
      return {merge(std::move(left), std::move(pr.first)), std::move(pr.second)};
    } else {
      return {std::move(left), std::move(right)};
    }
  }

  inline tuple<node_ptr, node_ptr, node_ptr> split3(node_ptr t, int l, int r) const { // 0-based, [l, r]
    auto pr1 = split(std::move(t), l);
    auto pr2 = split(std::move(pr1.second), r - l + 1);
    return {std::move(pr1.first), std::move(pr2.first), std::move(pr2.second)};
  }

  node_ptr append(node_ptr t, typename monoid::Value value) const {
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(std::move(t), node_ptr(n, false));
  }

  node_ptr prepend(node_ptr t, typename monoid::Value value) const {
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(node_ptr(n, false), std::move(t));
  }

  node_ptr insert(node_ptr t, int k, typename monoid::Value value) const { // 0-based
    auto pr = split(std::move(t), k);
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(std::move(pr.first), merge(node_ptr(n, false), std::move(pr.second)));
  }

  node_ptr erase(node_ptr t, int k) const { // 0-based
    auto tp = split3(std::move(t), k, k);
    return merge(std::move(get<0>(tp)), std::move(get<2>(tp)));
  }

  node_ptr find_kth(node_ptr t, int k) const { // 0-based
    if (!t) return node_ptr();
    if constexpr (monoid::has_lazy) {
      if (t->data.rev || !monoid::is_noop(t->data.lazy)) t = push(t);
    } else {
      if (t->data.rev) t = push(t);
    }
    node* p = t.get();
    if (p->is_leaf()) return (k == 0) ? std::move(t) : node_ptr();
    int left_count = p->left ? p->left->leaf_count : 0;
    
    if (k < left_count) {
      node* left_raw = p->left;
      if (left_raw) retain(left_raw);
      return find_kth(node_ptr(left_raw, false), k);
    } else {
      node* right_raw = p->right;
      if (right_raw) retain(right_raw);
      return find_kth(node_ptr(right_raw, false), k - left_count);
    }
  }
};

template <class monoid>
class pwbt_array {
  using tree = persistent_weight_balanced_tree<monoid>;
  using node_ptr = typename tree::node_ptr;

  tree pwbt_{};
  node_ptr root_{};

  pwbt_array build(int L, int R, const vector<typename monoid::Value>& A) const {
    if (L > R) return pwbt_array();
    int M = (L + R) / 2;
    auto left = build(L, M - 1, A);
    auto right = build(M + 1, R, A);
    return left.with_append_back(A[M]).with_concat(right);
  }

public:
  pwbt_array(node_ptr root = node_ptr()) : root_(std::move(root)) {}
  pwbt_array(const vector<typename monoid::Value>& A) : root_(build(0, A.size() - 1, A).root()) {}


  int size() const { return root_ ? root_->leaf_count : 0; }

  typename monoid::Value get(int index) const { // 0-based
    auto t = pwbt_.find_kth(node_ptr(root_), index);
    if (!t) throw out_of_range("index out of range");
    return t->data.value;
  }

  typename tree::node_data range_data(int left, int right) const { // 0-based, [left, right]
    auto tp = pwbt_.split3(node_ptr(root_), left, right);
    auto b = std::get<1>(tp);
    if (!b) throw out_of_range("range out of range");
    return b->data;
  }

  pwbt_array with_append_back(typename monoid::Value value) const {
    auto new_root = pwbt_.append(node_ptr(root_), value);
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_append_front(typename monoid::Value value) const {
    auto new_root = pwbt_.prepend(node_ptr(root_), value);
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_insert(int index, typename monoid::Value value) const { // 0-based
    auto new_root = pwbt_.insert(node_ptr(root_), index, value);
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_range_erase(int left, int right) const { // 0-based, [left, right]
    auto [A, B, C] = pwbt_.split3(node_ptr(root_), left, right);
    return pwbt_array(pwbt_.merge(std::move(A), std::move(C)));
  }

  pwbt_array with_concat(const pwbt_array& other) const {
    auto new_root = pwbt_.merge(node_ptr(root_), node_ptr(other.root_));
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_erase(int index) const { // 0-based
    auto new_root = pwbt_.erase(node_ptr(root_), index);
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_range_apply(int left, int right, typename monoid::Update upd) const {
    auto [A, B, C] = pwbt_.split3(node_ptr(root_), left, right);
    if (B) {
      B = pwbt_.make_unique(std::move(B));
      if (B->is_leaf()) {
        monoid::apply_in_place(B->data.value, upd, 1);
      } else {
        B->data.lazy = monoid::compose(B->data.lazy, upd);
        monoid::apply_in_place(B->data.value, upd, B->leaf_count);
      }
    }
    auto new_root = pwbt_.merge(std::move(A), pwbt_.merge(std::move(B), std::move(C)));
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_add(int index, typename monoid::Update upd) const {
    return with_range_apply(index, index, upd);
  }

  pwbt_array with_range_replace(int left, int right, const pwbt_array& sub) const {
    if (right - left + 1 != sub.size()) throw out_of_range("size mismatch");
    auto [A, B, C] = pwbt_.split3(node_ptr(root_), left, right);
    auto new_root = pwbt_.merge(std::move(A), pwbt_.merge(node_ptr(sub.root_), std::move(C)));
    return pwbt_array(std::move(new_root));
  }

  pwbt_array with_range_reverse(int left, int right) const {
    auto [A, B, C] = pwbt_.split3(node_ptr(root_), left, right);
    if (B) {
      B = pwbt_.make_unique(std::move(B));
      B->data.rev = !B->data.rev;
    }
    auto new_root = pwbt_.merge(std::move(A), pwbt_.merge(std::move(B), std::move(C)));
    return pwbt_array(std::move(new_root));
  }

  pwbt_array repeat(int times) const {
    pwbt_array ret;
    while (times) {
      int size = 1;
      node_ptr cur(root_);
      while (size * 2 <= times) {
        cur = pwbt_.merge(node_ptr(cur), node_ptr(cur));
        size <<= 1;
      }
      times -= size;
      ret = ret.with_concat(pwbt_array(std::move(cur)));
    }
    return ret;
  }

  pwbt_array slice(int left, int right) const {
    auto [A, B, C] = pwbt_.split3(node_ptr(root_), left, right);
    return pwbt_array(std::move(B));
  }

  pwbt_array clone() const {
    return pwbt_array(pwbt_.push(node_ptr(root_)));
  }

  node_ptr root() const { return root_; }

  typename monoid::Value operator[](int index) const { return get(index); }
  typename monoid::Value operator[](pair<int, int> range_inclusive) const { return range_data(range_inclusive.first, range_inclusive.second).value; }

  friend ostream& operator<<(ostream& os, const pwbt_array& arr) {
    for (int i = 0; i < arr.size(); ++i) {
      os << arr.get(i);
      if (i < arr.size() - 1) os << ' ';
    }
    return os;
  }
};

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  
  int N, M; cin >> N;
  vector<int> v(N);
  for (int i = 0; i < N; ++i) {
    cin >> v[i];
    // arr = arr.with_append_back(x);
  }

  pwbt_array<sum_add_monoid> arr;
  function<pwbt_array<sum_add_monoid>(int, int)> build = [&](int l, int r) -> pwbt_array<sum_add_monoid> {
    if (l > r) return pwbt_array<sum_add_monoid>();
    int m = (l + r) >> 1;
    auto L = build(l, m - 1);
    auto R = build(m + 1, r);
    return L.with_append_back(v[m]).with_concat(R);
  };
  arr = build(0, N - 1);


  cin >> M;
  while (M--) {
    int op, L, R; cin >> op >> L >> R, --L, --R;
    if (op == 1) {
      int X; cin >> X;
      arr = arr.with_range_apply(L, R, X);
    } else if (op == 2) {
      int S, E; cin >> S >> E, --S, --E;
      auto sub = arr.slice(S, E);
      arr = arr.with_range_replace(L, R, sub);
    } else {
      cout << arr.range_data(L, R).value << '\n';
    }
  }

  return 0;
}