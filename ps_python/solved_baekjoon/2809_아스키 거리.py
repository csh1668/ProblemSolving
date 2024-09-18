import sys


class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = dict()
            self.fail = None
            self.output = 0

    def __init__(self, words):
        self.root = self.Node('')
        self.root.fail = self.root
        for word in words:
            self.insert(word)
        self.build_fail()

    def insert(self, s):
        cur = self.root
        for ch in s:
            if ch not in cur.children:
                cur.children[ch] = self.Node(ch)
            cur = cur.children[ch]
        cur.output = max(cur.output, len(s))

    def build_fail(self):
        from collections import deque
        q = deque([self.root])
        while q:
            cur = q.popleft()
            for ch, nxt in cur.children.items():
                if not nxt: continue
                if cur == self.root: nxt.fail = self.root
                else:
                    dest = cur.fail
                    while dest != self.root and ch not in dest.children:
                        dest = dest.fail
                    if ch in dest.children:
                        dest = dest.children[ch]
                    nxt.fail = dest
                if nxt.fail.output: nxt.output = max(nxt.output, nxt.fail.output)
                q.append(nxt)

    def find(self, s):
        cur = self.root
        for i, ch in enumerate(s):
            while cur != self.root and ch not in cur.children:
                cur = cur.fail
            if ch in cur.children:
                cur = cur.children[ch]
            if cur.output: yield i, cur.output

n = int(sys.stdin.readline())
s = sys.stdin.readline().strip()
m = int(sys.stdin.readline())

matches = []
batch_size = 200
for k in range(m // batch_size + 1):
    ac = AhoCorasick([sys.stdin.readline().strip() for _ in range(min(batch_size, m - k * batch_size))])
    for i, sz in ac.find(s):
        matches.append((i - sz + 1, i))

prv, ans = -1, 0
for l, r in sorted(matches):
    if l > prv:
        ans += l - prv - 1
    prv = max(prv, r)
ans += len(s) - prv - 1
print(ans)