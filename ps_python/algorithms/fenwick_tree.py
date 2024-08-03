class Fenwick:
    def __init__(self, n):
        self.n = n
        self.fenwick = [0] * (n + 1)

    def find_prefix_sum(self, idx):
        ret = 0
        while idx:
            ret += self.fenwick[idx]
            idx -= idx & -idx
        return ret

    def find_sum(self, l, r):
        return self.find_prefix_sum(r) - self.find_prefix_sum(l - 1)

    def update(self, idx, val):
        while idx < self.n:
            self.fenwick[idx] += val
            idx += idx & -idx