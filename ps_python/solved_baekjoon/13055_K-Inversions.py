mod = 998244353
w = 3


def ntt(a, inverse=False):
    n = len(a)
    j = 0
    for i in range(1, n):
        reverse = n // 2
        while j >= reverse:
            j -= reverse
            reverse //= 2
        j += reverse
        if i < j:
            a[i], a[j] = a[j], a[i]
    step = 2
    while step <= n:
        half = step // 2
        u = pow(w, mod // step, mod)
        if inverse:
            u = pow(u, mod - 2, mod)
        for i in range(0, n, step):
            wi = 1
            for j in range(i, i + half):
                v = a[j + half] * wi
                a[j + half] = (a[j] - v) % mod
                a[j] = (a[j] + v) % mod
                wi = (u * wi) % mod
        step *= 2

    if inverse:
        num = mod - (mod - 1) // n
        for i in range(n):
            a[i] = (a[i] * num) % mod


def multiply_ntt(a, b):
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


s = input()
a = []
b = []
for c in s:
    if c == 'A':
        a.append(1)
        b.append(0)
    else:
        a.append(0)
        b.append(1)
a.reverse()

c = multiply_ntt(a, b)

for i in range(len(s) - 1 - 1, -1, -1):
    print(c[i])