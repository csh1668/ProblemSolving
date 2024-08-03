import sys
sys.setrecursionlimit((10 ** 5) << 1)


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
        a, b = self.find_prefix_sum(l - 1), self.find_prefix_sum(r)
        print(l, r, a, b)
        return self.find_prefix_sum(r) - self.find_prefix_sum(l - 1)

    def update(self, idx, val):
        while idx < self.n:
            self.fenwick[idx] += val
            idx += idx & -idx


n, m = map(int, sys.stdin.readline().split())
left, right = [0] * (n + 1), [0] * (n + 1)
children = [[] for _ in range(n + 1)]
for i, v in enumerate(list(map(int, sys.stdin.readline().split()[1:]))):
    children[v].append(i + 2)


def search(i, cur):
    left[i] = cur
    cur += 1
    for child in children[i]:
        cur = search(child, cur)
    right[i] = cur
    return cur


search(1, 1)
fenwick = Fenwick(n + 5)
for _ in range(m):
    op, *args = map(int, sys.stdin.readline().split())
    if op == 1:
        fenwick.update(left[args[0]], args[1])
        fenwick.update(right[args[0]], -args[1])
    else:
        print(fenwick.find_prefix_sum(left[args[0]]))