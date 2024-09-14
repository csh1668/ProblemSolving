import functools


class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = {}
            self.fail = None
            self.is_end = False

        def __repr__(self):
            return f'{self.key}: {len(self.children)}, {self.is_end}'

    def __init__(self, keywords):
        self.root = self.Node('')
        self.root.fail = self.root
        for kw in keywords:
            self.insert(kw)
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

    def find(self, s):
        cur = self.root
        for i, ch in enumerate(s):
            while cur != self.root and ch not in cur.children:
                cur = cur.fail
            if ch in cur.children:
                cur = cur.children[ch]
            if cur.is_end: yield i, cur

    def contains(self, s):
        return any(self.find(s))

    def count(self, s, return_idx=False):
        if return_idx: return list(self.find(s))
        else: return functools.reduce(lambda x, y: x + 1, self.find(s), 0)