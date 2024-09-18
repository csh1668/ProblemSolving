import sys

class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = {}
            self.fail = None
            self.is_end = False

    def __init__(self, patterns):
        self.root = self.Node('')
        for pattern in patterns:
            self.insert(pattern)
        self.build_fail()

    def insert(self, s):
        cur = self.root
        for ch in s:
            if ch not in cur.children:
                cur.children[ch] = self.Node(ch)
            cur = cur.children[ch]
        cur.is_end = True

    def build_fail(self):
        from collections import deque
        q = deque([self.root])
        while q:
            cur = q.popleft()
            for ch, nxt in cur.children.items():
                if cur == self.root: nxt.fail = self.root
                else:
                    dest = cur.fail
                    while dest != self.root and ch not in dest.children:
                        dest = dest.fail
                    if ch in dest.children:
                        dest = dest.children[ch]
                    nxt.fail = dest
                if nxt.fail.is_end: nxt.output = True
                q.append(nxt)

    def count_distinct(self, s):
        cur = self.root
        ans = 0
        for i, ch in enumerate(s):
            while cur != self.root and ch not in cur.children:
                cur = cur.fail
            if ch in cur.children: cur = cur.children[ch]
            if cur.is_end:
                ans += 1
                cur = self.root # 겹치는 패턴이 있으면 무시하도록
        return ans

while True:
    n, m = map(int, sys.stdin.readline().split())
    if n == 0 and m == 0: break
    patterns = [sys.stdin.readline().strip() for _ in range(n)]
    ac = AhoCorasick(patterns)
    ans = sum(ac.count_distinct(sys.stdin.readline().strip()) for _ in range(m))
    print(ans)