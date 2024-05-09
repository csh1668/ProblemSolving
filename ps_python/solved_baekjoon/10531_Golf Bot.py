import sys

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


n = int(sys.stdin.readline())
arr = [int(sys.stdin.readline()) for _ in range(n)]
k = [0 for _ in range(max(arr) + 1)]
for i in arr:
    k[i] = 1
m = int(sys.stdin.readline())
holes = [int(sys.stdin.readline()) for _ in range(m)]
two_shoot = multiply_ntt(k[:], k[:])

cnt = 0
len_k, len_two_shoot = len(k), len(two_shoot)
for hole in holes:
    if len_k > hole and k[hole]:
        cnt += 1
    elif len_two_shoot > hole and two_shoot[hole] > 0:
        cnt += 1
print(cnt)