#pragma GCC optimize("O3")
#pragma GCC target("avx2")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("inline")

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;
using ll = long long;
template<typename T, typename U> using hashmap = gp_hash_table<T, U, hash<T>, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;
template<typename T> using hashset = gp_hash_table<T, null_type, hash<T>, equal_to<T>, direct_mask_range_hashing<T>, linear_probe_fn<>, hash_standard_resize_policy<hash_exponential_size_policy<>, hash_load_check_resize_trigger<true>, true>>;
template<typename T, typename U> void init_hashmap(hashmap<T, U> &mp, int sz) { mp.resize(sz); mp.set_loads({0.2, 0.4}); }
template<typename T> void init_hashset(hashset<T> &st, int sz) { st.resize(sz); st.set_loads({0.2, 0.4}); }
template<typename T> using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
template<typename T, typename U> using ordered_map = tree<T, U, less<T>, rb_tree_tag, tree_order_statistics_node_update>;


int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);

    

    return 0;
}