import math
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
eratos = [1 for _ in range(MAX)]
eratos[0] = eratos[1] = 0
# primes = [2]
for i in range(2, MAX):
    if eratos[i]:
        j = i
        # primes.append(i)
        while i + j < len(eratos):
            eratos[i + j] = 0
            j += i
semi = [0, 0, 0, 0]
for i in range(4, MAX):
    if i & 1 == 0 and eratos[i >> 1]:
        semi.append(1)
    else:
        semi.append(0)

eratos[2] = 0
c = multiply(eratos[:], semi[:])

t = int(input())
for _ in range(t):
    n = int(sys.stdin.readline())
    print(c[n])