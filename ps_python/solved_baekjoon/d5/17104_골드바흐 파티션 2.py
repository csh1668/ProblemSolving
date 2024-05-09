import sys
import time

mod = 998244353
w = 3


def power_mod(a, n):
    value = 1
    while n:
        if n & 1:
            value = (value * a) % mod
        a = (a * a) % mod
        n >>= 1
    return value


def ntt(a, inverse=False):
    n = len(a)
    j = 0
    for i in range(1, n):
        reverse = n >> 1
        while j >= reverse:
            j -= reverse
            reverse >>= 1
        j += reverse
        if i < j:
            a[i], a[j] = a[j], a[i]
    step = 2
    while step <= n:
        half = step >> 1
        u = power_mod(w, mod // step)
        if inverse:
            u = power_mod(u, mod - 2)
        for i in range(0, n, step):
            wi = 1
            for j in range(i, i + half):
                v = a[j + half] * wi
                a[j + half] = (a[j] - v) % mod
                a[j] = (a[j] + v) % mod
                wi = (u * wi) % mod
        step <<= 1
    if inverse:
        num = mod - (mod - 1) // n
        for i in range(n):
            a[i] = (a[i] * num) % mod


def multiply(a, b):
    n = 1
    while n < len(a):
        n <<= 1
    while n < len(b):
        n <<= 1
    n <<= 1
    while len(a) < n:
        a.append(0)
    while len(b) < n:
        b.append(0)

    ntt(a)
    ntt(b)
    c = []
    for i in range(n):
        c.append(a[i] * b[i])
    ntt(c, True)
    return c


MAX = 1000001
eratos = [1 for _ in range(3, MAX, 2)]  # [0] = 3, [1] = 5, [2] = 7, [3] = 9, [4] = 11, [5] = 13, [6] = 15
# primes = []
for i in range(3, MAX, 2):
    idx = ((i - 1) // 2) - 1
    if eratos[idx]:
        j = i
        # primes.append(i)
        while idx + j < len(eratos):
            k = idx + j
            eratos[idx + j] = 0
            j += i

c = multiply(eratos[:], eratos[:])

t = int(input())
for _ in range(t):
    n = int(sys.stdin.readline())
    if n == 4:
        print(1)
        continue
    cnt = 1 if (n >> 1) & 1 and eratos[((n >> 1) - 3) >> 1] else 0
    cnt += c[(n - 6) >> 1] >> 1
    print(cnt)
