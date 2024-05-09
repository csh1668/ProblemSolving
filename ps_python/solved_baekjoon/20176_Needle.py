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


OFFSET = 30000
n_u = int(sys.stdin.readline())
arr_u = [0 for _ in range(-OFFSET, OFFSET + 1)]
for i in map(int, sys.stdin.readline().split()):
    arr_u[i + OFFSET] = 1
n_m = int(sys.stdin.readline())
arr_m = [0 for _ in range(-OFFSET, OFFSET + 1)]
for i in map(int, sys.stdin.readline().split()):
    arr_m[i + OFFSET] = 1
n_l = int(sys.stdin.readline())
arr_l = [0 for _ in range(-OFFSET, OFFSET + 1)]
for i in map(int, sys.stdin.readline().split()):
    arr_l[i + OFFSET] = 1

# print(arr_u)
# print(arr_m)
# print(arr_l)
# u(반대)랑 l을 곱하면 => 각각의 인덱스는 중앙을 0을 기준으로 처음과 끝의 변화량
# [0, 1, 0, 0, 0, 0, 1, 0, 1]
# [0, 0, 0, 0, 0, 1, 0, 0, 1]
# [0, 1, 0, 1, 1, 0, 1, 0, 2, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
# [0, 1, 0, 0, 1, 0, 0, 0, 1]
# 처음과 끝의 변화량이 0인 애들은 2개, 2인 애들은 1개라는 뜻
res = multiply_ntt(arr_u, arr_l)
# print(res)

cnt = 0
for i in range(2 * OFFSET + 1):
    if arr_m[i] != 0 and res[2 * i] != 0:
        cnt += res[2 * i] * arr_m[i]

print(cnt)