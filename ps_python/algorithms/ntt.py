MOD = 998244353
W = 3 # 원시근

def ntt(a, inverse=False):
    n = len(a); j = 0
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
        u = pow(W, MOD // step, MOD)
        if inverse:
            u = pow(u, MOD - 2, MOD)
        for i in range(0, n, step):
            wi = 1
            for j in range(i, i + half):
                v = a[j + half] * wi
                a[j + half] = (a[j] - v) % MOD
                a[j] = (a[j] + v) % MOD
                wi = (u * wi) % MOD
        step <<= 1

    if inverse:
        num = MOD - (MOD - 1) // n
        for i in range(n):
            a[i] = (a[i] * num) % MOD


def convolution(a, b):
    n = 1 << ((max(len(a), len(b)) - 1).bit_length() + 1)
    a.extend([0] * (n - len(a))); b.extend([0] * (n - len(b)))
    ntt(a); ntt(b)
    c = [a[i] * b[i] % MOD for i in range(n)]
    ntt(c, True)
    return c

print(max(convolution(list(map(int, input())), list(map(int, input())))))