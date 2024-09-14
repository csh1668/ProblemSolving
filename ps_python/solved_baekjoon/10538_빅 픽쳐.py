import sys


class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = [None] * 2
            self.fail = None
            self.is_end = 0

    def __init__(self, words):
        self.root = self.Node('')
        self.root.fail = self.root
        for word in words:
            self.insert(*word)
        self.build_fail()

    def insert(self, s, idx):
        cur = self.root
        for ch in s:
            ch = ch == 'o'
            if not cur.children[ch]:
                cur.children[ch] = self.Node(ch)
            cur = cur.children[ch]
        cur.is_end = idx

    def build_fail(self):
        from collections import deque
        q = deque([self.root])
        while q:
            cur = q.popleft()
            for ch, nxt in enumerate(cur.children):
                if nxt is None: continue
                if cur == self.root: nxt.fail = self.root
                else:
                    dest = cur.fail
                    while dest != self.root and not dest.children[ch]:
                        dest = dest.fail
                    if dest.children[ch]:
                        dest = dest.children[ch]
                    nxt.fail = dest
                if nxt.fail.is_end: nxt.is_end = nxt.fail.is_end
                q.append(nxt)

    def find(self, s):
        cur = self.root
        for i, ch in enumerate(s):
            ch = ch == 'o'
            while cur != self.root and not cur.children[ch]:
                cur = cur.fail
            if cur.children[ch]:
                cur = cur.children[ch]
            if cur.is_end: yield i, cur.is_end

hp, wp, hm, wm = map(int, sys.stdin.readline().split())
p = [sys.stdin.readline().strip() for _ in range(hp)]
m = [sys.stdin.readline().strip() for _ in range(hm)]

dic = dict(); cur = 1
for i in range(hp):
    if p[i] not in dic:
        dic[p[i]] = cur
        cur += 1
    p[i] = (p[i], dic[p[i]])

# Aho-Corasick
ac = AhoCorasick(p)
mark = [[0] * wm for _ in range(hm)]
for i in range(hm):
    for idx, num in ac.find(m[i]):
        mark[i][idx] = num

# KMP
t = [dic[p[i][0]] for i in range(hp)]
fail = [0] * hp
j = 0
for i in range(1, hp):
    while j > 0 and t[i] != t[j]: j = fail[j - 1]
    if t[i] == t[j]:
        j += 1
        fail[i] = j
cnt = 0
for i in range(wm):
    j = 0
    for k in range(hm):
        while j > 0 and mark[k][i] != t[j]: j = fail[j - 1]
        if mark[k][i] == t[j]:
            if j == hp - 1:
                cnt += 1
                j = fail[j]
            else:
                j += 1

print(cnt)