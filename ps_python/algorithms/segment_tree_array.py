import math


class ArraySegmentTree:
    def __init__(self, init_arr, init_val, merge_func):
        n = len(init_arr)
        self.init_arr = init_arr
        self.init_val = init_val
        self.merge_func = merge_func
        self.tree = [init_val for _ in range(pow(2, math.ceil(math.log(n, 2) + 1)))]
        self.loc = [0] * n
        self.build(0, n - 1, 1)

    def build(self, s, e, i=1):
        if s == e:
            self.tree[i] = self.init_arr[s]
            self.loc[s] = i
            return self.tree[i]
        m = s + e >> 1
        self.tree[i] = self.merge_func(self.build(s, m, i << 1), self.build(m + 1, e, i << 1 | 1))
        return self.tree[i]

    def search(self, s, e, l, r, i=1):
        if e < l or s > r: return self.init_val
        if l <= s and e <= r: return self.tree[i]
        m = s + e >> 1
        return self.merge_func(self.search(s, m, l, r, i << 1), self.search(m + 1, e, l, r, i << 1 | 1))

    def update_from_leaf(self, i, val):
        idx = self.loc[i]
        self.tree[idx] = val
        while idx > 1:
            idx >>= 1
            self.tree[idx] = self.merge_func(self.tree[idx << 1], self.tree[idx << 1 | 1])