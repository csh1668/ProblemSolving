import sys


class AhoCorasick:
    class Node:
        def __init__(self, key):
            self.key = key
            self.children = [None] * 26
            self.fail = None
            self.output = None

    def __init__(self, words):
        self.root = self.Node('')
        for word in words:
            self.insert(word)
        self.build_fail()

    def insert(self, s):
        cur = self.root
        for c in s:
            idx = ord(c) - ord('A')
            if not cur.children[idx]:
                cur.children[idx] = self.Node(c)
            cur = cur.children[idx]
        cur.output = s

    def build_fail(self):
        from collections import deque
        q = deque([self.root])
        while q:
            cur = q.popleft()
            for i, nxt in enumerate(cur.children):
                if nxt is None: continue
                if cur == self.root: nxt.fail = self.root
                else:
                    dest = cur.fail
                    while dest != self.root and not dest.children[i]:
                        dest = dest.fail
                    if dest.children[i]:
                        dest = dest.children[i]
                    nxt.fail = dest
                if nxt.fail.output is not None: nxt.output = nxt.fail.output
                q.append(nxt)

    def find(self, s):
        cur = self.root
        for i, c in enumerate(s):
            idx = ord(c) - ord('A')
            while cur != self.root and cur.children[idx] is None:
                cur = cur.fail
            if cur.children[idx]:
                cur = cur.children[idx]
            if cur.output is not None: yield i - len(cur.output) + 1, cur.output

class Iterator:
    def __init__(self, grid, direction):
        self.grid = grid
        self.direction = direction
        self.L = len(grid)
        self.C = len(grid[0])

    def __iter__(self):
        if self.direction == 'A':
            return self._north()
        elif self.direction == 'B':
            return self._northeast()
        elif self.direction == 'C':
            return self._east()
        elif self.direction == 'D':
            return self._southeast()
        elif self.direction == 'E':
            return self._south()
        elif self.direction == 'F':
            return self._southwest()
        elif self.direction == 'G':
            return self._west()
        elif self.direction == 'H':
            return self._northwest()
        else:
            raise ValueError("Invalid direction")

    def _north(self):
        for c in range(self.C):
            yield [self.grid[r][c] for r in range(self.L - 1, -1, -1)]

    def _northeast(self):
        for r in range(self.L):
            yield [self.grid[r - c][c] for c in range(min(r + 1, self.C))]
        for c in range(1, self.C):
            yield [self.grid[self.L - 1 - r][c + r] for r in range(min(self.L, self.C - c))]

    def _east(self):
        for r in range(self.L):
            yield [self.grid[r][c] for c in range(self.C)]

    def _southeast(self):
        for c in range(self.C - 1, 0, -1):
            yield [self.grid[r][c + r] for r in range(min(self.L, self.C - c))]
        for r in range(self.L):
            yield [self.grid[r + c][c] for c in range(min(self.C, self.L - r))]

    def _south(self):
        for c in range(self.C):
            yield [self.grid[r][c] for r in range(self.L)]

    def _southwest(self):
        for c in range(self.C - 1, -1, -1):
            yield [self.grid[r][c - r] for r in range(min(self.L, c + 1))]
        for r in range(1, self.L):
            yield [self.grid[r + c][self.C - 1 - c] for c in range(min(self.C, self.L - r))]

    def _west(self):
        for r in range(self.L):
            yield [self.grid[r][c] for c in range(self.C - 1, -1, -1)]

    def _northwest(self):
        for r in range(self.L - 1, -1, -1):
            yield [self.grid[r - c][self.C - 1 - c] for c in range(min(r + 1, self.C))]
        for c in range(self.C - 2, -1, -1):
            yield [self.grid[self.L - 1 - r][c - r] for r in range(min(self.L, c + 1))]

class IndexTracker:
    def __init__(self, L, C, direction):
        self.grid = grid
        self.direction = direction
        self.L = L
        self.C = C

    def __iter__(self):
        if self.direction == 'A':
            return self._north()
        elif self.direction == 'B':
            return self._northeast()
        elif self.direction == 'C':
            return self._east()
        elif self.direction == 'D':
            return self._southeast()
        elif self.direction == 'E':
            return self._south()
        elif self.direction == 'F':
            return self._southwest()
        elif self.direction == 'G':
            return self._west()
        elif self.direction == 'H':
            return self._northwest()
        else:
            raise ValueError("Invalid direction")

    def _north(self):
        for c in range(self.C):
            yield [(r, c) for r in range(self.L - 1, -1, -1)]

    def _northeast(self):
        for r in range(self.L):
            yield [(r - c, c) for c in range(min(r + 1, self.C))]
        for c in range(1, self.C):
            yield [(self.L - 1 - r, c + r) for r in range(min(self.L, self.C - c))]

    def _east(self):
        for r in range(self.L):
            yield [(r, c) for c in range(self.C)]

    def _southeast(self):
        for c in range(self.C - 1, 0, -1):
            yield [(r, c + r) for r in range(min(self.L, self.C - c))]
        for r in range(self.L):
            yield [(r + c, c) for c in range(min(self.C, self.L - r))]

    def _south(self):
        for c in range(self.C):
            yield [(r, c) for r in range(self.L)]

    def _southwest(self):
        for c in range(self.C - 1, -1, -1):
            yield [(r, c - r) for r in range(min(self.L, c + 1))]
        for r in range(1, self.L):
            yield [(r + c, self.C - 1 - c) for c in range(min(self.C, self.L - r))]

    def _west(self):
        for r in range(self.L):
            yield [(r, c) for c in range(self.C - 1, -1, -1)]

    def _northwest(self):
        for r in range(self.L - 1, -1, -1):
            yield [(r - c, self.C - 1 - c) for c in range(min(r + 1, self.C))]
        for c in range(self.C - 2, -1, -1):
            yield [(self.L - 1 - r, c - r) for r in range(min(self.L, c + 1))]

L, C, W = map(int, sys.stdin.readline().split())
grid = [list(sys.stdin.readline().rstrip()) for _ in range(L)]
words = [sys.stdin.readline().rstrip() for _ in range(W)]

directions = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H']
ac = AhoCorasick(words)
res = {word: (1001, 1001, 'H') for word in words}

for direction in directions:
    for line, cords in zip(Iterator(grid, direction), IndexTracker(L, C, direction)):
        for idx, word in ac.find(line):
            cur = (cords[idx][0], cords[idx][1], direction)
            res[word] = min(res[word], cur)

for word in words:
    print(res[word][0], res[word][1], res[word][2])


# ac = AhoCorasick(words)
#
# res = [(1001, 1001, 'H') for _ in range(W)]
# # 북쪽: A, 북동쪽: B, 동쪽: C, 남동쪽: D, 남쪽: E, 남서쪽: F, 서쪽: G, 북서쪽: H
# # 남쪽: c++
# for c in range(C):
#