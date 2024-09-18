import sys


class Fenwick:
    def __init__(self, n):
        self.n = n
        self.tree = [0] * (n + 1)

    def update(self, i, diff):
        while i <= self.n:
            self.tree[i] += diff
            i += i & -i

    def prefix(self, i):
        ret = 0
        while i > 0:
            ret += self.tree[i]
            i -= i & -i
        return ret

    def query(self, l, r):
        return self.prefix(r) - self.prefix(l - 1)

for _ in range(int(sys.stdin.readline())):
    n, k = map(int, sys.stdin.readline().split())
    fw = Fenwick(n + 3)
    for _ in range(k):
        op, l, r = map(int, sys.stdin.readline().split())
        l, r = l + 1, r + 1
        if op == 0:
            fw.update(l, 1)
            fw.update(r, -1)
        else:
            print('YES' if fw.query(l, r) == 0 else 'NO')