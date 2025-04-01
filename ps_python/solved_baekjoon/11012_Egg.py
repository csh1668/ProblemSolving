import sys


class PST:
    class Node:
        def __init__(self, v, left=None, right=None):
            self.v = v
            self.left = left
            self.right = right

    def __init__(self, n):
        self.N = n
        self.roots = [self.build(0, n)]

    def build(self, s, e):
        if s == e: return PST.Node(0)
        m = s + e >> 1
        l = self.build(s, m); r = self.build(m + 1, e)
        return PST.Node(l.v + r.v, l, r)

    def update(self, node, s, e, i, v):
        if s == e: return PST.Node(node.v + v)
        m = s + e >> 1
        if i <= m:
            l = self.update(node.left, s, m, i, v)
            return PST.Node(l.v + node.right.v, l, node.right)
        else:
            r = self.update(node.right, m + 1, e, i, v)
            return PST.Node(r.v + node.left.v, node.left, r)

    def query(self, l, r, s, e):
        return self._query(self.roots[r], self.roots[l - 1], 0, self.N, s, e)

    def _query(self, a, b, s, e, l, r):
        if r < s or e < l: return 0
        if l <= s and e <= r: return a.v - b.v
        m = s + e >> 1
        return self._query(a.left, b.left, s, m, l, r) + self._query(a.right, b.right, m + 1, e, l, r)

MX = 100000 + 5
for _ in range(int(sys.stdin.readline())):
    N, M = map(int, sys.stdin.readline().split())
    pst = PST(MX)
    sw = [[] for _ in range(MX)]
    for _ in range(N):
        x, y = map(lambda x: int(x) + 1, sys.stdin.readline().split())
        sw[x].append(y)
    for i in range(1, 100002):
        prv = pst.roots[i - 1]
        nxt = PST.Node(prv.v, prv.left, prv.right)
        for j in sw[i]:
            nxt = pst.update(nxt, 0, MX, j, 1)
        pst.roots.append(nxt)
    ans = 0
    for _ in range(M):
        a, b, c, d = map(lambda x: int(x) + 1, sys.stdin.readline().split())
        ans += pst.query(a, b, c, d)
    print(ans)