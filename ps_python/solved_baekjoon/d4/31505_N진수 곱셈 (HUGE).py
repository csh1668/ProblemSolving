mod = 2281701377
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


p1, p2 = 2281701377, 2483027969
w1, w2 = 3, 3
zero, neg = '!', '~'
zero_ord = ord(zero)
isNegative = False
v1, v2 = [], []
n = int(input())
a, b = input(), input()
if a == '!' or b == '!':
    print(zero)
    exit()
for ch in reversed(a):
    if ch == neg:
        isNegative ^= True
        continue
    v1.append(ord(ch) - zero_ord)
for ch in reversed(b):
    if ch == neg:
        isNegative ^= True
        continue
    v2.append(ord(ch) - zero_ord)
v3_1 = multiply_ntt(v1[:], v2[:])
mod, w = p2, w2
v3_2 = multiply_ntt(v1, v2)
v3 = []
p2_inv = pow(p2, p1 - 2, p1)
p1_inv = pow(p1, p2 - 2, p2)
for i in range(len(v3_1)):
    a = p2 * (v3_1[i] * p2_inv % p1)
    b = p1 * (v3_2[i] * p1_inv % p2)
    v3.append((a + b) % (p1 * p2))
k = len(v3)
for i in range(k):
    v3.append(0)
if n > 0:
    for i in range(len(v3) - 1):
        v3[i + 1] += v3[i] // n
        v3[i] %= n
else:
    n = -n
    for i in range(len(v3) - 1):
        if v3[i] > 0:
            v3[i + 1] -= v3[i] // n
            v3[i] %= n
        else:
            tmp = -v3[i] // n
            if -v3[i] % n != 0:
                tmp += 1
            v3[i] = (tmp * n + v3[i]) % n
            v3[i + 1] += tmp
if isNegative:
    print(neg, end='')
flag = True
for i in range(len(v3) - 1, -1, -1):
    if flag and v3[i] == 0:
        continue
    if flag and v3[i] != 0:
        flag = False
    print(chr(v3[i] + zero_ord), end='')