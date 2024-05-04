p = 998244353
w = 3


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
        u = pow(w, p // step, p)
        if inverse:
            u = pow(u, p - 2, p)
        for i in range(0, n, step):
            wi = 1
            for j in range(i, i + half):
                v = a[j + half] * wi
                a[j + half] = (a[j] - v) % p
                a[j] = (a[j] + v) % p
                wi = (u * wi) % p
        step <<= 1

    if inverse:
        num = p - (p - 1) // n
        for i in range(n):
            a[i] = (a[i] * num) % p


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


N = int(input())
a = list(map(int, input().split()))
b = list(map(int, input().split()))
b.reverse()
c = multiply(a, b)
result = 0
for i in range(N):
    if i >= N - 1:
        result = max(result, c[i])
    else:
        result = max(result, c[i] + c[i + N])
print(result)
