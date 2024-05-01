mod = 1000000000 + 7


def berlekamp_massey(x: list[int]):
    ls, cur = [], []
    lf = ld = 0
    for i in range(len(x)):
        t = 0
        for j in range(len(cur)):
            t = (t + x[i - j - 1] * cur[j]) % mod
        if (t - x[i]) % mod == 0:
            continue
        if len(cur) == 0:
            cur = [0] * (i + 1)
            lf = i
            ld = ((t - x[i]) % mod) - mod
            continue
        k = -(x[i] - t) * pow(ld, mod - 2, mod) % mod
        c = [0] * (i - lf - 1)
        c.append(k)
        for j in ls:
            c.append(-j * k % mod)
        while len(c) < len(cur):
            c.append(0)
        for j in range(len(cur)):
            c[j] = (c[j] + cur[j]) % mod
        if i - lf + len(ls) >= len(cur):
            ls, lf, ld = cur, i, t - x[i] % mod
        cur = c
    for i in cur:
        i %= mod
    return cur


def get_nth(rec, dp, n):
    m = len(rec)
    s, t = [0] * m, [0] * m
    s[0] = 1
    if m != 1:
        t[1] = 1
    else:
        t[0] = rec[0]

    def mul(v: list[int], w: list[int]):
        m = len(v)
        t = [0] * (2 * m)
        for j in range(m):
            for k in range(m):
                t[j + k] += v[j] * w[k] % mod
                if t[j + k] >= mod:
                    t[j + k] -= mod
        for j in range(2 * m - 1, m - 1, -1):
            for k in range(1, m + 1):
                t[j - k] += t[j] * rec[k - 1] % mod
                if t[j - k] >= mod:
                    t[j - k] -= mod
        t = [t[i] for i in range(m)]
        return t

    while n:
        if n & 1:
            s = mul(s, t)
        t = mul(t, t)
        n >>= 1
    res = 0
    for i in range(m):
        res += s[i] * dp[i] % mod
    return res % mod


def guess_nth_term(x: list[int], n: int):
    if n < len(x):
        return x[n]
    v = berlekamp_massey(x)
    # print(v)
    if len(v) == 0:
        return 0
    return get_nth(v, x, n)


fib_cache = dict()
a_cache = dict()
s_cache = dict()


def fib(a):
    if a <= 1:
        return 1
    if a in fib_cache.keys():
        return fib_cache[a]
    res = fib(a - 1) + fib(a - 2)
    fib_cache[a] = res
    return res


def a(i, k):
    if (i, k) in a_cache.keys():
        return a_cache[(i, k)]
    res = fib(i) * pow(i, k) % mod
    a_cache[(i, k)] = res
    return res


def s(n, k):
    res = 0
    for i in range(1, n + 1):
        res = (res + a(i, k)) % mod
    return res % mod


n, k = map(int, input().split())
brute = []
for i in range(1, min(n + 1, 167)):
    brute.append(s(i, k))
# print(brute)
print(guess_nth_term(brute, n - 1))