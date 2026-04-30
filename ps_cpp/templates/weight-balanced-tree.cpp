#pragma GCC optimize("O3")

#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

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
struct weight_balanced_tree {
  static constexpr int ALPHA = 3;

  struct node_data {
    typename monoid::Value value;
    typename monoid::Update lazy;
    bool rev;

    static node_data empty() { return node_data{monoid::identity_value(), monoid::identity_update(), false}; }
    static node_data create(typename monoid::Value v) { return node_data{v, monoid::identity_update(), false}; }
  };

  struct node {
    node_data data;
    node* left;
    node* right;
    int leaf_count;
    node() : data(node_data::empty()), left(nullptr), right(nullptr), leaf_count(0) {}
    inline bool is_leaf() const { return left == nullptr && right == nullptr; }
  };

  inline static node* allocate_node() { return new node(); }

  inline static void set_left(node* parent, node* child) {
    if (!parent) return;
    parent->left = child;
  }

  inline static void set_right(node* parent, node* child) {
    if (!parent) return;
    parent->right = child;
  }

  inline node* pull(node* t) const {
    if (!t) return nullptr;
    t->leaf_count = (t->left ? t->left->leaf_count : 0) + (t->right ? t->right->leaf_count : 0) + (t->is_leaf() ? 1 : 0);
    if (t->is_leaf()) return t;
    t->data.value = monoid::merge(t->left ? t->left->data.value : monoid::identity_value(),
                                  t->right ? t->right->data.value : monoid::identity_value());
    return t;
  }

  inline node* push(node* t) const {
    if (!t) return nullptr;
    if constexpr (monoid::has_lazy) {
      if (!t->data.rev && monoid::is_noop(t->data.lazy)) return t;
    } else {
      if (!t->data.rev) return t;
    }
    // reverse flag
    if (t->data.rev) {
      std::swap(t->left, t->right);
      if (t->left) t->left->data.rev = !t->left->data.rev;
      if (t->right) t->right->data.rev = !t->right->data.rev;
      t->data.rev = false;
    }
    if constexpr (monoid::has_lazy) {
      const auto upd = t->data.lazy;
      if (!t->is_leaf()) {
        if (t->left) {
          monoid::apply_in_place(t->left->data.value, upd, t->left->leaf_count);
          t->left->data.lazy = monoid::compose(t->left->data.lazy, upd);
        }
        if (t->right) {
          monoid::apply_in_place(t->right->data.value, upd, t->right->leaf_count);
          t->right->data.lazy = monoid::compose(t->right->data.lazy, upd);
        }
      }
      t->data.lazy = monoid::identity_update();
    }
    return pull(t);
  }

  node* merge(node* left, node* right) const {
    node* L = left;
    node* R = right;
    int left_count = L ? L->leaf_count : 0;
    int right_count = R ? R->leaf_count : 0;

    if (left_count > right_count * ALPHA) {
      if (L && (L->data.rev || (monoid::has_lazy && !monoid::is_noop(L->data.lazy)))) L = push(L);
      node* left_left = L->left;
      node* left_right = merge(L->right, R);
      int left_left_count = left_left ? left_left->leaf_count : 0;
      int left_right_count = left_right ? left_right->leaf_count : 0;
      if (left_left_count * ALPHA >= left_right_count) {
        set_left(L, left_left);
        set_right(L, left_right);
        return pull(L);
      } else {
        set_left(L, left_left);
        node* lr = left_right;
        set_right(L, lr ? lr->left : nullptr);
        L = pull(L);
        if (lr) {
          node* r = allocate_node();
          r->data = lr->data;
          set_left(r, L);
          set_right(r, lr->right);
          r = pull(r);
          return r;
        } else {
          return L;
        }
      }
    } else if (right_count > left_count * ALPHA) {
      if (R && (R->data.rev || (monoid::has_lazy && !monoid::is_noop(R->data.lazy)))) R = push(R);
      node* right_left = merge(L, R->left);
      node* right_right = R->right;
      int right_left_count = right_left ? right_left->leaf_count : 0;
      int right_right_count = right_right ? right_right->leaf_count : 0;
      if (right_left_count <= right_right_count * ALPHA) {
        set_left(R, right_left);
        set_right(R, right_right);
        return pull(R);
      } else {
        node* rl = right_left;
        set_left(R, rl ? rl->right : nullptr);
        set_right(R, right_right);
        R = pull(R);
        if (rl) {
          node* l = allocate_node();
          l->data = rl->data;
          set_left(l, rl->left);
          set_right(l, R);
          l = pull(l);
          return l;
        } else {
          return R;
        }
      }
    } else {
      if (!L && !R) return nullptr;
      if (!L) return R;
      if (!R) return L;
      node* ret = allocate_node();
      set_left(ret, L);
      set_right(ret, R);
      return pull(ret);
    }
  }

  pair<node*, node*> split(node* t, int k) const { // 0-based
    if (!t) return {nullptr, nullptr};
    if constexpr (monoid::has_lazy) {
      if (t->data.rev || !monoid::is_noop(t->data.lazy)) t = push(t);
    } else {
      if (t->data.rev) t = push(t);
    }
    if (k <= 0) return {nullptr, t};
    if (k >= t->leaf_count) return {t, nullptr};

    int left_count = t->left ? t->left->leaf_count : 0;
    if (k < left_count) {
      auto pr = split(t->left, k);
      set_left(t, pr.second);
      return {pr.first, pull(t)};
    } else if (k > left_count) {
      auto pr = split(t->right, k - left_count);
      set_right(t, pr.first);
      return {pull(t), pr.second};
    } else {
      node* L = t->left;
      node* R = t->right;
      delete t;
      return {L, R};
    }
  }

  inline tuple<node*, node*, node*> split3(node* t, int l, int r) const { // 0-based, [l, r]
    auto pr1 = split(t, l);
    auto pr2 = split(pr1.second, r - l + 1);
    return {pr1.first, pr2.first, pr2.second};
  }

  node* append(node* t, typename monoid::Value value) const {
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(t, n);
  }

  node* prepend(node* t, typename monoid::Value value) const {
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(n, t);
  }

  node* insert(node* t, int k, typename monoid::Value value) const { // 0-based
    auto pr = split(t, k);
    node* n = allocate_node();
    n->data = node_data::create(value);
    n->leaf_count = 1;
    return merge(pr.first, merge(n, pr.second));
  }

  node* erase(node* t, int k) const { // 0-based
    auto tp = split3(t, k, k);
    return merge(std::get<0>(tp), std::get<2>(tp));
  }

  node* find_kth(node* t, int k) const { // 0-based
    if (!t) return nullptr;
    if constexpr (monoid::has_lazy) {
      if (t->data.rev || !monoid::is_noop(t->data.lazy)) t = push(t);
    } else {
      if (t->data.rev) t = push(t);
    }
    if (t->is_leaf()) return (k == 0) ? t : nullptr;
    int left_count = t->left ? t->left->leaf_count : 0;
    if (k < left_count) return find_kth(t->left, k);
    return find_kth(t->right, k - left_count);
  }
};

template <class monoid>
class wbt_array {
  using tree = weight_balanced_tree<monoid>;
  using node = typename tree::node;

  tree wbt_{};
  node* root_{};

  wbt_array build(int L, int R, const vector<typename monoid::Value>& A) {
    if (L > R) return wbt_array();
    int M = (L + R) / 2;
    auto left = build(L, M - 1, A);
    auto right = build(M + 1, R, A);
    left.concat(right);
    left.insert(M - L, A[M]);
    return left;
  }

public:
  wbt_array(node* root = nullptr) : root_(root) {}
  wbt_array(const vector<typename monoid::Value>& A) : root_(nullptr) {
    for (auto& v : A) append_back(v);
  }

  int size() const { return root_ ? root_->leaf_count : 0; }

  typename monoid::Value get(int index) const { // 0-based
    node* t = wbt_.find_kth(root_, index);
    if (!t) throw out_of_range("index out of range");
    return t->data.value;
  }

  typename tree::node_data range_data(int left, int right) { // 0-based, [left, right]
    auto tp = wbt_.split3(root_, left, right);
    auto b = std::get<1>(tp);
    if (!b) throw out_of_range("range out of range");
    auto ret = b->data;
    root_ = wbt_.merge(std::get<0>(tp), wbt_.merge(b, std::get<2>(tp)));
    return ret;
  }

  void append_back(typename monoid::Value value) { root_ = wbt_.append(root_, value); }
  void append_front(typename monoid::Value value) { root_ = wbt_.prepend(root_, value); }
  void insert(int index, typename monoid::Value value) { root_ = wbt_.insert(root_, index, value); }

  void range_erase(int left, int right) { // 0-based, [left, right]
    auto [A, B, C] = wbt_.split3(root_, left, right);
    root_ = wbt_.merge(A, C);
  }

  void concat(const wbt_array& other) { root_ = wbt_.merge(root_, other.root_); }

  void erase(int index) { root_ = wbt_.erase(root_, index); }

  void range_apply(int left, int right, typename monoid::Update upd) {
    auto [A, B, C] = wbt_.split3(root_, left, right);
    if (B) {
      if (B->is_leaf()) {
        monoid::apply_in_place(B->data.value, upd, 1);
      } else {
        B->data.lazy = monoid::compose(B->data.lazy, upd);
        monoid::apply_in_place(B->data.value, upd, B->leaf_count);
      }
    }
    root_ = wbt_.merge(A, wbt_.merge(B, C));
  }

  void add(int index, typename monoid::Update upd) { range_apply(index, index, upd); }

  void range_replace(int left, int right, wbt_array& sub) {
    if (right - left + 1 != sub.size()) throw out_of_range("size mismatch");
    auto [A, B, C] = wbt_.split3(root_, left, right);
    root_ = wbt_.merge(A, wbt_.merge(sub.root_, C));
  }

  void range_reverse(int left, int right) {
    auto [A, B, C] = wbt_.split3(root_, left, right);
    if (B) B->data.rev = !B->data.rev;
    root_ = wbt_.merge(A, wbt_.merge(B, C));
  }

  // void repeat(int times) {
  //   wbt_array ret;
  //   while (times) {
  //     int size_block = 1;
  //     node* cur = root_;
  //     while (size_block * 2 <= times) {
  //       cur = wbt_.merge(cur, cur);
  //       size_block <<= 1;
  //     }
  //     times -= size_block;
  //     ret.concat(wbt_array(cur));
  //   }
  //   root_ = ret.root_;
  // }

  wbt_array slice(int left, int right) {
    auto [A, B, C] = wbt_.split3(root_, left, right);
    root_ = wbt_.merge(A, C);
    return wbt_array(B);
  }

  typename monoid::Value operator[](int index) const { return get(index); }
  typename monoid::Value operator[](pair<int, int> range_inclusive) { return range_data(range_inclusive.first, range_inclusive.second).value; }

  friend ostream& operator<<(ostream& os, const wbt_array& arr) {
    for (int i = 0; i < arr.size(); ++i) {
      os << arr.get(i);
      if (i < arr.size() - 1) os << ' ';
    }
    return os;
  }
};

int main() {
  ios::sync_with_stdio(false); cin.tie(nullptr);

  // freopen("test.txt", "r", stdin);

  int N, M, K; cin >> N >> M >> K;
  wbt_array<sum_add_monoid> arr;
  for (int i = 0; i < N; ++i) {
    ll x; cin >> x;
    arr.append_back(x);
  }

  for (int i = 0; i < M + K; ++i) {
    int a, b, c; cin >> a >> b >> c;
    if (a == 1) {
      ll d; cin >> d;
      arr.range_apply(b - 1, c - 1, d);
    } else {
      cout << arr.range_data(b - 1, c - 1).value << '\n';
    }
  }

  return 0;
}


