import sys


class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = [None] * 3  # A, B, C
            self.fail = None
            self.output = 0
            self.idx = -1

        def __repr__(self):
            return f'{self.key}: {self.output}'

    def __init__(self, words):
        self.root = self.Node('')
        self.root.idx = 0
        self.trie_nodes = [self.root]
        for word in words:
            self.insert(word)
        self.build_fail()

    def insert(self, s):
        cur = self.root
        for ch in s:
            idx = ord(ch) - ord('A')
            if cur.children[idx] is None:
                cur.children[idx] = self.Node(ch)
                cur.children[idx].idx = len(self.trie_nodes)
                self.trie_nodes.append(cur.children[idx])
            cur = cur.children[idx]
        cur.output += 1

    def build_fail(self):
        from collections import deque
        q = deque([self.root])
        while q:
            cur = q.popleft()
            for i, nxt in enumerate(cur.children):
                if nxt is None: continue
                if cur == self.root:
                    nxt.fail = self.root
                else:
                    dest = cur.fail
                    while dest != self.root and dest.children[i] is None:
                        dest = dest.fail
                    if dest.children[i] is not None:
                        dest = dest.children[i]
                    nxt.fail = dest
                if nxt.fail.output: nxt.output += nxt.fail.output
                q.append(nxt)

n, k = map(int, sys.stdin.readline().split())
words = [sys.stdin.readline().strip() for _ in range(n)]
ac = AhoCorasick(words)

# dp[i][j]: 길이가 i인 문자열에서 j번 트라이 노드까지 탐색되었을 때의 최댓값
dp = [[-1] * len(ac.trie_nodes) for _ in range(k + 1)]
dp[0][0] = 0
for i in range(k):
    for j, node in enumerate(ac.trie_nodes):
        if dp[i][j] == -1: continue
        for nxt in range(3):
            cur = node
            while cur != ac.root and cur.children[nxt] is None:
                cur = cur.fail
            if cur.children[nxt] is not None:
                cur = cur.children[nxt]
            dp[i + 1][cur.idx] = max(dp[i + 1][cur.idx], dp[i][j] + cur.output)

# print(dp)
print(max(dp[-1]))