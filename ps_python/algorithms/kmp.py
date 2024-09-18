class KMP:
    def __init__(self, pattern):
        self.p = pattern
        self.pi = [0] * len(pattern)
        j = 0
        for i in range(1, len(pattern)):
            while j > 0 and pattern[i] != pattern[j]:
                j = self.pi[j - 1]
            if pattern[i] == pattern[j]:
                j += 1
                self.pi[i] = j

    def find(self, s):
        j = 0
        for i in range(len(s)):
            while j > 0 and s[i] != self.p[j]:
                j = self.pi[j - 1]
            if s[i] == self.p[j]:
                j += 1
                if j == len(self.p):
                    yield i - len(self.p) + 1
                    j = self.pi[j - 1]